
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "My_Config.h"
#include "flash.h"
#include "lcd.h"
#include "key.h"

/***************************************************************/
/*"��",0*/
unsigned char Ru[]=	{0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
									 0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00};
/*"��",0*/
unsigned char Jin[]={0x40,0x40,0x42,0xCC,0x00,0x80,0x88,0x88,0xFF,0x88,0x88,0xFF,0x88,0x88,0x80,0x00,
									 0x00,0x40,0x20,0x1F,0x20,0x40,0x50,0x4C,0x43,0x40,0x40,0x5F,0x40,0x40,0x40,0x00};
/*"ϵ",0*/
unsigned char Xi[]={0x00,0x00,0x22,0x32,0x2A,0xA6,0xA2,0x62,0x21,0x11,0x09,0x81,0x01,0x00,0x00,0x00,
                  0x00,0x42,0x22,0x13,0x0B,0x42,0x82,0x7E,0x02,0x02,0x0A,0x12,0x23,0x46,0x00,0x00};
/*"ͳ",1*/
unsigned char Tong[]={0x20,0x30,0xAC,0x63,0x30,0x00,0x88,0xC8,0xA8,0x99,0x8E,0x88,0xA8,0xC8,0x88,0x00,
									  0x22,0x67,0x22,0x12,0x12,0x80,0x40,0x30,0x0F,0x00,0x00,0x3F,0x40,0x40,0x71,0x00};
/*"ģ"*/
unsigned char Mo[]={0x10,0x10,0xD0,0xFF,0x90,0x14,0xE4,0xAF,0xA4,0xA4,0xA4,0xAF,0xE4,0x04,0x00,0x00,
										0x04,0x03,0x00,0xFF,0x00,0x89,0x4B,0x2A,0x1A,0x0E,0x1A,0x2A,0x4B,0x88,0x80,0x00};
/*"ʽ"*/
unsigned char Shi[]={0x10,0x10,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0xFF,0x10,0x10,0x11,0x16,0x10,0x00,
										 0x00,0x20,0x60,0x20,0x3F,0x10,0x10,0x10,0x00,0x03,0x0C,0x10,0x20,0x40,0xF8,0x00};
/*"��"*/
unsigned char Jie[]={0x10,0x10,0x10,0xFF,0x10,0x50,0x44,0x54,0x65,0xC6,0x44,0x64,0x54,0x44,0x40,0x00,
										 0x04,0x44,0x82,0x7F,0x01,0x82,0x82,0x4A,0x56,0x23,0x22,0x52,0x4E,0x82,0x02,0x00};
/*"��"*/
unsigned char Shou[]={0x00,0xFC,0x00,0x00,0xFF,0x00,0x40,0x20,0xD8,0x17,0x10,0x10,0xF0,0x10,0x10,0x00,
										  0x00,0x0F,0x04,0x02,0xFF,0x00,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x00};
/*"��"*/					
unsigned char Fa[]={0x00,0x00,0x18,0x16,0x10,0xD0,0xB8,0x97,0x90,0x90,0x90,0x92,0x94,0x10,0x00,0x00,
						        0x00,0x20,0x10,0x8C,0x83,0x80,0x41,0x46,0x28,0x10,0x28,0x44,0x43,0x80,0x80,0x00};
/*"��"*/
unsigned char Song[]={0x40,0x40,0x42,0xCC,0x00,0x88,0x89,0x8E,0x88,0xF8,0x88,0x8C,0x8B,0x88,0x80,0x00,
										  0x00,0x40,0x20,0x1F,0x20,0x40,0x50,0x48,0x46,0x41,0x42,0x44,0x58,0x40,0x40,0x00};		
/*"��"*/
unsigned char Di[]={0x08,0x04,0x93,0x92,0x96,0x9A,0x92,0xFA,0x94,0x93,0x92,0x96,0xFA,0x02,0x02,0x00,
										0x40,0x40,0x47,0x24,0x24,0x14,0x0C,0xFF,0x04,0x04,0x24,0x44,0x24,0x1C,0x00,0x00};
/*"һ"*/
unsigned char Yi[]={0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
										0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*"��"*/
unsigned char Er[]={0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
										0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00};
/*"��"*/
unsigned char San[]={0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
										 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00};
/*"��"*/
unsigned char Si[]={0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
										0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00};	
/*"��"*/
unsigned char Lei[]={0x00,0x10,0x10,0x92,0x54,0x30,0x10,0xFF,0x10,0x30,0x54,0x92,0x10,0x10,0x00,0x00,
										 0x84,0x85,0x45,0x44,0x24,0x14,0x0C,0x06,0x0C,0x14,0x24,0x44,0x45,0x84,0x84,0x00};		
/*"��"*/
unsigned char Shu[]={0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00,
										 0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00};
/*"��"*/
unsigned char Ju[]={0x10,0x10,0xFF,0x10,0x90,0x00,0xFE,0x92,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00,
										0x42,0x82,0x7F,0x01,0x80,0x60,0x1F,0x00,0xFC,0x44,0x47,0x44,0x44,0xFC,0x00,0x00};
/*"��"*/
unsigned char Zhong[]={0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,
											 0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00};
/*" "*/
unsigned char Empty[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//���һ������λ��
/*"���������"*/
unsigned char EmptyStart[]={0x40,0xC0,0x40,0x40,0x40,0x60,0x1C,0x03,0x1C,0x60,0x40,0x40,0x40,0xC0,0x40,0x00,
														0x00,0x00,0x41,0x39,0x26,0x10,0x10,0x08,0x08,0x10,0x26,0x39,0x41,0x00,0x00,0x00};	
/*"ʵ�������"*/
unsigned char FullStart[]={0x40,0xC0,0xC0,0xC0,0xC0,0xE0,0xFC,0xFF,0xFC,0xE0,0xC0,0xC0,0xC0,0xC0,0x40,0x00,
													 0x00,0x00,0x41,0x39,0x3F,0x1F,0x1F,0x0F,0x1F,0x1F,0x3F,0x39,0x41,0x00,0x00,0x00};
/*"������"*/
unsigned char Triangle[]={0,0,0,0,0,0,254,252,248,240,224,192,128,0,0,0,0,0,0,0,0,0,63,31,15,7,3,1,0,0,0,0};
/*"��"*/
unsigned char Zi[]={0x00,0x00,0x00,0xF8,0x88,0x8C,0x8A,0x89,0x88,0x88,0x88,0xF8,0x00,0x00,0x00,0x00,
										0x00,0x00,0x00,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00,0x00};
/*"��"*/
unsigned char Jian[]={0x10,0x10,0xD0,0xFF,0x90,0x50,0x20,0x50,0x4C,0x43,0x4C,0x50,0x20,0x40,0x40,0x00,
											0x04,0x03,0x00,0xFF,0x00,0x41,0x44,0x58,0x41,0x4E,0x60,0x58,0x47,0x40,0x40,0x00};
/*"��"*/
unsigned char Cun[]={0x04,0x04,0x84,0xE4,0x1C,0x07,0x24,0x24,0x24,0x24,0xA4,0x64,0x24,0x04,0x04,0x00,
										 0x02,0x01,0x00,0xFF,0x00,0x02,0x02,0x42,0x82,0x7F,0x02,0x02,0x02,0x02,0x02,0x00};
/*"��"*/
unsigned char Chu[]={0x80,0x60,0xF8,0x87,0x84,0x98,0x00,0x20,0x24,0xA4,0x7F,0x24,0x34,0x28,0x24,0x00,
										 0x00,0x00,0xFF,0x00,0x00,0x7F,0x24,0x12,0x01,0xFF,0x49,0x49,0x49,0xFF,0x00,0x00};
/*"��"*/
unsigned char Qi[]={0x80,0x80,0x9E,0x92,0x92,0x92,0x9E,0xE0,0x80,0x9E,0xB2,0xD2,0x92,0x9E,0x80,0x00,
										0x08,0x08,0xF4,0x94,0x92,0x92,0xF1,0x00,0x01,0xF2,0x92,0x94,0x94,0xF8,0x08,0x00};
/*"��"*/
unsigned char Zheng[]={0x00,0x02,0x02,0xC2,0x02,0x02,0x02,0xFE,0x82,0x82,0x82,0x82,0x82,0x02,0x00,0x00,
											 0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00};
/*"��"*/
unsigned char Chang[]={0x20,0x18,0x08,0xEA,0xAC,0xA8,0xA8,0xAF,0xA8,0xA8,0xAC,0xEA,0x08,0x28,0x18,0x00,
											 0x00,0x00,0x3E,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x12,0x22,0x1E,0x00,0x00,0x00};
/*"��"*/
unsigned char Yi4[]={0x00,0x00,0x7E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0xE0,0x00,0x00,
										 0x08,0x88,0x48,0x28,0x1E,0x08,0x08,0x08,0x08,0x08,0xFE,0x08,0x08,0x08,0x08,0x00};
/*"��"*/
unsigned char Dao[]={0x42,0x62,0x52,0x4A,0xC6,0x42,0x52,0x62,0xC2,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
										 0x40,0xC4,0x44,0x44,0x7F,0x24,0x24,0x24,0x20,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00};
/*"��"*/
unsigned char Chu1[]={0x08,0x08,0x89,0xEA,0x18,0x88,0x00,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
                     0x02,0x01,0x00,0xFF,0x01,0x86,0x40,0x20,0x18,0x07,0x40,0x80,0x40,0x3F,0x00,0x00};
/*"ʼ"*/
unsigned char Shi3[]={0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
                     0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00};
/*"��"*/
unsigned char Hua[]={0x00,0x80,0x60,0xF8,0x07,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x04,0x00,0x00,
                     0x01,0x00,0x00,0xFF,0x00,0x04,0x02,0x01,0x3F,0x40,0x40,0x40,0x40,0x40,0x78,0x00};
/*"ָ"*/
unsigned char Zhi[]={0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x3F,0x48,0x48,0x44,0x44,0x44,0x42,0x70,0x00,
                     0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00};
/*"��"*/
unsigned char Ling[]={0x80,0x80,0x40,0x20,0x10,0x08,0x14,0x63,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x00,
                      0x00,0x00,0x00,0x01,0x01,0x09,0x11,0x21,0xD1,0x09,0x05,0x03,0x00,0x00,0x00,0x00};
/*"װ"*/
unsigned char Zhuang[]={0x40,0x42,0x24,0x10,0xFF,0x00,0x84,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x04,0x00,
                        0x22,0x22,0x12,0x12,0x0A,0xFE,0x42,0x27,0x0A,0x12,0x22,0x32,0x4A,0x42,0x42,0x00};
/*"��"*/
unsigned char Ding[]={0x40,0x40,0x42,0xCC,0x00,0x00,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,
                      0x00,0x00,0x00,0x7F,0x20,0x10,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00};
/*"��"*/
unsigned char Wan[]={0x10,0x0C,0x04,0x24,0x24,0x24,0x25,0x26,0x24,0x24,0x24,0x24,0x04,0x14,0x0C,0x00,
                     0x00,0x81,0x81,0x41,0x31,0x0F,0x01,0x01,0x01,0x7F,0x81,0x81,0x81,0xF1,0x00,0x00};
/*"��"*/
unsigned char Cheng[]={0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0xFF,0x08,0x09,0x0A,0xC8,0x08,0x00,
                       0x80,0x60,0x1F,0x00,0x10,0x20,0x1F,0x80,0x40,0x21,0x16,0x18,0x26,0x41,0xF8,0x00};
/***************************************************************/





//�����ʽ��0x55 0xaa Command Data1 Data2 Data3 Data4 Address1 Address2

bool 		TimeUp;
u8 Tempx,x,Len,y;
ReaderConfigStruct					ReaderConfig;							//CR95������Ϣ
uint8_t											CR95HFhandled;						//
u8 Response[100];
u8 ReturnData;
u8 *pResponse=Response;
u8 ResCR95HF_IDN[17];
u16 TempAdd;
u8 TempAddLow,TempAddHigh;
u8 TempM24Data_1,TempM24Data_2,TempM24Data_3,TempM24Data_4,TempM24Data=0;
u8 Send_Select;																				//���͵���������ѡ��
u8 System_Status;		/*ϵͳ����״̬��0��ϵͳ�Լ�
																		1��ѡ����ģʽ
																		2�����ݽ���ģʽ
																		3�����ݷ���ѡ��ģʽ
																		4�����ݿ�ʼ����
										ϵͳ����״̬*/
u8 Position;				//���λ��   1:��һ�У�2���ڶ��У�3�������У�4��������
u8 Key_Push;				//����ֵ��
u8 ReceivedLen;				//�Ѿ��������ݳ���
u8 ReceivedData[200];	//���ڽ������ݻ���
u8 WriteData[200];		//д��MR24������
u8 PC_Instruction;		//PC�����ָ��  0:����  1:ָ��1   2:ָ��2.������������255:���ݽ������
u8 Data_Type;
u16 PC_DataType;				//PC������������   ��λ�������ǵڼ���  ʮλ+��λ���������ڵڼ�������
                        //    101����һ�� ��1��     104����һ�� ��4��  407�������� ��7��
u8 Sum;								//����PC����У���
u32 Type1_Len=0,Type2_Len=0,Type3_Len=0,Type4_Len=0;// ��һ���������ܳ�
u16 Type1_Num=0,Type2_Num=0,Type3_Num=0,Type4_Num=0;//���������ݽ��յ������һ�����
u16 Number=0;						//��ǰ�����
u8 Send_Type=0x0F;			//�����������ͣ�   bit0����һ�����ݷ���Ϊ1��������Ϊ0
												//								 bit1���ڶ������ݷ���Ϊ1��������Ϊ0
												//								 bit2�����������ݷ���Ϊ1��������Ϊ0
												//								 bit3�����������ݷ���Ϊ1��������Ϊ0
												//								 bit4--bit7:����
u8 WriteFrameNum;
u8 Retry = 0 ;	
u16 WriteNum;						//����д����� 

int main(void)
{
	u32 Temp_DataLen;				//��ȡ���������ݳ��ȵ���ʱ����
	Sum = 0;
	
	
	ReturnData = 0;
	Key_Push = 0;
	SystemInit();  				//ϵͳʱ������
	delay_init(72);	     	//��ʱ��ʼ��
	NVIC_Configuration();	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	UART2PC_Init(115200);															//��ʼ��PCͨѶ�˿�  ������115200
	UART2CR95_Init(230400);														//��ʼ���뿪����ͨѶ�˿� ������230400
	SPI_Two_Init();																		//����SPI��ʼ��
	KEY_Init();																				//��ʼ������
	initial_lcd();																		//LCD��ʼ��
	clear_screen();																		//LCD����
	display_graphic_16x16_page(1,Xi,Tong,Zi,Jian,Zhong,Empty,Empty,Empty);//��ʾ"ϵͳ�Լ���"
	delay_ms(10);
	System_Status=0;																	//ϵͳ����״̬Ϊ�Լ�
	//�Լ�Flash
	FLASH_Read_ID(Response);													//��ȡFlash��ID
	Prints(DisplayPort,"Flash's ID is :");
	PrintHex(DisplayPort,Response[0]);
	Prints(DisplayPort,"  ");
	PrintHex(DisplayPort,Response[1]);
	if(Response[0]==0xEF && Response[1]==0x16){				//�ж�Flash��ID�Ƿ�����
		#ifdef DebugDisplay
			Prints(DisplayPort,"��⵽FLASH��W25Q64  \r\n");
		#endif
		display_graphic_16x16_page(2,Empty,Cun,Chu,Qi,Zheng,Chang,Empty,Empty);
	}
	else{
		#ifdef	DebugDisplay
			Prints(DisplayPort,"û�м�⵽FLASH  \r\n");
		#endif
		display_graphic_16x16_page(2,Empty,Cun,Chu,Qi,Yi4,Chang,Empty,Empty);
	}

	
	
	
	
	//�Լ�CR95HF  �Լ���ɺ�û�м�⵽CR95�����˳������˳���⡣
	for(Tempx=0;Tempx<50;Tempx++)		{					//���ж��CR95��⣬���û�м�⵽���ظ�50�Σ������ܼ�⵽������
		UART_SendByte(CR95Port,0x55);						//����CR95����֡ ֡ͷ 0x55
		UART_SendByte(CR95Port,0xaa);						//����CR95����֡ ֡ͷ 0xaa
		UART_SendByte(CR95Port,0x01);						//����CR95����֡ ��Ⲣ����ͨѶЭ��ΪISO15963 0x01
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ����1������������Ч��
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ����2������������Ч��
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ����3������������Ч��
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ����4������������Ч��
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ��ַ1������������Ч��
		UART_SendByte(CR95Port,0xFF);						//����CR95����֡ ��ַ2������������Ч��
		delay_ms(500);													//��ʱ500ms
		if(ReturnData==1)	{				//�з�������
			ReturnData=0;
			#ifdef DebugDisplay
				Prints(DisplayPort,"CR95-����1��������Ϊ��");
				for(x=0;x<6;x++){
					PrintHex(DisplayPort,Response[x]);
					Prints(DisplayPort,"  ");
				}
				Prints(DisplayPort,"\n");
			#endif
			if(Response[2]=='N' && Response[3]=='F' && Response[4]=='C' && Response[5]==' ')
				Tempx=60;
		}
	}
	if(Tempx>55)		{
		display_graphic_16x16_page(3,Empty,Shu,Ju,Zheng,Chang,Empty,Empty,Empty);
		display_graphic_16x16_page(4,Xi,Tong,Zi,Jian,Zheng,Chang,Empty,Empty);
		#ifdef DebugDisplay
			Prints(DisplayPort,"CR95���������\n");
			Prints(DisplayPort,"ϵͳ�Լ���ɣ�\n");
		#endif
	}
	else		{
		display_graphic_16x16_page(3,Empty,Shu,Ju,Yi4,Chang,Empty,Empty,Empty);
		display_graphic_16x16_page(4,Xi,Tong,Zi,Jian,Yi4,Chang,Empty,Empty);
		#ifdef DebugDisplay
			Prints(DisplayPort,"CR95����쳣��\n");
			Prints(DisplayPort,"ϵͳ�Լ��쳣�������˼��ɼ�����\n");
		#endif
		for(x=0;x<255;x++)	{			//û���ܼ�⵽CR95��������ѭ���������̣��ȴ��û��˳�
			Key_Push = KEY_Scan();
			if(Key_Push == 3)	{				//���»��˰���   �˳�ѭ�� ����
				#ifdef DebugDisplay
					Prints(DisplayPort,"ϵͳ�쳣���û�ѡ�������\n");
				#endif
				x=254;
			}
			else	{			//û�а��»��˼�  ������ѭ��
				x=0;
			}
		}
	}
	delay_ms(5000);
	//ϵͳ�Լ����������� ѡ����ģʽ 
	System_Status = 1;									//ϵͳ״̬��ѡ����ģʽ
	clear_screen();
	Position = 1;												//���λ�õ�һ��
	display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
	display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"��ѡ����ģʽ��\n");
		Prints(DisplayPort,"->  �������ݽ���ģʽ��\n");
		Prints(DisplayPort,"    �������ݷ���ģʽ��\n");
	#endif
	while(1)	{
			
		switch(System_Status)	{
			case 0:		{//ϵͳ�Լ�--Ӧ���Ѿ����ˣ��������
				#ifdef DebugDisplay
					Prints(DisplayPort,"ϵͳ�������β�Ӧ�ý��룡����\n");
				#endif
				break;
			}
			case 1:		{//ѡ����ģʽ
				Key_Push = KEY_Scan();
			  if(Key_Push == 1)			{			//���·����
					if(Position == 1)			{								//ԭ����ڵ�һ�У������λ�ø�Ϊ2�����ƶ����
						Position = 2;
						display_graphic_16x16(1,1,Empty);			//�����һ�й��
						display_graphic_16x16(3,1,Triangle);	//�ڶ�����ʾ���
						#ifdef DebugDisplay
							Prints(DisplayPort,"��ѡ����ģʽ��\n");
							Prints(DisplayPort,"    �������ݽ���ģʽ��\n");
							Prints(DisplayPort,"->  �������ݷ���ģʽ��\n");
						#endif
					}
					else		{															//ԭ����ڵ�һ�У������λ�ø�Ϊ1�����ƶ����
						Position = 1;
						display_graphic_16x16(3,1,Empty);			//�����2�й��
						display_graphic_16x16(1,1,Triangle);	//��1����ʾ���
						#ifdef DebugDisplay
							Prints(DisplayPort,"��ѡ����ģʽ��\n");
							Prints(DisplayPort,"->  �������ݽ���ģʽ��\n");
							Prints(DisplayPort,"    �������ݷ���ģʽ��\n");
						#endif
					}
				}
				else if(Key_Push == 2)		{			//���½����
					if(Position == 1)		{									//ԭ����ڵ�һ�У��������ݽ���ģʽ����ϵͳ״̬�ı䣬������ �����ݽ����С�
						System_Status = 2;									//ϵͳ״̬��Ϊ���ݽ���״̬
						clear_screen();
						Position=1;
						display_graphic_16x16_page(1,Shu,Ju,Jie,Shou,Zhong,Empty,Empty,Empty);
						#ifdef DebugDisplay
							Prints(DisplayPort,"�������ݽ���ģʽ��--�ȴ�����ָ������ݣ���\n");
						#endif
					}
					else		{															//ԭ����ڵڶ��У��������ݷ���ѡ��ģʽ����ϵͳ״̬�ı䣬����ʾ���ݷ���ѡ��״̬
						System_Status = 3;									//ϵͳ״̬��Ϊ���ݷ���ѡ��״̬
						clear_screen();
						Position=1;
						#ifdef DebugDisplay
							Prints(DisplayPort,"�������ݷ���ģʽ--���ݷ���ѡ��\n");
						#endif
						if((Send_Type & 0x1) == 0x01)		{//��1�������ݷ���
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"-> ��һ������ ����\n");
							#endif
						}
						else	{	//��1�����ݲ�����
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"-> ��һ������ ������\n");
							#endif
						}
						if((Send_Type & 0x2) == 0x02)		{//��2�������ݷ���
							display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   �ڶ������� ����\n");
							#endif
						}
						else	{	//��2�����ݲ�����
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   �ڶ������� ������\n");
							#endif
						}
						if((Send_Type & 0x4) == 0x04)		{//��3�������ݷ���
							display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   ���������� ����\n");
							#endif
						}
						else	{	//��3�����ݲ�����
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   ���������� ������\n");
							#endif
						}
						if((Send_Type & 0x8) == 0x08)		{//��4�������ݷ���
							display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   ���������� ����\n");
							#endif
						}
						else	{	//��4�����ݲ�����
							display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   ���������� ������\n");
							#endif
						}
					}
				}
				else	if(Key_Push == 3)		{			//���»��˰���
					System_Status = 1;									//ϵͳ״̬��ѡ����ģʽ
					clear_screen();
					Position = 1;												//���λ�õ�һ��
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
					#ifdef DebugDisplay
						Prints(DisplayPort,"��ѡ����ģʽ��\n");
						Prints(DisplayPort,"    �������ݽ���ģʽ��\n");
						Prints(DisplayPort,"->  �������ݷ���ģʽ��\n");
					#endif
				}
				Key_Push=0;
				break;
			}//ѡ����ģʽ
			case 2:		{//���ݽ���ģʽ
				if(PC_Instruction==1)		{			//���յ�ָ��1  ��ʼ��ָ���ʽ��FLASH,������Ļ��ʾ
					PC_Instruction=0;						//�������ָ���־
					Sum=0;							//���У���
					clear_screen();
					display_graphic_16x16_page(2,Shou,Dao,Chu1,Shi3,Hua,Zhi,Ling,Empty);			//��ʾ"�յ���ʼ��ָ��"
					display_graphic_16x16_page(3,Chu1,Shi3,Hua,Cun,Chu,Qi,Empty,Empty);				//��ʾ����ʼ���洢����
					Flash_Format();
					#ifdef DebugDisplay
						Prints(DisplayPort,"�յ�ָ��1--��ʼ��ָ�\n");
						Prints(DisplayPort,"��ʼ����ɣ����س�ʼ����ȷָ�\n");
					#endif
					display_graphic_16x16_page(3,Chu1,Shi3,Hua,Wan,Cheng,Empty,Empty,Empty);	//��ʾ����ʼ����ɡ�
					//��PC���س�ʼ����ȷָ��
					UART_SendByte(DisplayPort,0x7E);
					UART_SendByte(DisplayPort,0x7E);
					UART_SendByte(DisplayPort,0xCD);
					UART_SendByte(DisplayPort,0x03);
					UART_SendByte(DisplayPort,0xCC);
				}
				else if(PC_Instruction==2)	{	//���յ�ָ��2   װ�����ָ��  �����ݽ���У�顣
					PC_Instruction=0;						//�������ָ���־
					Sum=0;							//���У���
					clear_screen();
					display_graphic_16x16_page(2,Shou,Dao,Zhuang,Ding,Zhi,Ling,Empty,Empty);			//��ʾ"�յ�װ��ָ��"
					if(((Type1_Len+184-1)/184==Type1_Num) && 
						 ((Type2_Len+184-1)/184==Type2_Num) &&
						 ((Type3_Len+184-1)/184==Type3_Num) &&
						 ((Type4_Len+184-1)/184==Type4_Num))	{	//�ж��Ƿ����ݽ������   ���Ƿ���յ����һ������		��PC����װ����ȷָ��
						#ifdef DebugDisplay
							Prints(DisplayPort,"��һ������Ӧ�յ� ");
							PrintHex(DisplayPort,(Type1_Len+184-1)/184);
							Prints(DisplayPort," �����ݣ��յ� ");
							PrintHex(DisplayPort,Type1_Num);
							Prints(DisplayPort," ��\n");
							Prints(DisplayPort,"�ڶ�������Ӧ�յ� ");
							PrintHex(DisplayPort,(Type2_Len+184-1)/184);
							Prints(DisplayPort," �����ݣ��յ� ");
							PrintHex(DisplayPort,Type2_Num);
							Prints(DisplayPort," ��\n");
							Prints(DisplayPort,"����������Ӧ�յ� ");
							PrintHex(DisplayPort,(Type3_Len+184-1)/184);
							Prints(DisplayPort," �����ݣ��յ� ");
							PrintHex(DisplayPort,Type3_Num);
							Prints(DisplayPort," ��\n");
							Prints(DisplayPort,"����������Ӧ�յ� ");
							PrintHex(DisplayPort,(Type4_Len+184-1)/184);
							Prints(DisplayPort," �����ݣ��յ� ");
							PrintHex(DisplayPort,Type4_Num);
							Prints(DisplayPort," ��\n");
							Prints(DisplayPort,"����װ����ȷָ�\n");
						#endif
						display_graphic_16x16_page(3,Zhuang,Ding,Wan,Cheng,Empty,Empty,Empty,Empty);			//��ʾ"װ�����"
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x07);
						UART_SendByte(DisplayPort,0xD0);
					}
					else	{			//��PC����װ������ָ��
						#ifdef DebugDisplay
							Prints(DisplayPort,"װ�����󣡷���װ������ָ�\n");
						#endif
						display_graphic_16x16_page(3,Zhuang,Ding,Yi4,Chang,Empty,Empty,Empty,Empty);			//��ʾ"װ���쳣"
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x08);
						UART_SendByte(DisplayPort,0xD1);
					}
				}
				else if(PC_Instruction == 255){  //���յ��������
					PC_Instruction=0;						//�������ָ���־
					#ifdef DebugDisplay
						Prints(DisplayPort,"���յ�һ�����ݣ�\n");
					#endif
					//�ж������Ƿ���ȷ
					if(Sum==ReceivedData[199])			//У�����ȷ
					{
						Sum=0;												//����У���
						Temp_DataLen = ReceivedData[7];												//��ȡ���������ݵĳ���
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[6];
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[5];
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[4];
						Number = ReceivedData[9];															//��ȡ��ǰ�����
						Number = (Number<<8) + ReceivedData[8];
						if(ReceivedData[3]==1)										//�õ����ǵ�һ��������
						{
							Type1_Len = Temp_DataLen;		//�Ե�һ�����ݳ��Ƚ��и�ֵ
							Type1_Num = Number;					//�Ե�1�����ݰ���Ÿ�ֵ
							#ifdef DebugDisplay
								Prints(DisplayPort,"��һ�����ݴ�ŵ�ַΪ����1ҳ����6ҳ��ÿ������ռ��1ҳ��\n");
							#endif
							//����һ�������ݴ洢����Ӧ��ַ   ��һ������ ���1K�ֽڣ����ռ��6ҳ  �ӵ�1ҳ��ʼ�洢  0ҳ256�ֽڱ���
							Flash_Write_Page(ReceivedData,Number,200);
						}
						else if(ReceivedData[3]==2)								//�õ����ǵ�2��������
						{
							Type2_Len = Temp_DataLen;		//�Ե�2�������ݳ��ȸ�ֵ
							Type2_Num = Number;					//�Ե�2�����ݰ���Ÿ�ֵ
							#ifdef DebugDisplay
								Prints(DisplayPort,"�ڶ������ݴ�ŵ�ַΪ����7ҳ����12ҳ��ÿ������ռ��1ҳ��\n");
							#endif
							//����2�������ݴ洢����Ӧ��ַ   ��2������ ���1K�ֽڣ����ռ��6ҳ  �ӵ�7ҳ��ʼ�洢
							Flash_Write_Page(ReceivedData,Number+6,200);
						}
						else if(ReceivedData[3]==3)								//�õ����ǵ�3��������
						{
							Type3_Len = Temp_DataLen;		//�Ե�3�������ݳ��ȸ�ֵ
							Type3_Num = Number;					//�Ե�3�����ݰ���Ÿ�ֵ
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݴ�ŵ�ַΪ����13ҳ����21ҳ��ÿ������ռ��1ҳ��\n");
							#endif
							//����3�������ݴ洢����Ӧ��ַ   ��3������ ���1.5k�ֽڣ����ռ��9ҳ �ӵ�13ҳ��ʼ�洢
							Flash_Write_Page(ReceivedData,Number+12,200);
						}
						else if(ReceivedData[3]==4)								//�õ����ǵ�4��������
						{
							Type4_Len = Temp_DataLen;		//�Ե�4�������ݳ��ȸ�ֵ
							Type4_Num = Number;					//�Ե�4�����ݰ���Ÿ�ֵ
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݴ�ŵ�ַΪ����22ҳ����47ҳ��ÿ������ռ��1ҳ��\n");
							#endif
							//����4�����ݴ洢����Ӧ��ַ      ��4������ ���4.5k�ֽڣ����ռ��26ҳ ��22ҳ��ʼ�洢
							Flash_Write_Page(ReceivedData,Number+21,200);
						}
						#ifdef DebugDisplay
							Prints(DisplayPort,"�յ��� ");
						  PrintHex(DisplayPort,ReceivedData[3]);
							Prints(DisplayPort," �������ݣ��� ");
							PrintHex(DisplayPort,Number);
//							PrintShortIntHex(DisplayPort,Number);
							Prints(DisplayPort," �����ݣ����������ܳ� ");
							PrintLongInt(DisplayPort,Temp_DataLen);
							Prints(DisplayPort," \n");
							Prints(DisplayPort,"����У����ȷ�����ذ��洢��ȷָ�\n");
						#endif
						clear_screen();
						display_graphic_16x16_page(2,Shou,Dao,Zheng,Chang,Shu,Ju,Empty,Empty);			//��ʾ"�յ���������"
						//��PC���ذ��洢��ȷָ��
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x05);
						UART_SendByte(DisplayPort,0xCE);
					}
					else				//У��Ͳ���ȷ
					{
						Sum=0;												//����У���
						#ifdef DebugDisplay
							Prints(DisplayPort,"У��ʹ��󣡷��ذ��洢����ָ�\n");
						#endif
						clear_screen();
						display_graphic_16x16_page(2,Shou,Dao,Yi4,Chang,Shu,Ju,Empty,Empty);			//��ʾ"�յ��쳣����"
						//��PC���ذ��洢����ָ��
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x06);
						UART_SendByte(DisplayPort,0xCF);
					}
				}
				Key_Push = KEY_Scan();
				if(Key_Push == 3)	{				//���»��˰���
					System_Status = 1;									//ϵͳ״̬��ѡ����ģʽ
					clear_screen();
					Position = 1;												//���λ�õ�һ��
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
					Type1_Len=0,Type2_Len=0,Type3_Len=0,Type4_Len=0;// �����������ܳ�����
					Type1_Num=0,Type2_Num=0,Type3_Num=0,Type4_Num=0;//���������ݽ��յ������һ����� ����
					Number=0;						//��ǰ���������
					#ifdef DebugDisplay
						Prints(DisplayPort,"��ѡ����ģʽ��\n");
						Prints(DisplayPort,"->  �������ݽ���ģʽ��\n");
						Prints(DisplayPort,"    �������ݷ���ģʽ��\n");
					#endif
				}
				else	{										//��������  �޷�Ӧ
					System_Status = 2;
					Position = 1;
				}
				Key_Push=0;
			}//���ݽ���ģʽ
			case 3:		{//���ݷ���ѡ��ģʽ
				Key_Push = KEY_Scan();
				if(Key_Push == 1)		{	//���·����
					Position++;
					if(Position ==2){		//��꽫�Ƶ��ڶ���
						display_graphic_16x16(1,1,Empty);			//�����һ�й��
						display_graphic_16x16(3,1,Triangle);	//��ʾ�ڶ��й��
						#ifdef DebugDisplay
							Prints(DisplayPort,"ѡ��ڶ��������Ƿ��ͣ�\n");
						#endif
					}
					else if(Position == 3){		//��꽫�Ƶ���3��
						display_graphic_16x16(3,1,Empty);			//�����2�й��
						display_graphic_16x16(5,1,Triangle);	//��ʾ��3�й��
						#ifdef DebugDisplay
							Prints(DisplayPort,"ѡ������������Ƿ��ͣ�\n");
						#endif
					}
					else if(Position == 4){		//��꽫�Ƶ���4��
						display_graphic_16x16(5,1,Empty);			//�����3�й��
						display_graphic_16x16(7,1,Triangle);	//��ʾ��4�й��
						#ifdef DebugDisplay
							Prints(DisplayPort,"ѡ������������Ƿ��ͣ�\n");
						#endif
					}
					else if(Position == 5){		//��꽫�Ƶ���5��
						#ifdef DebugDisplay
							Prints(DisplayPort,"ȷ�����ݷ���ѡ�񣬿�ʼ�������ݣ�\n");
						#endif
						if((Send_Type & 0x2) == 0x02)		{//��2�������ݷ���
							display_graphic_16x16_page(1,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						else	{	//��2�����ݲ�����
							display_graphic_16x16_page(1,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x4) == 0x04)		{//��3�������ݷ���
							display_graphic_16x16_page(2,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
						}
						else	{	//��3�����ݲ�����
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x8) == 0x08)		{//��4�������ݷ���
							display_graphic_16x16_page(3,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						else	{	//��4�����ݲ�����
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						display_graphic_16x16_page(4,Triangle,Shu,Ju,Fa,Song,Empty,Empty,Empty);
					}
					else if(Position ==6){			//��꽫�ƶ�����6��  ��귵�ص�һ�У���ʾ���õ����ݷ���ѡ��״̬
						#ifdef DebugDisplay
							Prints(DisplayPort,"ѡ���һ�������Ƿ��ͣ�\n");
						#endif
						Position=1;
						if((Send_Type & 0x1) == 0x01)		{//��1�������ݷ���
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
						}
						else	{	//��1�����ݲ�����
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x2) == 0x02)		{//��2�������ݷ���
							display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						else	{	//��2�����ݲ�����
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x4) == 0x04)		{//��3�������ݷ���
							display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
						}
						else	{	//��3�����ݲ�����
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x8) == 0x08)		{//��4�������ݷ���
							display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						else	{	//��4�����ݲ�����
							display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
						}
					}
				}
				else if(Key_Push == 2){	//����ȷ�ϼ�
					if(Position == 1)		{		//����ڵ�һ�У�����һ�����ݷ����벻���ͻ�����ʵ������������ǻ���
						if((Send_Type & 0x1) == 0x01)		{//	��1��������ԭ��Ϊ����  ��Ϊ�����ͣ�ʵ����Ǹ�Ϊ����
							Send_Type &=  0xFE;						//Send_Type��bit0����
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"��һ�����ݲ����ͣ�\n");
							#endif
						}
						else	{//��һ��������ԭ��Ϊ�����ͣ���Ϊ���ͣ�������Ǹ�Ϊʵ�����
							Send_Type |=  0x01;						//Send_Type��bit0��1
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"��һ�����ݷ��ͣ�\n");
							#endif
						}
					}
					else if(Position == 2){		//����ڵ�2�У�����2�����ݷ����벻���ͻ�����ʵ������������ǻ���
						if((Send_Type & 0x02) == 0x02)		{//	��2��������ԭ��Ϊ����  ��Ϊ�����ͣ�ʵ����Ǹ�Ϊ����
							Send_Type &=  0xFD;						//Send_Type��bit1����
							display_graphic_16x16_page(2,Triangle,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"�ڶ������ݲ����ͣ�\n");
							#endif
						}
						else	{//��2��������ԭ��Ϊ�����ͣ���Ϊ���ͣ�������Ǹ�Ϊʵ�����
							Send_Type |=  0x02;						//Send_Type��bit1��1
							display_graphic_16x16_page(2,Triangle,FullStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"�ڶ������ݷ��ͣ�\n");
							#endif
						}
					}
					else if(Position ==3){		//����ڵ�3�У�����3�����ݷ����벻���ͻ�����ʵ������������ǻ���
						if((Send_Type & 0x04) == 0x04)		{//	��3��������ԭ��Ϊ����  ��Ϊ�����ͣ�ʵ����Ǹ�Ϊ����
							Send_Type &=  0xFB;						//Send_Type��bit2����
							display_graphic_16x16_page(3,Triangle,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݲ����ͣ�\n");
							#endif
						}
						else	{//��2��������ԭ��Ϊ�����ͣ���Ϊ���ͣ�������Ǹ�Ϊʵ�����
							Send_Type |=  0x04;						//Send_Type��bit1��1
							display_graphic_16x16_page(3,Triangle,FullStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݷ��ͣ�\n");
							#endif
						}
					}
					else if(Position ==4){		//����ڵ�4�У�����4�����ݷ����벻���ͻ�����ʵ������������ǻ���
						if((Send_Type & 0x08) == 0x08)		{//	��4��������ԭ��Ϊ����  ��Ϊ�����ͣ�ʵ����Ǹ�Ϊ����
							Send_Type &=  0xF7;						//Send_Type��bit3����
							display_graphic_16x16_page(4,Triangle,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݲ����ͣ�\n");
							#endif
						}
						else	{//��2��������ԭ��Ϊ�����ͣ���Ϊ���ͣ�������Ǹ�Ϊʵ�����
							Send_Type |=  0x08;						//Send_Type��bit3��1
							display_graphic_16x16_page(4,Triangle,FullStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"���������ݷ��ͣ�\n");
							#endif
						}
					}
					else if(Position ==5){		//����ڵ�5�У��������ݿ�ʼ����   ���ԡ����ݷ����С�
						System_Status = 4;
						Position = 1;
						clear_screen();
						display_graphic_16x16_page(1,Shu,Ju,Fa,Song,Zhong,Empty,Empty,Empty);
						#ifdef DebugDisplay
							Prints(DisplayPort,"���������б�����\n");
							if((Send_Type & 0x01) == 0x01)		{
								Prints(DisplayPort,"  ��1�����ݷ��ͣ�\n");
							}
							else	{
								Prints(DisplayPort,"  ��1�����ݲ����ͣ�\n");
							}
							if((Send_Type & 0x02) == 0x02)		{
								Prints(DisplayPort,"  ��2�����ݷ��ͣ�\n");
							}
							else	{
								Prints(DisplayPort,"  ��2�����ݲ����ͣ�\n");
							}
							if((Send_Type & 0x04) == 0x04)		{
								Prints(DisplayPort,"  ��3�����ݷ��ͣ�\n");
							}
							else	{
								Prints(DisplayPort,"  ��3�����ݲ����ͣ�\n");
							}
							if((Send_Type & 0x08) == 0x08)		{
								Prints(DisplayPort,"  ��4�����ݷ��ͣ�\n");
							}
							else	{
								Prints(DisplayPort,"  ��4�����ݲ����ͣ�\n");
							}
							Prints(DisplayPort,"��ʼ����ѡ�����ݣ�\n");
						#endif
					}
				}
				else if (Key_Push ==3){	//���»��˼�   ����ѡ����ģʽ
					System_Status = 1;									//ϵͳ״̬��ѡ����ģʽ
					Send_Type=0x0F;											//Ĭ�Ϸ�����������
					clear_screen();
					Position = 1;												//���λ�õ�һ��
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"��ѡ����ģʽ��\n");
						Prints(DisplayPort,"->  �������ݽ���ģʽ��\n");
						Prints(DisplayPort,"    �������ݷ���ģʽ��\n");
					#endif
				}
				break;
			}
			case 4:{	//���ݷ�����		�����շ���ѡ����з��ͣ�ʵ�ʷ������д洢�������ݣ������͵Ŀ��Բ���ȡ�
				y=0;																			//��Ϊ�ȴ�KFB���س�ʱ����
				WriteNum = 0;															//д���������
				for(Tempx=1;Tempx<39;Tempx++)		{					//���ݴ洢����--��һҳ����38ҳ
					ReturnData = 0;						//KFB����״̬����
					//��1ҳ�洢�����ݶ�ȡ200��������ReceivedData[]��
					Flash_Read(ReceivedData,Tempx,200);
					for(WriteFrameNum=0;WriteFrameNum<46;WriteFrameNum++)		{
						WriteNum++;
						for(Retry=0;Retry<10;Retry++)		{		//KFB����������д�����ݲ�ͬ�����Դ���
							USART_KFB_Write(WriteFrameNum,ReceivedData[WriteFrameNum*4+10],ReceivedData[WriteFrameNum*4+11],ReceivedData[WriteFrameNum*4+12],ReceivedData[WriteFrameNum*4+13]);
							y=0;
							while(ReturnData == 0)	{				//�ȴ�KFB��������		д��ʱ�������ݲ���Ҫ��ֱ������
								y++;
								delay_ms(100);
								if(y>100)		{									//����1s�޷��أ�ֱ�ӱ���������ѭ��������--���ݷ����쳣
									#ifdef DebugDisplay
										Prints(DisplayPort,"CR95д�뷵�س�ʱ��ϵͳ���󣡣�������������\n");
									#endif
									clear_screen();
									display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
								}
							}
							ReturnData = 0;									//KFB����״̬����
							#ifdef DebugDisplay
								Prints(DisplayPort,"CR95-д�����������Ϊ��");
								for(x=0;x<6;x++){
									PrintHex(DisplayPort,Response[x]);
									Prints(DisplayPort,"  ");
								}
								Prints(DisplayPort,"\n");
							#endif
							USART_KFB_Read(WriteFrameNum);							//��ȡKFB��WriteFrameNum��ֵַ
							y=0;
							while(ReturnData == 0)	{				//�ȴ�KFB��������   ��ȡ���ݽ��бȽϣ�Ϊд�����ݣ�����д�룬����д�����ݣ�����10��
								y++;
								delay_ms(10);
								if(y>100)		{									//����1s�޷��أ�ֱ�ӱ���������ѭ��������--���ݷ����쳣
									#ifdef DebugDisplay
										Prints(DisplayPort,"CR95��ȡ���س�ʱ��ϵͳ���󣡣�������������\n");
									#endif
									clear_screen();
									display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
									while(1);
								}
							}
							ReturnData = 0;									//KFB����״̬����
							#ifdef DebugDisplay
								Prints(DisplayPort,"CR95-��ȡ���������Ϊ��");
								for(x=0;x<6;x++){
									PrintHex(DisplayPort,Response[x]);
									Prints(DisplayPort,"  ");
								}
								Prints(DisplayPort,"\n");
							#endif
							if((Response[2] == ReceivedData[WriteFrameNum*4+10]) &&
								 (Response[3] == ReceivedData[WriteFrameNum*4+11]) &&
								 (Response[4] == ReceivedData[WriteFrameNum*4+12]) &&
								 (Response[5] == ReceivedData[WriteFrameNum*4+13]) )	{	//KFB����������д������ͬ��ֱ���������Բ��衣
								Retry=20;						//����Retryֵ����Χ����������							
							}
							else	{							//KFB����������д�����ݲ�ͬ�����ԡ�
								;
							}
							if(Retry > 15)		{		//����д����ȷ��
//								#ifdef DebugDisplay
									PrintShortIntHex(DisplayPort,WriteNum);
									Prints(DisplayPort,"����д����ȷ��\n");
//								#endif
								clear_screen();
								display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Zheng,Chang,Empty,Empty);
							}
							else						{			//����д�벻��ȷ��������ѭ��������--���ݷ����쳣
								#ifdef DebugDisplay
									Prints(DisplayPort,"����д���쳣��������Զ��쳣������������\n");
								#endif
								clear_screen();
								display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
							}
						}
					}
				}
				//����ȫ��������ɣ��������ݷ���ѡ��״̬��
				System_Status = 3;									//ϵͳ״̬��Ϊ���ݷ���ѡ��״̬
				clear_screen();
				Position=1;
				#ifdef DebugDisplay
					Prints(DisplayPort,"���ݷ�����ɣ��������ݷ���ѡ��ģʽ��\n");
				#endif
				if((Send_Type & 0x1) == 0x01)		{//��1�������ݷ���
					display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"-> ��һ������ ����\n");
					#endif
				}
				else	{	//��1�����ݲ�����
					display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"-> ��һ������ ������\n");
					#endif
				}
				if((Send_Type & 0x2) == 0x02)		{//��2�������ݷ���
					display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   �ڶ������� ����\n");
					#endif
				}
				else	{	//��2�����ݲ�����
					display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   �ڶ������� ������\n");
					#endif
				}
				if((Send_Type & 0x4) == 0x04)		{//��3�������ݷ���
					display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   ���������� ����\n");
					#endif
				}
				else	{	//��3�����ݲ�����
					display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   ���������� ������\n");
					#endif
				}
				if((Send_Type & 0x8) == 0x08)		{//��4�������ݷ���
					display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   ���������� ����\n");
					#endif
				}
				else	{	//��4�����ݲ�����
					display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   ���������� ������\n");
					#endif
				}
			}
		}
	}	 
}

