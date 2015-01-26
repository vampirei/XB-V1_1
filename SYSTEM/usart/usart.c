#include "sys.h"
#include "usart.h"
#include "My_Config.h"
#include "flash.h"

//////////////////////////////////////////////////////////////////////////////////	 
////////////////////////////////////////////////////////////////////////////////// 	  

extern u8 Response[100];
extern u8 ReturnData;
extern u8 System_Status;			//系统工作状态
extern u8 ReceivedLen;				//已经接收数据长度
extern u8 ReceivedData[200];	//串口接收数据缓存
extern u8 Data_Type;					//接收类型 :0-保留	1-指令	2-数据
extern u8 PC_Instruction;			//PC传输的指令  0:保留  1:指令1   2:指令2.。。。。。。
extern u16 PC_DataType;				//PC传输数据类型  
extern u8 Sum;								//校验和
extern u8 WriteData[200];			//读取出的数据存放
u8 X=0;
//加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
u8 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//uart1端口初始化
void UART2PC_Init(u32 bound){//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);
  //USART1_TX   PA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART1_RX	  PA.10
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  //USART 初始化设置
  USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启中断
  USART_Cmd(USART1, ENABLE);                    			//使能串口 
}
//uart2端口初始化
void UART2CR95_Init(u32 bound){//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);								//允许UART2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |												//允许GPIOA时钟和功能复用时钟
												 RCC_APB2Periph_AFIO	, ENABLE);
  //USART2_TX   PA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  //USART2_RX	  PA.3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
  //USART 初始化设置
  USART_InitStructure.USART_BaudRate = bound;							//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);					//关闭中断
  USART_Cmd(USART2, ENABLE);                    					//使能串口 
}
//uart3端口初始化
void uart3_init(u32 bound){//GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	//关闭USART3，设置管脚为悬浮输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);								//允许UART3时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |												//允许GPIOB时钟和功能复用时钟
//												 RCC_APB2Periph_AFIO	, ENABLE);
//  //USART3_TX   PB.10
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  //USART3_RX	  PB.11
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);  
//	//Usart3 NVIC 配置
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQ通道使能
//	NVIC_Init(&NVIC_InitStructure);													//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器USART1
//  //USART 初始化设置
//  USART_InitStructure.USART_BaudRate = bound;							//一般设置为9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART3, &USART_InitStructure);
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);					//关闭中断
//  USART_Cmd(USART3, ENABLE);                    					//使能串口 
}

void UART_SendByte(USART_TypeDef* USARTx, uint8_t data) 
{	
	/* Wait for USART Tx buffer empty */
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);

	/* Send byte through the UART peripheral */
	USART_SendData(USARTx, data);	
}

uint8_t UART_ReceiveByte( USART_TypeDef* USARTx ) 
{	
	StartTimeOut(300);
	/* Wait for UART data reception	*/
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET && TimeUp != TRUE);
	StopTimeOut( );
	/* Read & return UART received data	*/
	return USART_ReceiveData(USARTx);
}

void UART_SendBuffer(USART_TypeDef* USARTx, uc8 *pCommand, uint8_t length) 
{
	uint8_t i;

	for(i=0; i<length; i++)
	{
		UART_SendByte(USARTx, pCommand[i]);
	}
}

void UART_ReceiveBuffer(USART_TypeDef* USARTx, uint8_t *pResponse, uint8_t length) 
{
	uint8_t i;

	for(i=0; i<length; i++)
	{
		pResponse[i] = UART_ReceiveByte(USARTx);
	}
}

void Prints(USART_TypeDef* USARTx,u8 *pd)
{
	while((*pd)!='\0')
	{
		UART_SendByte(USARTx,*pd);
		pd++;
	}
}

void PrintShortIntHex(USART_TypeDef* USARTx,u16 x)
{
	u8 i;
	u8 display_buffer[7];
	display_buffer[6]=0;
	display_buffer[0]=0;
	display_buffer[1]='x';
	for(i=5;i>=2;i--)
	{
		display_buffer[i]=HexTable[(x & 0xFF)];
		x >>= 4;
	}
	Prints(USARTx,display_buffer);
}

void PrintLongInt(USART_TypeDef* USARTx,u32 x)
{
	int i;
	u8 display_buffer[10];
	for(i=9;i>=0;i--)
	{
		display_buffer[i]='0' + x % 10;
		x /= 10;
	}
	for(i=0;i<9;i++)
	{
		if(display_buffer[i]!='0')
			break;
	}
	for(;i<10;i++)
	{
		UART_SendByte(USARTx,display_buffer[i]);
	}
}

void PrintHex(USART_TypeDef* USARTx,u8 x)
{
	UART_SendByte(USARTx,'0');
	UART_SendByte(USARTx,'x');
	UART_SendByte(USARTx,HexTable[x>>4]);
	UART_SendByte(USARTx,HexTable[x&0xF]);
	UART_SendByte(USARTx,' ');
}

void USART_KFB_Write(u16 Add,u8 Data1,u8 Data2, u8 Data3,u8 Data4)
{
	u8 ADD_H,ADD_L;
	ADD_H=(u8)(Add>>8);
	ADD_L=(u8)Add;
	UART_SendByte(CR95Port,0x55);								//发送帧头
	UART_SendByte(CR95Port,0xAA);								//发送帧头
	UART_SendByte(CR95Port,0x02);								//发送命令字  2写入数据
	UART_SendByte(CR95Port,Data1);							//发送数据1
	UART_SendByte(CR95Port,Data2);							//发送数据2
	UART_SendByte(CR95Port,Data3);							//发送数据3
	UART_SendByte(CR95Port,Data4);							//发送数据4
	UART_SendByte(CR95Port,ADD_H);							//发送地址高
	UART_SendByte(CR95Port,ADD_L);							//发送地址低
}

void USART_KFB_Read(u16 Add)
{
	u8 ADD_H,ADD_L;
	ADD_H=(u8)(Add>>8);
	ADD_L=(u8)Add;
	UART_SendByte(CR95Port,0x55);								//发送帧头
	UART_SendByte(CR95Port,0xAA);								//发送帧头
	UART_SendByte(CR95Port,0x03);								//发送命令字  3读取数据
	UART_SendByte(CR95Port,0x00);								//发送数据1
	UART_SendByte(CR95Port,0x00);								//发送数据2
	UART_SendByte(CR95Port,0x00);								//发送数据3
	UART_SendByte(CR95Port,0x00);								//发送数据4
	UART_SendByte(CR95Port,ADD_H);							//发送地址高
	UART_SendByte(CR95Port,ADD_L);							//发送地址低
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 Receive;			//接收缓存数据
	u8 i;
	if(System_Status==2)				//在数据接收模式 才进行数据接收工作
	{
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)			//收到数据
		{
			Receive=USART_ReceiveData(USART1);				//缓存数据
			if(ReceivedLen==0)					//接收到第一个数据
			{
				if(Receive == 0x7E)				//接收到的是0x7E
				{
					ReceivedLen=1;						//接收到第一个数据
					ReceivedData[0]=Receive;//放入数据存储区
					Sum+=Receive;															//计算校验和
				}
				else											//接收到的不是0x7E
				{
					ReceivedLen = 0;
					Sum=0;
				}
			}
			else if(ReceivedLen==1)					//接收到第二个数据
			{
				if(Receive == 0x7E)				//接收到的是0x7E
				{
					ReceivedLen=2;						//接收到第2个数据
					ReceivedData[1]=Receive;	//放入数据存储区
					Sum+=Receive;															//计算校验和
				}
				else											//接收到的不是0x7E
				{
					ReceivedLen = 0;
					Sum=0;
				}
			}
			else if(ReceivedLen == 2)		//接收到第3个数据
			{
				if(Receive==0xCD)			//接收到的是0xCD
				{
					ReceivedLen=3;			//接收到3个数据
					Data_Type=1;				//接收的将是指令
					ReceivedData[2]=Receive;//缓存
					Sum+=Receive;															//计算校验和
				}
				else if (Receive==0xDA)//接收到的是0xDA
				{
					ReceivedLen=3;			//接收到3个数据
					Data_Type=2;				//接收的将是数据
					ReceivedData[2]=Receive;//缓存
					Sum+=Receive;															//计算校验和
				}
				else		{							//接收到未知数据，清零接收数据
					Receive=0;
					Sum=0;
				}
			}
			else									//其余数据
			{
				ReceivedData[ReceivedLen++]=Receive;		//缓存数据 并将指针+1
				if(Data_Type==1)				//接下来的是指令
				{
					if(ReceivedLen==5)			//指令接收完成
					{
						if(ReceivedData[3]==0x09 && ReceivedData[4]==0xD2)							//指令判断--初始化指令  返回指令1
						{
							PC_Instruction=1;
							ReceivedLen=0;
							Sum=0;							//清除校验和
							#ifdef DebugDisplay
								Prints(DisplayPort,"收到指令1--初始化指令！\n");
							#endif
						}
						else if(ReceivedData[3]==0x04 && ReceivedData[4]==0xCD)					//指令判断--装订完成指令  返回指令2
						{
							PC_Instruction=2;
							ReceivedLen=0;
							Sum=0;							//清除校验和
							#ifdef DebugDisplay
								Prints(DisplayPort,"收到指令2--装订完成指令！\n");
							#endif
						}
						else if(ReceivedData[4]==0xCA)																	//指令判断--读取FLASH指定页数据
						{
							PC_Instruction=3;
							ReceivedLen=0;
							Sum=0;							//清除校验和
							Flash_Read(WriteData,ReceivedData[3],200);
							Prints(DisplayPort,"读取FLAHS第 ");
							PrintHex(DisplayPort,ReceivedData[3]);
							Prints(DisplayPort,"页数据:\n");
							for(i=0;i<200;i++)
							{
								PrintHex(DisplayPort,WriteData[i]);
							}
						}
						else																						//指令判断--未知指令
						{
							PC_Instruction=0;
							ReceivedLen=0;
							Sum=0;							//清除校验和
							#ifdef DebugDisplay
								Prints(DisplayPort,"收到错误指令!\n");
							#endif
						}
					}
				}//接下来的是指令
				else									//接下来是数据
				{
					if(ReceivedLen==200)												//数据接收完成
					{
						PC_Instruction=255;
						ReceivedLen=0;
					}
					else
					{
						Sum+=Receive;															//计算校验和
					}
				}//接下来是数据
			}
		}
	}//在数据接收模式 才进行数据接收工作
	else												//不在数据接收模式，什么都不干
	{
		;
	}//不在数据接收模式，什么都不干
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//清中断标志
} 

void USART2_IRQHandler(void)                	//串口2中断服务程序
{
	u8 Receive;
	Receive = USART_ReceiveData(USART2);
	if(X == 0)
	{
		if(Receive == 0x55)
		{
			X=1;
			Response[0] = 0x55;
		}
		else
			X=0;
	}
	else if(X == 1)
	{
		if(Receive == 0xaa)
		{
			X=2;
			Response[1] = 0xaa;
		}
		else if(Receive == 0x55)
			X=1;
		else
			X=0;
	}
	else if(X == 2)
	{
		Response[2] = Receive;
		X=3;
	}
	else if(X == 3)
	{
		Response[3] = Receive;
		X=4;
	}
	else if(X == 4)
	{
		Response[4] = Receive;
		X=5;
	}
	else if(X == 5)
	{
		Response[5] = Receive;
		X=0;
		ReturnData = 1;
	}
	USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}


void USART3_IRQHandler(void)                	//串口3中断服务程序
{
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}

























