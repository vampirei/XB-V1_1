#include "flash.h"

void SPI_FLASH_Init(void)
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
//��FLASH����һ���ֽ�
u8 FLASH_SendByte(u8 byte)
{
	return SPI_SendReceiveByte(FLASH_SPI,byte);
}
//��ȡSPI_FLASH״̬�Ĵ���
u8 SPI_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	FLASH_NCS_LOW();                            
	FLASH_SendByte(FLASH_Read_Status_Reg);
	byte=FLASH_SendByte(0Xff);            
	FLASH_NCS_HIGH();                          
	return byte;   
} 
//�ȴ�����
void Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR()&0x01)==0x01);   
} 
u8 FLASH_Read_ID(u8 *pResponse)
{
	FLASH_NCS_LOW();																				//FLASHƬѡ��Ч
	SPI_SendReceiveByte(FLASH_SPI,FLASH_Device_ID);					//���Ͷ�ȡID������
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//���Ͷ�ȡID��ַ  0x000000
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//���Ͷ�ȡID��ַ  0x000000
	SPI_SendReceiveByte(FLASH_SPI,0x00);										//���Ͷ�ȡID��ַ  0x000000
	*pResponse = SPI_SendReceiveByte(FLASH_SPI,0xFF);				//��ȡ��һ���ֽ�  Ӧ��Ϊ0xEF
	pResponse++;
	*pResponse = SPI_SendReceiveByte(FLASH_SPI,0xFF);				//��ȡ�ڶ����ֽ�  Ӧ��Ϊ0x16
	FLASH_NCS_HIGH();																				//FLASHƬѡ��Ч
	return 0;
}
//FLASHдʹ��
void FLASH_Write_En(void)   
{
	FLASH_NCS_LOW();                           //????   
  FLASH_SendByte(FLASH_Write_Enable);      //?????  
	FLASH_NCS_HIGH();                           //????     	      
} 
//ȫƬ����
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
//SPI��һҳ(0~65535)��д������256���ֽ�����
//��ָ����ַд�����256�ֽڵ�����
//pBuffer:���ݴ洢��
//WriteAddr:��ʼд���ַ(24bit)
//NumByteToWrite:Ҫд����ֽ���(���256),�������ܳ�����ҳʣ���ֽ���!!!	 
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
//��ȡSPI FLASH  
//��ָ����ַ��ʼ��ȡָ����������
//pBuffer:���ݴ洢��
//ReadAddr:��ʼ��ȡ��ַ(24bit)
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
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















