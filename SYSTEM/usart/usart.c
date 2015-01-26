#include "sys.h"
#include "usart.h"
#include "My_Config.h"
#include "flash.h"

//////////////////////////////////////////////////////////////////////////////////	 
////////////////////////////////////////////////////////////////////////////////// 	  

extern u8 Response[100];
extern u8 ReturnData;
extern u8 System_Status;			//ϵͳ����״̬
extern u8 ReceivedLen;				//�Ѿ��������ݳ���
extern u8 ReceivedData[200];	//���ڽ������ݻ���
extern u8 Data_Type;					//�������� :0-����	1-ָ��	2-����
extern u8 PC_Instruction;			//PC�����ָ��  0:����  1:ָ��1   2:ָ��2.������������
extern u16 PC_DataType;				//PC������������  
extern u8 Sum;								//У���
extern u8 WriteData[200];			//��ȡ�������ݴ��
u8 X=0;
//�������´���,֧��printf����,������Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 
u8 HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
//uart1�˿ڳ�ʼ��
void UART2PC_Init(u32 bound){//GPIO�˿�����
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
	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
  //USART ��ʼ������
  USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//�����ж�
  USART_Cmd(USART1, ENABLE);                    			//ʹ�ܴ��� 
}
//uart2�˿ڳ�ʼ��
void UART2CR95_Init(u32 bound){//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);								//����UART2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |												//����GPIOAʱ�Ӻ͹��ܸ���ʱ��
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
	//Usart2 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
  //USART ��ʼ������
  USART_InitStructure.USART_BaudRate = bound;							//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_2;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);					//�ر��ж�
  USART_Cmd(USART2, ENABLE);                    					//ʹ�ܴ��� 
}
//uart3�˿ڳ�ʼ��
void uart3_init(u32 bound){//GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	//�ر�USART3�����ùܽ�Ϊ��������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	USART_InitTypeDef USART_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);								//����UART3ʱ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |												//����GPIOBʱ�Ӻ͹��ܸ���ʱ��
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
//	//Usart3 NVIC ����
//	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ��ʹ��
//	NVIC_Init(&NVIC_InitStructure);													//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���USART1
//  //USART ��ʼ������
//  USART_InitStructure.USART_BaudRate = bound;							//һ������Ϊ9600;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
//	USART_Init(USART3, &USART_InitStructure);
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);					//�ر��ж�
//  USART_Cmd(USART3, ENABLE);                    					//ʹ�ܴ��� 
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
	UART_SendByte(CR95Port,0x55);								//����֡ͷ
	UART_SendByte(CR95Port,0xAA);								//����֡ͷ
	UART_SendByte(CR95Port,0x02);								//����������  2д������
	UART_SendByte(CR95Port,Data1);							//��������1
	UART_SendByte(CR95Port,Data2);							//��������2
	UART_SendByte(CR95Port,Data3);							//��������3
	UART_SendByte(CR95Port,Data4);							//��������4
	UART_SendByte(CR95Port,ADD_H);							//���͵�ַ��
	UART_SendByte(CR95Port,ADD_L);							//���͵�ַ��
}

void USART_KFB_Read(u16 Add)
{
	u8 ADD_H,ADD_L;
	ADD_H=(u8)(Add>>8);
	ADD_L=(u8)Add;
	UART_SendByte(CR95Port,0x55);								//����֡ͷ
	UART_SendByte(CR95Port,0xAA);								//����֡ͷ
	UART_SendByte(CR95Port,0x03);								//����������  3��ȡ����
	UART_SendByte(CR95Port,0x00);								//��������1
	UART_SendByte(CR95Port,0x00);								//��������2
	UART_SendByte(CR95Port,0x00);								//��������3
	UART_SendByte(CR95Port,0x00);								//��������4
	UART_SendByte(CR95Port,ADD_H);							//���͵�ַ��
	UART_SendByte(CR95Port,ADD_L);							//���͵�ַ��
}


void USART1_IRQHandler(void)                	//����1�жϷ������
{
	u8 Receive;			//���ջ�������
	u8 i;
	if(System_Status==2)				//�����ݽ���ģʽ �Ž������ݽ��չ���
	{
		if(USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET)			//�յ�����
		{
			Receive=USART_ReceiveData(USART1);				//��������
			if(ReceivedLen==0)					//���յ���һ������
			{
				if(Receive == 0x7E)				//���յ�����0x7E
				{
					ReceivedLen=1;						//���յ���һ������
					ReceivedData[0]=Receive;//�������ݴ洢��
					Sum+=Receive;															//����У���
				}
				else											//���յ��Ĳ���0x7E
				{
					ReceivedLen = 0;
					Sum=0;
				}
			}
			else if(ReceivedLen==1)					//���յ��ڶ�������
			{
				if(Receive == 0x7E)				//���յ�����0x7E
				{
					ReceivedLen=2;						//���յ���2������
					ReceivedData[1]=Receive;	//�������ݴ洢��
					Sum+=Receive;															//����У���
				}
				else											//���յ��Ĳ���0x7E
				{
					ReceivedLen = 0;
					Sum=0;
				}
			}
			else if(ReceivedLen == 2)		//���յ���3������
			{
				if(Receive==0xCD)			//���յ�����0xCD
				{
					ReceivedLen=3;			//���յ�3������
					Data_Type=1;				//���յĽ���ָ��
					ReceivedData[2]=Receive;//����
					Sum+=Receive;															//����У���
				}
				else if (Receive==0xDA)//���յ�����0xDA
				{
					ReceivedLen=3;			//���յ�3������
					Data_Type=2;				//���յĽ�������
					ReceivedData[2]=Receive;//����
					Sum+=Receive;															//����У���
				}
				else		{							//���յ�δ֪���ݣ������������
					Receive=0;
					Sum=0;
				}
			}
			else									//��������
			{
				ReceivedData[ReceivedLen++]=Receive;		//�������� ����ָ��+1
				if(Data_Type==1)				//����������ָ��
				{
					if(ReceivedLen==5)			//ָ��������
					{
						if(ReceivedData[3]==0x09 && ReceivedData[4]==0xD2)							//ָ���ж�--��ʼ��ָ��  ����ָ��1
						{
							PC_Instruction=1;
							ReceivedLen=0;
							Sum=0;							//���У���
							#ifdef DebugDisplay
								Prints(DisplayPort,"�յ�ָ��1--��ʼ��ָ�\n");
							#endif
						}
						else if(ReceivedData[3]==0x04 && ReceivedData[4]==0xCD)					//ָ���ж�--װ�����ָ��  ����ָ��2
						{
							PC_Instruction=2;
							ReceivedLen=0;
							Sum=0;							//���У���
							#ifdef DebugDisplay
								Prints(DisplayPort,"�յ�ָ��2--װ�����ָ�\n");
							#endif
						}
						else if(ReceivedData[4]==0xCA)																	//ָ���ж�--��ȡFLASHָ��ҳ����
						{
							PC_Instruction=3;
							ReceivedLen=0;
							Sum=0;							//���У���
							Flash_Read(WriteData,ReceivedData[3],200);
							Prints(DisplayPort,"��ȡFLAHS�� ");
							PrintHex(DisplayPort,ReceivedData[3]);
							Prints(DisplayPort,"ҳ����:\n");
							for(i=0;i<200;i++)
							{
								PrintHex(DisplayPort,WriteData[i]);
							}
						}
						else																						//ָ���ж�--δָ֪��
						{
							PC_Instruction=0;
							ReceivedLen=0;
							Sum=0;							//���У���
							#ifdef DebugDisplay
								Prints(DisplayPort,"�յ�����ָ��!\n");
							#endif
						}
					}
				}//����������ָ��
				else									//������������
				{
					if(ReceivedLen==200)												//���ݽ������
					{
						PC_Instruction=255;
						ReceivedLen=0;
					}
					else
					{
						Sum+=Receive;															//����У���
					}
				}//������������
			}
		}
	}//�����ݽ���ģʽ �Ž������ݽ��չ���
	else												//�������ݽ���ģʽ��ʲô������
	{
		;
	}//�������ݽ���ģʽ��ʲô������
	USART_ClearITPendingBit(USART1,USART_IT_RXNE);		//���жϱ�־
} 

void USART2_IRQHandler(void)                	//����2�жϷ������
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


void USART3_IRQHandler(void)                	//����3�жϷ������
{
	USART_ClearITPendingBit(USART3,USART_IT_RXNE);
}

























