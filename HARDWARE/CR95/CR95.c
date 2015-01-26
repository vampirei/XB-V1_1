#include "cr95.h"

extern ReaderConfigStruct					ReaderConfig;							//CR95配置信息
__IO CR95HF_INTERFACE	CR95HF_Interface;
u8 CR95_Protocol[5]={0x00,0x02,0x02,0x01,0x01};				//26kbps(H) Respect_312us_Delay 100%_Modulation Single_Subcarrier Append_CRC


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
	/* 配置CR95管脚  NCS */		
	GPIO_InitStructure.GPIO_Pin   			= CR95HF_NCS_PIN;
	GPIO_InitStructure.GPIO_Mode  			= GPIO_Mode_Out_PP; 
	GPIO_Init(FLASH_NCS_GPIO_PORT, &GPIO_InitStructure);
	GPIO_SetBits(FLASH_NCS_GPIO_PORT, CR95HF_NCS_PIN);  
	
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

void CR95HF_SendComand(u8 Command)
{
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(5);
	SPI_SendReceiveByte(CR95HF_SPI,0x00);				//向CR95发送命令指令
	SPI_SendReceiveByte(CR95HF_SPI,Command);		//向CR95发送命令
	CR95HF_NSS_HIGH();													//CR95HF片选失效
	delay_ms(2);
}

void CR95HF_SendCommandBuff(u8 *Command)
{
	u8 i,s;
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(5);
	SPI_SendReceiveByte(CR95HF_SPI,0x00);				//向CR95发送命令指令
	for(i=0;i<Command[1];i++)
	{
		s=i+1;
		SPI_SendReceiveByte(CR95HF_SPI,Command[s]);				//向CR95发送命令
	}
	CR95HF_NSS_HIGH();													//CR95HF片选失效
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
		Prints(DisplayPort,"发送命令字0x03,返回：");
		PrintHex(DisplayPort,Temp);
		Prints(DisplayPort,"\n");
		if((Temp<0x10) && ((Temp & 0x08) == 0x08))
		{
			x=200;
			Prints(DisplayPort,"Poll正确\n");
		}
		else
			Prints(DisplayPort,"Poll不正确\n");
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
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(1);
	SPI_SendReceiveByte(CR95HF_SPI,0x02);				//发送读取指令
	*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//读取返回代码
	pResponse++;
	*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//读取返回数据长度
	for(i=0;i<pResponse[1];i++)
	{
		pResponse++;
		*pResponse = SPI_SendReceiveByte(CR95HF_SPI,0xff);			//读取返回数据
	}
	CR95HF_NSS_HIGH();													//CR95HF片选失效
}
int8_t CR95HF_PORsequence( void )
{
	u8 Temp;
	CR95HF_NSS_LOW();	
	delay_ms(1);
	Temp=SPI_SendReceiveByte(FLASH_SPI,0x00);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"发送命令字0x00,返回：");
		PrintHex(DisplayPort,Temp);
		Prints(DisplayPort,"\n");
	#endif
	Temp=SPI_SendReceiveByte(FLASH_SPI,0x55);			
	#ifdef DebugDisplay	
		Prints(DisplayPort,"发送命令字0x55,返回：");
		PrintHex(DisplayPort,Temp);
		Prints(DisplayPort,"\n");
	#endif
	CR95HF_NSS_HIGH();	
	delay_ms(10);
	Temp=CR95HF_Poll();
	if(Temp==CR95HF_POLLING_CR95HF)
	{
		#ifdef DebugDisplay
			Prints(DisplayPort,"Poll正确\n");
		#endif
		CR95HF_NSS_LOW();	
		delay_ms(1);
		Temp=SPI_SendReceiveByte(FLASH_SPI,0x02);		
		#ifdef DebugDisplay		
			Prints(DisplayPort,"发送命令字0x02,返回：");
			PrintHex(DisplayPort,Temp);
		#endif
		Temp=SPI_SendReceiveByte(FLASH_SPI,0xff);		
		#ifdef DebugDisplay		
			Prints(DisplayPort,"发送命令字0xff,返回：");
			PrintHex(DisplayPort,Temp);
		#endif
		CR95HF_NSS_HIGH();	
		delay_ms(10);
	}
	else
	{
		;
		#ifdef DebugDisplay
			Prints(DisplayPort,"Poll不正确\n");
		#endif
	}
	return Temp;
}

void CR95HF_ReadIDN(u8 *pIDN)
{
	u8 Temp;
	//-----------------读取IDN
		//发送IDN命令
		CR95HF_NSS_LOW();														//CR95HF片选有效
		delay_ms(5);
		Temp=SPI_SendReceiveByte(CR95HF_SPI,0x00);
		#ifdef DebugDisplay	
			Prints(DisplayPort,"发送命令前导 0x00   返回值:");
			PrintHex(DisplayPort,Temp);
			Prints(DisplayPort,"\n");
			delay_ms(10);
		#endif
		Temp=SPI_SendReceiveByte(CR95HF_SPI,0x01);	
		#ifdef DebugDisplay	
			Prints(DisplayPort,"发送IDN命令 0x01   返回值:");
			PrintHex(DisplayPort,Temp);
			Prints(DisplayPort,"\n");
			delay_ms(10);
		#endif
		Temp=SPI_SendReceiveByte(CR95HF_SPI,0x00);	
		#ifdef DebugDisplay	
			Prints(DisplayPort,"发送IDN命令长度 0x00   返回值:");
			PrintHex(DisplayPort,Temp);
			Prints(DisplayPort,"\n");
			delay_ms(10);
		#endif
		CR95HF_NSS_HIGH();														//CR95HF片选失效
		delay_ms(5);
		//POLL
		Temp=CR95HF_Poll();
		//读取值
		if(Temp==CR95HF_POLLING_CR95HF)
		{
			CR95HF_NSS_LOW();														//CR95HF片选有效
			delay_ms(5);
			Temp=SPI_SendReceiveByte(CR95HF_SPI,0x02);	
			#ifdef DebugDisplay	
				Prints(DisplayPort,"发送读取命令 0x02   返回值:");
				PrintHex(DisplayPort,Temp);
				Prints(DisplayPort,"\n");
			#endif
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//第一个为 RespCode
			#ifdef DebugDisplay	
				Prints(DisplayPort,"IDN:");
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//第二个为 Len  0x0F
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//第三个为 Data1	
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data2
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data3
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data4
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data5
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data6
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data7
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data8
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data9
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data10
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data11
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data12
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data13
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data14
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			pIDN++;
			*pIDN=SPI_SendReceiveByte(CR95HF_SPI,0xFF);					//Data15
			#ifdef DebugDisplay
				PrintHex(DisplayPort,*pIDN);
				Prints(DisplayPort,"\t");
				delay_ms(1);
			#endif
			CR95HF_NSS_HIGH();														//CR95HF片选失效
			delay_ms(5);
		}
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
	// CR95HF片选有效
	CR95HF_NSS_LOW();
	// 发送Reset命令
	SPI_SendReceiveByte(CR95HF_SPI, CR95HF_COMMAND_RESET);
	// CR95HF片选释放
	CR95HF_NSS_HIGH();
	delay_ms(3);
//	delayHighPriority_ms(3);
	// 发送IRQ_IN低脉冲
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

u8 CR95HF_SET_PROTOCOL_15693(u8 *pSS)
{
	u8 Tempx,Len,x;
	u8 *pResponse;
	pResponse=pSS;
	#ifdef DebugDisplay
		Prints(DisplayPort,"Set up The CR95HF's protocol to 15693\r\n");
	#endif
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(5);
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,CR95_Protocol[0]);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Control Byte 0x00  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,CR95_Protocol[1]);		
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Command Code 0x02  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,CR95_Protocol[2]);		
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Length of Data 0x02  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,CR95_Protocol[3]);		
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Protocol Code for 15693 0x01  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,CR95_Protocol[4]);		
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Protocol's parameters 0x01  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	CR95HF_NSS_HIGH();													//CR95HF片选失效
	Tempx=CR95HF_Poll();
	if(Tempx==CR95HF_POLLING_CR95HF)						//Poll正确
	{
		#ifdef DebugDisplay
			Prints(DisplayPort,"POLL正确\r\n");
			delay_ms(1);
			Prints(DisplayPort,"开始读取设置通讯协议的返回值\r\n");
			delay_ms(1);
		#endif
		CR95HF_NSS_LOW();														//CR95HF片选有效
		delay_ms(5);
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x02);
		#ifdef DebugDisplay
			Prints(DisplayPort,"Send Read Control Byte 0x02  Return--");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		*pResponse=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第1个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 1st Return Data That is RespCode --");
			PrintHex(DisplayPort,*pResponse);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		pResponse++;
		*pResponse=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第2个值  Len
		Len=*pResponse;
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 2nd Return Data That is LEN --");
			PrintHex(DisplayPort,*pResponse);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		if(Len==0x00)
		{
			#ifdef DebugDisplay
				Prints(DisplayPort,"The Response's Length is 0,There is no Data for Receive!\r\n");
				delay_ms(1);
			#endif
			;
		}		//Len==0x00
		else//Len!=0x00
		{
			#ifdef DebugDisplay
				Prints(DisplayPort,"There has ");
				PrintHex(DisplayPort,x);
				Prints(DisplayPort," Data for Receive!\r\n");
				delay_ms(1);
			#endif
			for(x=0;x<Len;x++)
			{
				pResponse++;
				*pResponse=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第3个值  第1个数据
				#ifdef DebugDisplay
					if(x==0)
						Prints(DisplayPort,"The 3rd of ResponseData is The 1st DATA --");
					else if(x==1)
						Prints(DisplayPort,"The 4th of ResponseData is The 2nd DATA --");
					else if(x==2)
						Prints(DisplayPort,"The 5th of ResponseData is The 3rd DATA --");
					else
					{
						Prints(DisplayPort,"The ");
						PrintHex(DisplayPort,x+3);
						Prints(DisplayPort,"th of ResponseData is The ");
						PrintHex(DisplayPort,x+1);
						Prints(DisplayPort,"rd DATA --");
					}
					PrintHex(DisplayPort,*pResponse);
					Prints(DisplayPort,"\r\n");
					delay_ms(1);
				#endif
			}//for(x=0;x<Len;x++)
		}//Len!=0x00
		CR95HF_NSS_HIGH();														//CR95HF片选失效
	}			//Tempx==CR95HF_POLLING_CR95HF
	else	//Tempx!=CR95HF_POLLING_CR95HF
	{
		;
		#ifdef DebugDisplay
			Prints(DisplayPort,"*****Poll Error for SET THE PROTOCOL!*****\r\n");
			delay_ms(1);
		#endif
	}			//Tempx!=CR95HF_POLLING_CR95HF
	delay_ms(2);
	pResponse=pSS;
	return *pResponse;
}

void M24XX_ReadSingleBlock(u16 Address)
{
	u8 TempAddLow,TempAddHigh,Tempx;
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(5);
	TempAddLow=Address;													//分配地址低字节
	TempAddHigh=Address>>8;											//分配地址高字节
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x00);	//发送CR95命令控制字0x00
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Control Byte 0x00  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x04);	//发送CR95命令读写操作0x04
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The SendReceive Command  0x04  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x04);	//发送读写数据长度
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The LEN  0x04  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x0A);	//发送给M24的请求标志
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Request Flag  0x0A  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x20);	//发送给M24的读取单块的命令0x20
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Command ReadSingleBlock 0x20 Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,TempAddLow);	//发送给M24的读取单块的低地址TempAddLow
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Command ReadSingleBlock ");
		PrintHex(DisplayPort,TempAddLow);
		Prints(DisplayPort," Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,TempAddHigh);	//发送给M24的读取单块的低地址TempAddHigh
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Command ReadSingleBlock ");
		PrintHex(DisplayPort,TempAddHigh);
		Prints(DisplayPort," Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	CR95HF_NSS_HIGH();														//CR95HF片选失效
	delay_ms(5);
	Tempx=CR95HF_Poll();
	if(Tempx==CR95HF_POLLING_CR95HF)						//Poll正确
	{
		#ifdef DebugDisplay
			Prints(DisplayPort,"POLL正确\r\n");
			delay_ms(1);
			Prints(DisplayPort,"开始读取M24XX的ReadSingleBlock返回值\r\n");
			delay_ms(1);
		#endif
		CR95HF_NSS_LOW();														//CR95HF片选有效
		delay_ms(5);
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x02);
		#ifdef DebugDisplay
			Prints(DisplayPort,"Send Read Control Byte 0x02  Return--");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第1个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 1st Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif			
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第2个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 2 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第3个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 3 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第4个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 4 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第5个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 5 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第6个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 6 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第7个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 7 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第8个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 8 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第9个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 9 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第10个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 10 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		CR95HF_NSS_HIGH();														//CR95HF片选失效
		delay_ms(5);
		
	}//if(Tempx==CR95HF_POLLING_CR95HF)
}

void M24XX_ReadIDN(void)
{
	u8 Tempx;
	CR95HF_NSS_LOW();														//CR95HF片选有效
	delay_ms(5);
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x00);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The Control Byte 0x00  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x04);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The SendReceive Command  0x04  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x03);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The LEN  0x03  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x26);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Request Flag  0x26  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x01);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Command Inventory 0x01 Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x00);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"Send The M24XX's Mask Length 0x00  Return--");
		PrintHex(DisplayPort,Tempx);
		Prints(DisplayPort,"\r\n");
		delay_ms(1);
	#endif
	CR95HF_NSS_HIGH();														//CR95HF片选失效
	delay_ms(5);
	Tempx=CR95HF_Poll();
	if(Tempx==CR95HF_POLLING_CR95HF)						//Poll正确
	{
		#ifdef DebugDisplay
			Prints(DisplayPort,"POLL正确\r\n");
			delay_ms(1);
			Prints(DisplayPort,"开始读取M24XX的UID返回值\r\n");
			delay_ms(1);
		#endif
		CR95HF_NSS_LOW();														//CR95HF片选有效
		delay_ms(5);
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0x02);
		#ifdef DebugDisplay
			Prints(DisplayPort,"Send Read Control Byte 0x02  Return--");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第1个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 1st Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif			
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第2个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 2 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第3个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 3 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第4个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 4 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第5个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 5 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第6个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 6 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第7个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 7 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第8个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 8 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第9个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 9 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第10个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 10 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第11个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 11 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第12个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 12 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第13个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 13 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第14个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 14 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		Tempx=SPI_SendReceiveByte(CR95HF_SPI,0xFF);		//读取第15个值  RespCode
		#ifdef DebugDisplay
			Prints(DisplayPort,"The 15 Return Data That is RespCode --");
			PrintHex(DisplayPort,Tempx);
			Prints(DisplayPort,"\r\n");
			delay_ms(1);
		#endif
		
		
		CR95HF_NSS_HIGH();														//CR95HF片选失效
		delay_ms(5);
		
	}//if(Tempx==CR95HF_POLLING_CR95HF)
}























