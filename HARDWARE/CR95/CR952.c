#include "cr95.h"

extern ReaderConfigStruct					ReaderConfig;							//CR95������Ϣ
__IO CR95HF_INTERFACE	CR95HF_Interface;


int8_t InitCR95HFConfigStructure (ReaderConfigStruct *ReaderConfig)
{
	ReaderConfig->Interface	=	CR95HF_INTERFACE_SPI;
	ReaderConfig->SpiMode		=	SPI_POLLING;
	ReaderConfig->State 		= CR95HF_STATE_POWERUP;
  return 0;
}

void User_SerialInterfaceInitilisation ( void )
{
	SPI_CR95HF_Init();
}

void SPI_CR95HF_Init(void)
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
	/* ����CR95�ܽ�  NCS */		
	GPIO_InitStructure.GPIO_Pin   			= CR95HF_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_NCS_GPIO_PORT, CR95HF_NCS_PIN);  
	
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

void CR95HF_SendComand(u8 Command)
{
	CR95HF_NSS_LOW();														//CR95HFƬѡ��Ч
	delay_ms(5);
	SPI_SendReceiveByte(CR95HF_SPI,0x00);				//��CR95��������ָ��
	SPI_SendReceiveByte(CR95HF_SPI,Command);		//��CR95��������
	CR95HF_NSS_HIGH();													//CR95HFƬѡʧЧ
	delay_ms(2);
}

void CR95HF_SendCommandBuff(u8 *Command)
{
	u8 i,s;
	CR95HF_NSS_LOW();														//CR95HFƬѡ��Ч
	delay_ms(5);
	SPI_SendReceiveByte(CR95HF_SPI,0x00);				//��CR95��������ָ��
	for(i=0;i<Command[1];i++)
	{
		s=i+1;
		SPI_SendReceiveByte(CR95HF_SPI,Command[s]);				//��CR95��������
	}
	CR95HF_NSS_HIGH();													//CR95HFƬѡʧЧ
	delay_ms(2);
}

u8 CR95HF_Poll(void)
{
	u8 Temp,x;
	CR95HF_NSS_LOW();	
	delay_ms(1);
	for(x=0;x<100;x++)
	{
		Temp=SPI_SendReceiveByte(FLASH_SPI,0x03);				
		Prints(DisplayPort,"����������0x03,���أ�");
		PrintHex(DisplayPort,Temp);
		Prints(DisplayPort,"\n");
		if((Temp<0x10) && ((Temp & 0x08) == 0x08))
		{
			x=200;
			Prints(DisplayPort,"Poll��ȷ\n");
		}
		else
			Prints(DisplayPort,"Poll����ȷ\n");
	}
	CR95HF_NSS_HIGH();	
	delay_ms(10);
	if(x>150)
		return CR95HF_POLLING_CR95HF;
	else
		return CR95HF_POLLING_TIMEOUT;
}

void CR95HF_ReadData(u8 *pResponse)
{
	u8 i;
	CR95HF_NSS_LOW();														//CR95HFƬѡ��Ч
	delay_ms(1);
	SPI_SendReceiveByte(CR95HF_SPI,0x02);				//���Ͷ�ȡָ��
	*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//��ȡ���ش���
	pResponse++;
	*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//��ȡ�������ݳ���
	for(i=0;i<pResponse[1];i++)
	{
		pResponse++;
		*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//��ȡ��������
	}
	CR95HF_NSS_HIGH();													//CR95HFƬѡʧЧ
}
int8_t CR95HF_PORsequence( void )
{
	u8 Temp,x;
	CR95HF_NSS_LOW();	
	delay_ms(1);
	Temp=SPI_SendReceiveByte(FLASH_SPI,0x00);				
	Prints(DisplayPort,"����������0x00,���أ�");
	PrintHex(DisplayPort,Temp);
	Prints(DisplayPort,"\n");
	Temp=SPI_SendReceiveByte(FLASH_SPI,0x55);				
	Prints(DisplayPort,"����������0x55,���أ�");
	PrintHex(DisplayPort,Temp);
	Prints(DisplayPort,"\n");
	CR95HF_NSS_HIGH();	
	delay_ms(10);
	
	Temp=CR95HF_Poll();
	if(Temp==CR95HF_POLLING_CR95HF)
	{
		Prints(DisplayPort,"Poll��ȷ\n");
		CR95HF_NSS_LOW();	
		delay_ms(1);
		Temp=SPI_SendReceiveByte(FLASH_SPI,0x02);				
		Prints(DisplayPort,"����������0x02,���أ�");
		PrintHex(DisplayPort,Temp);
		Temp=SPI_SendReceiveByte(FLASH_SPI,0xff);				
		Prints(DisplayPort,"����������0xff,���أ�");
		PrintHex(DisplayPort,Temp);
		CR95HF_NSS_HIGH();	
		delay_ms(10);
	}
	else
		Prints(DisplayPort,"Poll����ȷ\n");
//	CR95HF_NSS_LOW();	
//	delay_ms(1);
//	for(x=0;x<100;x++)
//	{
//		Temp=SPI_SendReceiveByte(FLASH_SPI,0x03);				
//		Prints(DisplayPort,"����������0x03,���أ�");
//		PrintHex(DisplayPort,Temp);
//		Prints(DisplayPort,"\n");
//		if((Temp<0x10) && ((Temp & 0x08) == 0x08))
//		{
//			x=100;
//			Prints(DisplayPort,"Poll��ȷ\n");
//		}
//		else
//			Prints(DisplayPort,"Poll����ȷ\n");
//	}
//	CR95HF_NSS_HIGH();	
//	delay_ms(10);
	
//	CR95HF_NSS_LOW();	
//	delay_ms(1);
//	Temp=SPI_SendReceiveByte(FLASH_SPI,0x02);				
//	Prints(DisplayPort,"����������0x02,���أ�");
//	PrintHex(DisplayPort,Temp);
//	Temp=SPI_SendReceiveByte(FLASH_SPI,0xff);				
//	Prints(DisplayPort,"����������0xff,���أ�");
//	PrintHex(DisplayPort,Temp);
//	CR95HF_NSS_HIGH();	
//	delay_ms(10);
	return Temp;
}

void CR95HF_Send_IRQIN_NegativePulse(void)
{
	CR95HF_IRQIN_HIGH();
	delay_ms(1);
//	delayHighPriority_ms(1);
	CR95HF_IRQIN_LOW();
	delay_ms(1);
//	delayHighPriority_ms(1);
	CR95HF_IRQIN_HIGH();
}

int8_t CR95HF_Echo(uint8_t *pResponse)
{
	while(SPI_I2S_GetFlagStatus(CR95HF_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(CR95HF_SPI, ECHO);	
	while(SPI_I2S_GetFlagStatus(CR95HF_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	pResponse[0] = SPI_I2S_ReceiveData(CR95HF_SPI);
	return CR95HF_SUCCESS_CODE;

}

int8_t SpyTransaction(uc8 *pCommand, uint8_t *pResponse)
{
	switch (pCommand[CR95HF_COMMAND_OFFSET])
	{
		case PROTOCOL_SELECT: 
			if (CR95HF_IsReaderResultCodeOk (pCommand[CR95HF_COMMAND_OFFSET],pResponse) == TRUE)
				ReaderConfig.CurrentProtocol=pCommand[CR95HF_DATA_OFFSET];
			return CR95HF_SUCCESS_CODE;	
		default: 
			return CR95HF_SUCCESS_CODE;
	
	}
}

void CR95HF_Send_SPI_ResetSequence(void)
{
	// CR95HFƬѡ��Ч
	CR95HF_NSS_LOW();
	// ����Reset����
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_RESET);
	// CR95HFƬѡ�ͷ�
	CR95HF_NSS_HIGH();
	delay_ms(3);
//	delayHighPriority_ms(3);
	// ����IRQ_IN������
	CR95HF_Send_IRQIN_NegativePulse();
	delay_ms(10);
//	delayHighPriority_ms(10);
}

int8_t CR95HF_IsReaderResultCodeOk (uint8_t CmdCode,uc8 *ReaderReply)
{

   CmdCode = CmdCode & 0xFF;

   if (ReaderReply[READERREPLY_STATUSOFFSET] == CR95HF_ERRORCODE_DEFAULT)
		return CR95HF_NOREPLY_CODE;

  	switch (CmdCode)
	{
		case ECHO: 
			if (ReaderReply[PSEUDOREPLY_OFFSET] == ECHO)
				return CR95HF_SUCCESS_CODE;
			else 
				return CR95HF_ERROR_CODE;
		case IDN: 
			if (ReaderReply[READERREPLY_STATUSOFFSET] == IDN_RESULTSCODE_OK)
				return CR95HF_SUCCESS_CODE;
			else 
				return CR95HF_ERROR_CODE;
		case PROTOCOL_SELECT: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDN_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case PROTOCOLSELECT_ERRORCODE_CMDLENGTH :
					return CR95HF_ERROR_CODE;
				case PROTOCOLSELECT_ERRORCODE_INVALID :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case SEND_RECEIVE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case SENDRECV_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case SENDRECV_ERRORCODE_COMERROR :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_FRAMEWAIT :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_SOF :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_OVERFLOW :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_FRAMING :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_EGT :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_CRC :
					return CR95HF_ERROR_CODE;
				case SENDRECV_ERRORCODE_RECEPTIONLOST :
					return CR95HF_ERROR_CODE;
				default :
					return CR95HF_ERROR_CODE;
			}			
		case IDLE: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case IDLE_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case IDLE_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case READ_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case READREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				case READREG_ERRORCODE_LENGTH :
					return CR95HF_ERROR_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case WRITE_REGISTER: 
			switch (ReaderReply[READERREPLY_STATUSOFFSET])
			{
				case WRITEREG_RESULTSCODE_OK :
					return CR95HF_SUCCESS_CODE;
				default : return CR95HF_ERROR_CODE;
			}
		case BAUD_RATE: 
			return CR95HF_ERROR_CODE;
		default: 
			return ERRORCODE_GENERIC;
	}
}



