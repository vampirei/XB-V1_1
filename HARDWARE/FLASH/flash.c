#include "flash.h"

void SPI_FLASH_Init(void)
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
//向FLASH发送一个字节
u8 FLASH_SendByte(u8 byte)
{
	return SPI_SendReceiveByte(FLASH_SPI,byte);
}
//读取SPI_FLASH状态寄存器
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	FLASH_NCS_LOW();                            
	FLASH_SendByte(FLASH_Read_Status_Reg);
	byte=FLASH_SendByte(0Xff);            
	FLASH_NCS_HIGH();                          
	return byte;   
} 
//等待空闲
void Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   
} 
u8 FLASH_Read_ID(u8 *pResponse)
{
	FLASH_NCS_LOW();																				//FLASH片选有效
	SPI_SendReceiveByte(FLASH_SPI,FLASH_Device_ID);					//发送读取ID命令字
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//发送读取ID地址  0x000000
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//发送读取ID地址  0x000000
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//发送读取ID地址  0x000000
	*pResponse = SPI_SendReceiveByte(FLASH_SPI,0xFF);				//读取第一个字节  应该为0xEF
	pResponse++;
	*pResponse = SPI_SendReceiveByte(FLASH_SPI,0xFF);				//读取第二个字节  应该为0x16
	FLASH_NCS_HIGH();																				//FLASH片选无效
	return 0;
}
//FLASH写使能
void FLASH_Write_En(void)   
{
	FLASH_NCS_LOW();                           //????   
  FLASH_SendByte(FLASH_Write_Enable);      //?????  
	FLASH_NCS_HIGH();                           //????     	      
} 
//全片擦除
//W25X64:40s 
void Flash_Format(void)   
{                                             
  FLASH_Write_En();                  //SET WEL 
  Flash_Wait_Busy();   
  FLASH_NCS_LOW();                
  FLASH_SendByte(FLASH_Chip_Erase);
	FLASH_NCS_HIGH();                  	      
	Flash_Wait_Busy();   				
}
//SPI在一页(0~65535)内写入少于256个字节数据
//在指定地址写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不能超过该页剩余字节数!!!	 
void Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
	u32 TempAddr;
	TempAddr=WriteAddr<<8;
  FLASH_Write_En();                  //SET WEL 
	FLASH_NCS_LOW();                              
  FLASH_SendByte(FLASH_Page_Program);     
  FLASH_SendByte((u8)((TempAddr)>>16));   
  FLASH_SendByte((u8)((TempAddr)>>8));   
  FLASH_SendByte((u8)TempAddr);       
//  FLASH_SendByte((u8)((WriteAddr)>>16));   
//  FLASH_SendByte((u8)((WriteAddr)>>8));   
//  FLASH_SendByte((u8)WriteAddr);   
  for(i=0;i<NumByteToWrite;i++)
	{
		FLASH_SendByte(pBuffer[i]);
	}
	FLASH_NCS_HIGH();                           
	Flash_Wait_Busy();					   
} 
//读取SPI FLASH  
//在指定地址开始读取指定长度数据
//pBuffer:数据存储区
//ReadAddr:开始读取地址(24bit)
//NumByteToRead:要读取的字节数(最大65535)
void Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;    		
	u32 TempAddr;		
	TempAddr=ReadAddr<<8;	
	FLASH_NCS_LOW();                              
  FLASH_SendByte(FLASH_Read_Data);      
  FLASH_SendByte((u8)((TempAddr)>>16));   
  FLASH_SendByte((u8)((TempAddr)>>8));   
  FLASH_SendByte((u8)TempAddr);            
//  FLASH_SendByte((u8)((ReadAddr)>>16)); 
//  FLASH_SendByte((u8)((ReadAddr)>>8));   
//  FLASH_SendByte((u8)ReadAddr);   
  for(i=0;i<NumByteToRead;i++)
	{ 
    pBuffer[i]=FLASH_SendByte(0XFF); 
  }
	FLASH_NCS_HIGH();                         	      
}  















