#include "spi.h"

extern ReaderConfigStruct								ReaderConfig;
__IO bool																CR95HF_TimeOut;

void SPI_Two_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStructure;
	
	SPI_I2S_DeInit(FLASH_SPI);
	RCC_APB2PeriphClockCmd(FLASH_SPI_GPIO_CLK ,ENABLE);						//����SPI���ڶ˿�ʱ��
  RCC_APB2PeriphClockCmd(FLASH_SPI_CLK, ENABLE);								//����SPIʱ��
	/* ����SPI�ܽ�: SCK, MISO and MOSI */
	GPIO_InitStructure.GPIO_Pin 				= FLASH_SPI_SCK_PIN | 
																				FLASH_SPI_MISO_PIN |
																				FLASH_SPI_MOSI_PIN;
	GPIO_InitStructure.GPIO_Speed 			= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode 				= GPIO_Mode_AF_PP;
	GPIO_Init(FLASH_SPI_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_SPI_PORT,FLASH_SPI_SCK_PIN | FLASH_SPI_MISO_PIN | FLASH_SPI_MOSI_PIN);
	/* ����SPI�ܽ�  NCS */		
	GPIO_InitStructure.GPIO_Pin   			= FLASH_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_NCS_GPIO_PORT, FLASH_NCS_PIN);  
	GPIO_InitStructure.GPIO_Pin					= CR95HF_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(CR95HF_NCS_GPIO_PORT, CR95HF_NCS_PIN);  
	
	SPI_StructInit(&SPI_InitStructure);								//��ʼ��SPIĬ��ֵ
	SPI_InitStructure.SPI_Direction					= SPI_Direction_2Lines_FullDuplex;	//˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode 							= SPI_Mode_Master;						//SPI������ģʽ
	SPI_InitStructure.SPI_DataSize					= SPI_DataSize_8b;						//8λ����
	SPI_InitStructure.SPI_CPOL 							= SPI_CPOL_High;							//
	SPI_InitStructure.SPI_CPHA 							= SPI_CPHA_2Edge;							//
	SPI_InitStructure.SPI_NSS  							= SPI_NSS_Soft;								//���NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;	//
	SPI_InitStructure.SPI_FirstBit					= SPI_FirstBit_MSB;						//��MSB��ʼ����
	SPI_InitStructure.SPI_CRCPolynomial			=	7;
	SPI_Init(FLASH_SPI, &SPI_InitStructure);															//
	SPI_Cmd(FLASH_SPI, ENABLE);																						//����SPI
}

int8_t SPI_SendReceive(uc8 *pCommand, uint8_t *pResponse)
{
	*pResponse =CR95HF_ERRORCODE_DEFAULT;
	*(pResponse+1) = 0x00;
	// ��һ��  ��������
	CR95HF_Send_SPI_Command(pCommand);
	// �ڶ���  ��ѯ
	if (CR95HF_PollingCommand( ) != CR95HF_SUCCESS_CODE)
	{	*pResponse =CR95HF_ERRORCODE_TIMEOUT;
		return CR95HF_POLLING_CR95HF;												//����0x00
	}
	// ������  - ��ȡ����
	CR95HF_Receive_SPI_Response(pResponse);
// update global variable case of shadow mode
	SpyTransaction(pCommand,pResponse);
	return RESULTOK; 
}

static void CR95HF_Send_SPI_Command(uc8 *pData)
{
	uint8_t DummyBuffer[256];
	//CR95HFƬѡ��Ч
	CR95HF_NSS_LOW();
	//������������  0x00
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_SEND);
	if(*pData == ECHO)
	{
		//���͵�������ECHO��CR95HF 
		SPI_SendReceiveByte(CR95HF_SPI, ECHO);
	}
	else
	{
		//ͨ��SPI���ͻ�������
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

	StartTimeOut(30000);						//���ó�ʱʱ��			
	while( Polling_Status 	!= CR95HF_FLAG_DATA_READY && 
					 CR95HF_TimeOut != TRUE )
	{
		//CR95HFƬѡ��Ч
		CR95HF_NSS_LOW();
		// ����Polling����
		SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
		// Poll CR95HF ֱ������׼����
		Polling_Status  = SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_POLLING);
		Polling_Status &= CR95HF_FLAG_DATA_READY_MASK;
		// �ͷ�CR95HFƬѡ 
		CR95HF_NSS_HIGH();		
	}
	StopTimeOut();									//�����ʱʱ��
	if ( CR95HF_TimeOut == TRUE )		//��ʱ���� ��ʱ  0x01
		return CR95HF_POLLING_TIMEOUT;
	return CR95HF_SUCCESS_CODE;			//��������  0x00
}

void CR95HF_Receive_SPI_Response(uint8_t *pData)
{
	uint8_t DummyBuffer[MAX_BUFFER_SIZE];
	// CR95HF Ƭѡ��Ч
	CR95HF_NSS_LOW();
	// ���Ͷ�ȡ��������
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_RECEIVE);
	pData[CR95HF_COMMAND_OFFSET] = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);
	if(pData[CR95HF_COMMAND_OFFSET] == ECHO)					//���յ�ECHO	����Ϊ0
		 pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else if(pData[CR95HF_COMMAND_OFFSET] == 0xFF)			//���յ�0xFF ����Ϊ��������Ϊ0
		pData[CR95HF_LENGTH_OFFSET]  = 0x00;
	else
	{
		// ���³���
		pData[CR95HF_LENGTH_OFFSET]  = SPI_SendReceiveByte(CR95HF_SPI, DUMMY_BYTE);
		// У�鳤�� ����������
		if(pData[CR95HF_LENGTH_OFFSET] != 0x00)
			SPI_SendReceiveBuffer(CR95HF_SPI, DummyBuffer, pData[CR95HF_LENGTH_OFFSET], &pData[CR95HF_DATA_OFFSET]);
	}
	//CR95HFƬѡʧЧ
	CR95HF_NSS_HIGH();
}












