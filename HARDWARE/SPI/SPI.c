#include "spi.h"

extern ReaderConfigStruct								ReaderConfig;
__IO bool																CR95HF_TimeOut;

void SPI_Two_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_I2S_DeInit(FLASH_SPI);
	RCC_APB2PeriphClockCmd(FLASH_SPI_GPIO_CLK ,ENABLE);						//允许SPI所在端口时钟
  RCC_APB2PeriphClockCmd(FLASH_SPI_CLK, ENABLE);								//允许SPI时钟
	/* 配置SPI管脚: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin 				= FLASH_SPI_SCK_PIN | 
																				FLASH_SPI_MISO_PIN |
																				FLASH_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed 			= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 				= GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_SPI_PORT,FLASH_SPI_SCK_PIN | FLASH_SPI_MISO_PIN | FLASH_SPI_MOSI_PIN);
	/* 配置SPI管脚  NCS */		
	GPIO_InitStructure.GPIO_Pin   			= FLASH_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_NCS_GPIO_PORT, FLASH_NCS_PIN);  
	GPIO_InitStructure.GPIO_Pin					= CR95HF_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(CR95HF_NCS_GPIO_PORT, CR95HF_NCS_PIN);  
	
	SPI_StructInit(&SPI_InitStructure);								//初始化SPI默认值
	SPI_InitStructure.SPI_Direction					= SPI_Direction_2Lines_FullDuplex;	//双向双线全双工
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;						//SPI工作主模式
	SPI_InitStructure.SPI_DataSize					= SPI_DataSize_8b;						//8位数据
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;							//
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;							//
	SPI_InitStructure.SPI_NSS  							= SPI_NSS_Soft;								//软件NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//
	SPI_InitStructure.SPI_FirstBit					= SPI_FirstBit_MSB;						//从MSB开始发送
	SPI_InitStructure.SPI_CRCPolynomial			=	7;
	SPI_Init(FLASH_SPI, &SPI_InitStructure);															//
	SPI_Cmd(FLASH_SPI, ENABLE);																						//允许SPI
}

int8_t SPI_SendReceive(uc8 *pCommand, uint8_t *pResponse)
{
	*pResponse =CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	// 第一步  发送命令
	CR95HF_Send_SPI_Command(pCommand);
	// 第二步  查询
	if (CR95HF_PollingCommand( ) != CR95HF_SUCCESS_CODE)
	{	*pResponse =CR95HF_ERRORCODE_TIMEOUT;
		return CR95HF_POLLING_CR95HF;												//返回0x00
	}
	// 第三步  - 读取数据
	CR95HF_Receive_SPI_Response(pResponse);
// update global variable case of shadow mode
	SpyTransaction(pCommand,pResponse);
	return RESULTOK; 
}

static void CR95HF_Send_SPI_Command(uc8 *pData)
{
	uint8_t DummyBuffer[256];
	//CR95HF片选有效
	CR95HF_NSS_LOW();
	//发送命令请求  0x00
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_SEND);
	if(*pData == ECHO)
	{
		//发送单个命令ECHO到CR95HF 
		SPI_SendReceiveByte(CR95HF_SPI, ECHO);
	}
	else
	{
		//通过SPI发送缓存数据
		SPI_SendReceiveBuffer(CR95HF_SPI, pData, pData[CR95HF_LENGTH_OFFSET]+CR95HF_DATA_OFFSET, DummyBuffer);
	}

	//Deselect CR95HF over SPI 
	CR95HF_NSS_HIGH();
}

uint8_t SPI_SendReceiveByte(SPI_TypeDef* SPIx, uint8_t data) 
{	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPIx, data);	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(SPIx);
}

void SPI_SendReceiveBuffer(SPI_TypeDef* SPIx, uc8 *pCommand, uint8_t length, uint8_t *pResponse) 
{
uint8_t i;
	
	// the buffer size is limited to SPI_RESPONSEBUFFER_SIZE
	length = MIN (SPI_RESPONSEBUFFER_SIZE,length);
	for(i=0; i<length; i++)
		pResponse[i] = SPI_SendReceiveByte(SPIx, pCommand[i]);
}

static int8_t CR95HF_PollingCommand( void )
{
	uint8_t Polling_Status = 0;

	StartTimeOut(30000);						//设置超时时间			
	while( Polling_Status 	!= CR95HF_FLAG_DATA_READY && 
					 CR95HF_TimeOut != TRUE )
	{
		//CR95HF片选有效
		CR95HF_NSS_LOW();
		// 发送Polling请求
		SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
		// Poll CR95HF 直到数据准备好
		Polling_Status  = SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
		Polling_Status &= CR95HF_FLAG_DATA_READY_MASK;
		// 释放CR95HF片选 
		CR95HF_NSS_HIGH();		
	}
	StopTimeOut();									//清除超时时间
	if ( CR95HF_TimeOut == TRUE )		//超时返回 超时  0x01
		return CR95HF_POLLING_TIMEOUT;
	return CR95HF_SUCCESS_CODE;			//返回正常  0x00
}

void CR95HF_Receive_SPI_Response(uint8_t *pData)
{
	uint8_t DummyBuffer[MAX_BUFFER_SIZE];
	// CR95HF 片选有效
	CR95HF_NSS_LOW();
	// 发送读取数据请求
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_RECEIVE);
	pData[CR95HF_COMMAND_OFFSET] = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);
	if(pData[CR95HF_COMMAND_OFFSET] == ECHO)					//接收到ECHO	长度为0
		 pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else if(pData[CR95HF_COMMAND_OFFSET] == 0xFF)			//接收到0xFF 估计为出错，长度为0
		pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else
	{
		// 更新长度
		pData[CR95HF_LENGTH_OFFSET]  = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);
		// 校验长度 并接收数据
		if(pData[CR95HF_LENGTH_OFFSET] != 0x00)
			SPI_SendReceiveBuffer(CR95HF_SPI, DummyBuffer, pData[CR95HF_LENGTH_OFFSET], &pData[CR95HF_DATA_OFFSET]);
	}
	//CR95HF片选失效
	CR95HF_NSS_HIGH();
}












