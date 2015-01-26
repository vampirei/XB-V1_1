
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "My_Config.h"
#include "flash.h"
#include "lcd.h"
#include "key.h"

/***************************************************************/
/*"入",0*/
unsigned char Ru[]=	{0x00,0x00,0x00,0x00,0x00,0x01,0xE2,0x1C,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
									 0x80,0x40,0x20,0x10,0x0C,0x03,0x00,0x00,0x00,0x03,0x0C,0x30,0x40,0x80,0x80,0x00};
/*"进",0*/
unsigned char Jin[]={0x40,0x40,0x42,0xCC,0x00,0x80,0x88,0x88,0xFF,0x88,0x88,0xFF,0x88,0x88,0x80,0x00,
									 0x00,0x40,0x20,0x1F,0x20,0x40,0x50,0x4C,0x43,0x40,0x40,0x5F,0x40,0x40,0x40,0x00};
/*"系",0*/
unsigned char Xi[]={0x00,0x00,0x22,0x32,0x2A,0xA6,0xA2,0x62,0x21,0x11,0x09,0x81,0x01,0x00,0x00,0x00,
                  0x00,0x42,0x22,0x13,0x0B,0x42,0x82,0x7E,0x02,0x02,0x0A,0x12,0x23,0x46,0x00,0x00};
/*"统",1*/
unsigned char Tong[]={0x20,0x30,0xAC,0x63,0x30,0x00,0x88,0xC8,0xA8,0x99,0x8E,0x88,0xA8,0xC8,0x88,0x00,
									  0x22,0x67,0x22,0x12,0x12,0x80,0x40,0x30,0x0F,0x00,0x00,0x3F,0x40,0x40,0x71,0x00};
/*"模"*/
unsigned char Mo[]={0x10,0x10,0xD0,0xFF,0x90,0x14,0xE4,0xAF,0xA4,0xA4,0xA4,0xAF,0xE4,0x04,0x00,0x00,
										0x04,0x03,0x00,0xFF,0x00,0x89,0x4B,0x2A,0x1A,0x0E,0x1A,0x2A,0x4B,0x88,0x80,0x00};
/*"式"*/
unsigned char Shi[]={0x10,0x10,0x90,0x90,0x90,0x90,0x90,0x10,0x10,0xFF,0x10,0x10,0x11,0x16,0x10,0x00,
										 0x00,0x20,0x60,0x20,0x3F,0x10,0x10,0x10,0x00,0x03,0x0C,0x10,0x20,0x40,0xF8,0x00};
/*"接"*/
unsigned char Jie[]={0x10,0x10,0x10,0xFF,0x10,0x50,0x44,0x54,0x65,0xC6,0x44,0x64,0x54,0x44,0x40,0x00,
										 0x04,0x44,0x82,0x7F,0x01,0x82,0x82,0x4A,0x56,0x23,0x22,0x52,0x4E,0x82,0x02,0x00};
/*"收"*/
unsigned char Shou[]={0x00,0xFC,0x00,0x00,0xFF,0x00,0x40,0x20,0xD8,0x17,0x10,0x10,0xF0,0x10,0x10,0x00,
										  0x00,0x0F,0x04,0x02,0xFF,0x00,0x80,0x40,0x21,0x16,0x08,0x16,0x21,0x40,0x80,0x00};
/*"发"*/					
unsigned char Fa[]={0x00,0x00,0x18,0x16,0x10,0xD0,0xB8,0x97,0x90,0x90,0x90,0x92,0x94,0x10,0x00,0x00,
						        0x00,0x20,0x10,0x8C,0x83,0x80,0x41,0x46,0x28,0x10,0x28,0x44,0x43,0x80,0x80,0x00};
/*"送"*/
unsigned char Song[]={0x40,0x40,0x42,0xCC,0x00,0x88,0x89,0x8E,0x88,0xF8,0x88,0x8C,0x8B,0x88,0x80,0x00,
										  0x00,0x40,0x20,0x1F,0x20,0x40,0x50,0x48,0x46,0x41,0x42,0x44,0x58,0x40,0x40,0x00};		
/*"第"*/
unsigned char Di[]={0x08,0x04,0x93,0x92,0x96,0x9A,0x92,0xFA,0x94,0x93,0x92,0x96,0xFA,0x02,0x02,0x00,
										0x40,0x40,0x47,0x24,0x24,0x14,0x0C,0xFF,0x04,0x04,0x24,0x44,0x24,0x1C,0x00,0x00};
/*"一"*/
unsigned char Yi[]={0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,
										0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*"二"*/
unsigned char Er[]={0x00,0x00,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x08,0x00,0x00,0x00,
										0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x00};
/*"三"*/
unsigned char San[]={0x00,0x04,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x84,0x04,0x00,0x00,
										 0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00};
/*"四"*/
unsigned char Si[]={0x00,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
										0x00,0x7F,0x28,0x24,0x23,0x20,0x20,0x20,0x20,0x21,0x22,0x22,0x22,0x7F,0x00,0x00};	
/*"类"*/
unsigned char Lei[]={0x00,0x10,0x10,0x92,0x54,0x30,0x10,0xFF,0x10,0x30,0x54,0x92,0x10,0x10,0x00,0x00,
										 0x84,0x85,0x45,0x44,0x24,0x14,0x0C,0x06,0x0C,0x14,0x24,0x44,0x45,0x84,0x84,0x00};		
/*"数"*/
unsigned char Shu[]={0x90,0x52,0x34,0x10,0xFF,0x10,0x34,0x52,0x80,0x70,0x8F,0x08,0x08,0xF8,0x08,0x00,
										 0x82,0x9A,0x56,0x63,0x22,0x52,0x8E,0x00,0x80,0x40,0x33,0x0C,0x33,0x40,0x80,0x00};
/*"据"*/
unsigned char Ju[]={0x10,0x10,0xFF,0x10,0x90,0x00,0xFE,0x92,0x92,0x92,0xF2,0x92,0x92,0x9E,0x80,0x00,
										0x42,0x82,0x7F,0x01,0x80,0x60,0x1F,0x00,0xFC,0x44,0x47,0x44,0x44,0xFC,0x00,0x00};
/*"中"*/
unsigned char Zhong[]={0x00,0x00,0xF0,0x10,0x10,0x10,0x10,0xFF,0x10,0x10,0x10,0x10,0xF0,0x00,0x00,0x00,
											 0x00,0x00,0x0F,0x04,0x04,0x04,0x04,0xFF,0x04,0x04,0x04,0x04,0x0F,0x00,0x00,0x00};
/*" "*/
unsigned char Empty[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};//清空一个汉字位置
/*"空心五角形"*/
unsigned char EmptyStart[]={0x40,0xC0,0x40,0x40,0x40,0x60,0x1C,0x03,0x1C,0x60,0x40,0x40,0x40,0xC0,0x40,0x00,
														0x00,0x00,0x41,0x39,0x26,0x10,0x10,0x08,0x08,0x10,0x26,0x39,0x41,0x00,0x00,0x00};	
/*"实心五角形"*/
unsigned char FullStart[]={0x40,0xC0,0xC0,0xC0,0xC0,0xE0,0xFC,0xFF,0xFC,0xE0,0xC0,0xC0,0xC0,0xC0,0x40,0x00,
													 0x00,0x00,0x41,0x39,0x3F,0x1F,0x1F,0x0F,0x1F,0x1F,0x3F,0x39,0x41,0x00,0x00,0x00};
/*"三角形"*/
unsigned char Triangle[]={0,0,0,0,0,0,254,252,248,240,224,192,128,0,0,0,0,0,0,0,0,0,63,31,15,7,3,1,0,0,0,0};
/*"自"*/
unsigned char Zi[]={0x00,0x00,0x00,0xF8,0x88,0x8C,0x8A,0x89,0x88,0x88,0x88,0xF8,0x00,0x00,0x00,0x00,
										0x00,0x00,0x00,0xFF,0x44,0x44,0x44,0x44,0x44,0x44,0x44,0xFF,0x00,0x00,0x00,0x00};
/*"检"*/
unsigned char Jian[]={0x10,0x10,0xD0,0xFF,0x90,0x50,0x20,0x50,0x4C,0x43,0x4C,0x50,0x20,0x40,0x40,0x00,
											0x04,0x03,0x00,0xFF,0x00,0x41,0x44,0x58,0x41,0x4E,0x60,0x58,0x47,0x40,0x40,0x00};
/*"存"*/
unsigned char Cun[]={0x04,0x04,0x84,0xE4,0x1C,0x07,0x24,0x24,0x24,0x24,0xA4,0x64,0x24,0x04,0x04,0x00,
										 0x02,0x01,0x00,0xFF,0x00,0x02,0x02,0x42,0x82,0x7F,0x02,0x02,0x02,0x02,0x02,0x00};
/*"储"*/
unsigned char Chu[]={0x80,0x60,0xF8,0x87,0x84,0x98,0x00,0x20,0x24,0xA4,0x7F,0x24,0x34,0x28,0x24,0x00,
										 0x00,0x00,0xFF,0x00,0x00,0x7F,0x24,0x12,0x01,0xFF,0x49,0x49,0x49,0xFF,0x00,0x00};
/*"器"*/
unsigned char Qi[]={0x80,0x80,0x9E,0x92,0x92,0x92,0x9E,0xE0,0x80,0x9E,0xB2,0xD2,0x92,0x9E,0x80,0x00,
										0x08,0x08,0xF4,0x94,0x92,0x92,0xF1,0x00,0x01,0xF2,0x92,0x94,0x94,0xF8,0x08,0x00};
/*"正"*/
unsigned char Zheng[]={0x00,0x02,0x02,0xC2,0x02,0x02,0x02,0xFE,0x82,0x82,0x82,0x82,0x82,0x02,0x00,0x00,
											 0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x00};
/*"常"*/
unsigned char Chang[]={0x20,0x18,0x08,0xEA,0xAC,0xA8,0xA8,0xAF,0xA8,0xA8,0xAC,0xEA,0x08,0x28,0x18,0x00,
											 0x00,0x00,0x3E,0x02,0x02,0x02,0x02,0xFF,0x02,0x02,0x12,0x22,0x1E,0x00,0x00,0x00};
/*"异"*/
unsigned char Yi4[]={0x00,0x00,0x7E,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0x9E,0x80,0xE0,0x00,0x00,
										 0x08,0x88,0x48,0x28,0x1E,0x08,0x08,0x08,0x08,0x08,0xFE,0x08,0x08,0x08,0x08,0x00};
/*"到"*/
unsigned char Dao[]={0x42,0x62,0x52,0x4A,0xC6,0x42,0x52,0x62,0xC2,0x00,0xF8,0x00,0x00,0xFF,0x00,0x00,
										 0x40,0xC4,0x44,0x44,0x7F,0x24,0x24,0x24,0x20,0x00,0x0F,0x40,0x80,0x7F,0x00,0x00};
/*"初"*/
unsigned char Chu1[]={0x08,0x08,0x89,0xEA,0x18,0x88,0x00,0x04,0x04,0xFC,0x04,0x04,0x04,0xFC,0x00,0x00,
                     0x02,0x01,0x00,0xFF,0x01,0x86,0x40,0x20,0x18,0x07,0x40,0x80,0x40,0x3F,0x00,0x00};
/*"始"*/
unsigned char Shi3[]={0x10,0x10,0xF0,0x1F,0x10,0xF0,0x00,0x40,0xE0,0x58,0x47,0x40,0x50,0x60,0xC0,0x00,
                     0x40,0x22,0x15,0x08,0x16,0x21,0x00,0x00,0xFE,0x42,0x42,0x42,0x42,0xFE,0x00,0x00};
/*"化"*/
unsigned char Hua[]={0x00,0x80,0x60,0xF8,0x07,0x00,0x00,0x00,0xFF,0x40,0x20,0x10,0x08,0x04,0x00,0x00,
                     0x01,0x00,0x00,0xFF,0x00,0x04,0x02,0x01,0x3F,0x40,0x40,0x40,0x40,0x40,0x78,0x00};
/*"指"*/
unsigned char Zhi[]={0x10,0x10,0x10,0xFF,0x10,0x90,0x00,0x3F,0x48,0x48,0x44,0x44,0x44,0x42,0x70,0x00,
                     0x04,0x44,0x82,0x7F,0x01,0x00,0x00,0xFF,0x49,0x49,0x49,0x49,0x49,0xFF,0x00,0x00};
/*"令"*/
unsigned char Ling[]={0x80,0x80,0x40,0x20,0x10,0x08,0x14,0x63,0x04,0x08,0x10,0x20,0x40,0x80,0x80,0x00,
                      0x00,0x00,0x00,0x01,0x01,0x09,0x11,0x21,0xD1,0x09,0x05,0x03,0x00,0x00,0x00,0x00};
/*"装"*/
unsigned char Zhuang[]={0x40,0x42,0x24,0x10,0xFF,0x00,0x84,0x44,0x44,0x44,0x7F,0x44,0x44,0x44,0x04,0x00,
                        0x22,0x22,0x12,0x12,0x0A,0xFE,0x42,0x27,0x0A,0x12,0x22,0x32,0x4A,0x42,0x42,0x00};
/*"订"*/
unsigned char Ding[]={0x40,0x40,0x42,0xCC,0x00,0x00,0x04,0x04,0x04,0x04,0xFC,0x04,0x04,0x04,0x04,0x00,
                      0x00,0x00,0x00,0x7F,0x20,0x10,0x00,0x00,0x40,0x80,0x7F,0x00,0x00,0x00,0x00,0x00};
/*"完"*/
unsigned char Wan[]={0x10,0x0C,0x04,0x24,0x24,0x24,0x25,0x26,0x24,0x24,0x24,0x24,0x04,0x14,0x0C,0x00,
                     0x00,0x81,0x81,0x41,0x31,0x0F,0x01,0x01,0x01,0x7F,0x81,0x81,0x81,0xF1,0x00,0x00};
/*"成"*/
unsigned char Cheng[]={0x00,0x00,0xF8,0x88,0x88,0x88,0x88,0x08,0x08,0xFF,0x08,0x09,0x0A,0xC8,0x08,0x00,
                       0x80,0x60,0x1F,0x00,0x10,0x20,0x1F,0x80,0x40,0x21,0x16,0x18,0x26,0x41,0xF8,0x00};
/***************************************************************/





//命令格式：0x55 0xaa Command Data1 Data2 Data3 Data4 Address1 Address2

bool 		TimeUp;
u8 Tempx,x,Len,y;
ReaderConfigStruct					ReaderConfig;							//CR95配置信息
uint8_t											CR95HFhandled;						//
u8 Response[100];
u8 ReturnData;
u8 *pResponse=Response;
u8 ResCR95HF_IDN[17];
u16 TempAdd;
u8 TempAddLow,TempAddHigh;
u8 TempM24Data_1,TempM24Data_2,TempM24Data_3,TempM24Data_4,TempM24Data=0;
u8 Send_Select;																				//发送的数据类型选择
u8 System_Status;		/*系统工作状态，0：系统自检
																		1：选择工作模式
																		2：数据接收模式
																		3：数据发送选择模式
																		4：数据开始发送
										系统工作状态*/
u8 Position;				//光标位置   1:第一行，2：第二行，3：第三行，4：第四行
u8 Key_Push;				//按键值。
u8 ReceivedLen;				//已经接收数据长度
u8 ReceivedData[200];	//串口接收数据缓存
u8 WriteData[200];		//写入MR24的数据
u8 PC_Instruction;		//PC传输的指令  0:保留  1:指令1   2:指令2.。。。。。。255:数据接收完成
u8 Data_Type;
u16 PC_DataType;				//PC传输数据类型   百位：数据是第几类  十位+各位：该类型内第几包数据
                        //    101：第一类 第1包     104：第一类 第4包  407：第四类 第7包
u8 Sum;								//接收PC数据校验和
u32 Type1_Len=0,Type2_Len=0,Type3_Len=0,Type4_Len=0;// 第一类型数据总长
u16 Type1_Num=0,Type2_Num=0,Type3_Num=0,Type4_Num=0;//各类型数据接收到的最后一包序号
u16 Number=0;						//当前包序号
u8 Send_Type=0x0F;			//发送数据类型：   bit0：第一类数据发送为1，不发送为0
												//								 bit1：第二类数据发送为1，不发送为0
												//								 bit2：第三类数据发送为1，不发送为0
												//								 bit3：第四类数据发送为1，不发送为0
												//								 bit4--bit7:保留
u8 WriteFrameNum;
u8 Retry = 0 ;	
u16 WriteNum;						//数据写入计数 

int main(void)
{
	u32 Temp_DataLen;				//读取各类型数据长度的临时数据
	Sum = 0;
	
	
	ReturnData = 0;
	Key_Push = 0;
	SystemInit();  				//系统时钟设置
	delay_init(72);	     	//延时初始化
	NVIC_Configuration();	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	UART2PC_Init(115200);															//初始化PC通讯端口  波特率115200
	UART2CR95_Init(230400);														//初始化与开发板通讯端口 波特率230400
	SPI_Two_Init();																		//公用SPI初始化
	KEY_Init();																				//初始化按键
	initial_lcd();																		//LCD初始化
	clear_screen();																		//LCD清屏
	display_graphic_16x16_page(1,Xi,Tong,Zi,Jian,Zhong,Empty,Empty,Empty);//显示"系统自检中"
	delay_ms(10);
	System_Status=0;																	//系统工作状态为自检
	//自检Flash
	FLASH_Read_ID(Response);													//读取Flash的ID
	Prints(DisplayPort,"Flash's ID is :");
	PrintHex(DisplayPort,Response[0]);
	Prints(DisplayPort,"  ");
	PrintHex(DisplayPort,Response[1]);
	if(Response[0]==0xEF && Response[1]==0x16){				//判断Flash的ID是否正常
		#ifdef DebugDisplay
			Prints(DisplayPort,"检测到FLASH：W25Q64  \r\n");
		#endif
		display_graphic_16x16_page(2,Empty,Cun,Chu,Qi,Zheng,Chang,Empty,Empty);
	}
	else{
		#ifdef	DebugDisplay
			Prints(DisplayPort,"没有检测到FLASH  \r\n");
		#endif
		display_graphic_16x16_page(2,Empty,Cun,Chu,Qi,Yi4,Chang,Empty,Empty);
	}

	
	
	
	
	//自检CR95HF  自检完成后，没有检测到CR95，按退出键，退出检测。
	for(Tempx=0;Tempx<50;Tempx++)		{					//进行多次CR95检测，如果没有检测到，重复50次，都不能检测到，报错。
		UART_SendByte(CR95Port,0x55);						//发送CR95命令帧 帧头 0x55
		UART_SendByte(CR95Port,0xaa);						//发送CR95命令帧 帧头 0xaa
		UART_SendByte(CR95Port,0x01);						//发送CR95命令帧 检测并设置通讯协议为ISO15963 0x01
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 数据1，该命令中无效。
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 数据2，该命令中无效。
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 数据3，该命令中无效。
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 数据4，该命令中无效。
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 地址1，该命令中无效。
		UART_SendByte(CR95Port,0xFF);						//发送CR95命令帧 地址2，该命令中无效。
		delay_ms(500);													//延时500ms
		if(ReturnData==1)	{				//有返回数据
			ReturnData=0;
			#ifdef DebugDisplay
				Prints(DisplayPort,"CR95-命令1返回数据为：");
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
			Prints(DisplayPort,"CR95检测正常！\n");
			Prints(DisplayPort,"系统自检完成！\n");
		#endif
	}
	else		{
		display_graphic_16x16_page(3,Empty,Shu,Ju,Yi4,Chang,Empty,Empty,Empty);
		display_graphic_16x16_page(4,Xi,Tong,Zi,Jian,Yi4,Chang,Empty,Empty);
		#ifdef DebugDisplay
			Prints(DisplayPort,"CR95检测异常！\n");
			Prints(DisplayPort,"系统自检异常！按回退键可继续！\n");
		#endif
		for(x=0;x<255;x++)	{			//没有能检测到CR95，进入死循环。检测键盘，等待用户退出
			Key_Push = KEY_Scan();
			if(Key_Push == 3)	{				//按下回退按键   退出循环 继续
				#ifdef DebugDisplay
					Prints(DisplayPort,"系统异常，用户选择继续！\n");
				#endif
				x=254;
			}
			else	{			//没有按下回退键  进入死循环
				x=0;
			}
		}
	}
	delay_ms(5000);
	//系统自检正常后，屏显 选择工作模式 
	System_Status = 1;									//系统状态：选择工作模式
	clear_screen();
	Position = 1;												//光标位置第一行
	display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
	display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
	#ifdef DebugDisplay
		Prints(DisplayPort,"请选择工作模式！\n");
		Prints(DisplayPort,"->  进入数据接收模式！\n");
		Prints(DisplayPort,"    进入数据发送模式！\n");
	#endif
	while(1)	{
			
		switch(System_Status)	{
			case 0:		{//系统自检--应该已经过了，不会出现
				#ifdef DebugDisplay
					Prints(DisplayPort,"系统出错，本段不应该进入！！！\n");
				#endif
				break;
			}
			case 1:		{//选择工作模式
				Key_Push = KEY_Scan();
			  if(Key_Push == 1)			{			//按下方向键
					if(Position == 1)			{								//原光标在第一行，将光标位置改为2，并移动光标
						Position = 2;
						display_graphic_16x16(1,1,Empty);			//清除第一行光标
						display_graphic_16x16(3,1,Triangle);	//第二行显示光标
						#ifdef DebugDisplay
							Prints(DisplayPort,"请选择工作模式！\n");
							Prints(DisplayPort,"    进入数据接收模式！\n");
							Prints(DisplayPort,"->  进入数据发送模式！\n");
						#endif
					}
					else		{															//原光标在第一行，将光标位置改为1，并移动光标
						Position = 1;
						display_graphic_16x16(3,1,Empty);			//清除第2行光标
						display_graphic_16x16(1,1,Triangle);	//第1行显示光标
						#ifdef DebugDisplay
							Prints(DisplayPort,"请选择工作模式！\n");
							Prints(DisplayPort,"->  进入数据接收模式！\n");
							Prints(DisplayPort,"    进入数据发送模式！\n");
						#endif
					}
				}
				else if(Key_Push == 2)		{			//按下进入键
					if(Position == 1)		{									//原光标在第一行，进入数据接收模式，将系统状态改变，并屏显 “数据接收中”
						System_Status = 2;									//系统状态改为数据接收状态
						clear_screen();
						Position=1;
						display_graphic_16x16_page(1,Shu,Ju,Jie,Shou,Zhong,Empty,Empty,Empty);
						#ifdef DebugDisplay
							Prints(DisplayPort,"进入数据接收模式！--等待传输指令或数据！！\n");
						#endif
					}
					else		{															//原光标在第二行，进入数据发送选择模式，将系统状态改变，并显示数据发送选择状态
						System_Status = 3;									//系统状态改为数据发送选择状态
						clear_screen();
						Position=1;
						#ifdef DebugDisplay
							Prints(DisplayPort,"进入数据发送模式--数据发送选择！\n");
						#endif
						if((Send_Type & 0x1) == 0x01)		{//第1类型数据发送
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"-> 第一类数据 发送\n");
							#endif
						}
						else	{	//第1类数据不发送
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"-> 第一类数据 不发送\n");
							#endif
						}
						if((Send_Type & 0x2) == 0x02)		{//第2类型数据发送
							display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第二类数据 发送\n");
							#endif
						}
						else	{	//第2类数据不发送
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第二类数据 不发送\n");
							#endif
						}
						if((Send_Type & 0x4) == 0x04)		{//第3类型数据发送
							display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第三类数据 发送\n");
							#endif
						}
						else	{	//第3类数据不发送
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第三类数据 不发送\n");
							#endif
						}
						if((Send_Type & 0x8) == 0x08)		{//第4类型数据发送
							display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第四类数据 发送\n");
							#endif
						}
						else	{	//第4类数据不发送
							display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"   第四类数据 不发送\n");
							#endif
						}
					}
				}
				else	if(Key_Push == 3)		{			//按下回退按键
					System_Status = 1;									//系统状态：选择工作模式
					clear_screen();
					Position = 1;												//光标位置第一行
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
					#ifdef DebugDisplay
						Prints(DisplayPort,"请选择工作模式！\n");
						Prints(DisplayPort,"    进入数据接收模式！\n");
						Prints(DisplayPort,"->  进入数据发送模式！\n");
					#endif
				}
				Key_Push=0;
				break;
			}//选择工作模式
			case 2:		{//数据接收模式
				if(PC_Instruction==1)		{			//接收到指令1  初始化指令，格式化FLASH,并在屏幕显示
					PC_Instruction=0;						//清除接收指令标志
					Sum=0;							//清除校验和
					clear_screen();
					display_graphic_16x16_page(2,Shou,Dao,Chu1,Shi3,Hua,Zhi,Ling,Empty);			//显示"收到初始化指令"
					display_graphic_16x16_page(3,Chu1,Shi3,Hua,Cun,Chu,Qi,Empty,Empty);				//显示“初始化存储器”
					Flash_Format();
					#ifdef DebugDisplay
						Prints(DisplayPort,"收到指令1--初始化指令！\n");
						Prints(DisplayPort,"初始化完成，返回初始化正确指令！\n");
					#endif
					display_graphic_16x16_page(3,Chu1,Shi3,Hua,Wan,Cheng,Empty,Empty,Empty);	//显示“初始化完成”
					//向PC返回初始化正确指令
					UART_SendByte(DisplayPort,0x7E);
					UART_SendByte(DisplayPort,0x7E);
					UART_SendByte(DisplayPort,0xCD);
					UART_SendByte(DisplayPort,0x03);
					UART_SendByte(DisplayPort,0xCC);
				}
				else if(PC_Instruction==2)	{	//接收到指令2   装订完成指令  对数据进行校验。
					PC_Instruction=0;						//清除接收指令标志
					Sum=0;							//清除校验和
					clear_screen();
					display_graphic_16x16_page(2,Shou,Dao,Zhuang,Ding,Zhi,Ling,Empty,Empty);			//显示"收到装订指令"
					if(((Type1_Len+184-1)/184==Type1_Num) && 
						 ((Type2_Len+184-1)/184==Type2_Num) &&
						 ((Type3_Len+184-1)/184==Type3_Num) &&
						 ((Type4_Len+184-1)/184==Type4_Num))	{	//判断是否数据接收完成   都是否接收到最后一包数据		向PC返回装订正确指令
						#ifdef DebugDisplay
							Prints(DisplayPort,"第一类数据应收到 ");
							PrintHex(DisplayPort,(Type1_Len+184-1)/184);
							Prints(DisplayPort," 包数据，收到 ");
							PrintHex(DisplayPort,Type1_Num);
							Prints(DisplayPort," 包\n");
							Prints(DisplayPort,"第二类数据应收到 ");
							PrintHex(DisplayPort,(Type2_Len+184-1)/184);
							Prints(DisplayPort," 包数据，收到 ");
							PrintHex(DisplayPort,Type2_Num);
							Prints(DisplayPort," 包\n");
							Prints(DisplayPort,"第三类数据应收到 ");
							PrintHex(DisplayPort,(Type3_Len+184-1)/184);
							Prints(DisplayPort," 包数据，收到 ");
							PrintHex(DisplayPort,Type3_Num);
							Prints(DisplayPort," 包\n");
							Prints(DisplayPort,"第四类数据应收到 ");
							PrintHex(DisplayPort,(Type4_Len+184-1)/184);
							Prints(DisplayPort," 包数据，收到 ");
							PrintHex(DisplayPort,Type4_Num);
							Prints(DisplayPort," 包\n");
							Prints(DisplayPort,"返回装订正确指令！\n");
						#endif
						display_graphic_16x16_page(3,Zhuang,Ding,Wan,Cheng,Empty,Empty,Empty,Empty);			//显示"装订完成"
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x07);
						UART_SendByte(DisplayPort,0xD0);
					}
					else	{			//向PC返回装订错误指令
						#ifdef DebugDisplay
							Prints(DisplayPort,"装订错误！返回装订错误指令！\n");
						#endif
						display_graphic_16x16_page(3,Zhuang,Ding,Yi4,Chang,Empty,Empty,Empty,Empty);			//显示"装订异常"
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x08);
						UART_SendByte(DisplayPort,0xD1);
					}
				}
				else if(PC_Instruction == 255){  //接收到数据完成
					PC_Instruction=0;						//清除接收指令标志
					#ifdef DebugDisplay
						Prints(DisplayPort,"接收到一包数据！\n");
					#endif
					//判断数据是否正确
					if(Sum==ReceivedData[199])			//校验和正确
					{
						Sum=0;												//清零校验和
						Temp_DataLen = ReceivedData[7];												//获取该类型数据的长度
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[6];
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[5];
						Temp_DataLen = (Temp_DataLen<<8) + ReceivedData[4];
						Number = ReceivedData[9];															//获取当前包序号
						Number = (Number<<8) + ReceivedData[8];
						if(ReceivedData[3]==1)										//得到的是第一类型数据
						{
							Type1_Len = Temp_DataLen;		//对第一类数据长度进行赋值
							Type1_Num = Number;					//对第1类数据包序号赋值
							#ifdef DebugDisplay
								Prints(DisplayPort,"第一类数据存放地址为：第1页到第6页，每包数据占用1页。\n");
							#endif
							//将第一类型数据存储到对应地址   第一类数据 最大1K字节，最多占用6页  从第1页开始存储  0页256字节保留
							Flash_Write_Page(ReceivedData,Number,200);
						}
						else if(ReceivedData[3]==2)								//得到的是第2类型数据
						{
							Type2_Len = Temp_DataLen;		//对第2类型数据长度赋值
							Type2_Num = Number;					//对第2类数据包序号赋值
							#ifdef DebugDisplay
								Prints(DisplayPort,"第二类数据存放地址为：第7页到第12页，每包数据占用1页。\n");
							#endif
							//将第2类型数据存储到对应地址   第2类数据 最大1K字节，最多占用6页  从第7页开始存储
							Flash_Write_Page(ReceivedData,Number+6,200);
						}
						else if(ReceivedData[3]==3)								//得到的是第3类型数据
						{
							Type3_Len = Temp_DataLen;		//对第3类型数据长度赋值
							Type3_Num = Number;					//对第3类数据包序号赋值
							#ifdef DebugDisplay
								Prints(DisplayPort,"第三类数据存放地址为：第13页到第21页，每包数据占用1页。\n");
							#endif
							//将第3类型数据存储到对应地址   第3类数据 最大1.5k字节，最多占用9页 从第13页开始存储
							Flash_Write_Page(ReceivedData,Number+12,200);
						}
						else if(ReceivedData[3]==4)								//得到的是第4类型数据
						{
							Type4_Len = Temp_DataLen;		//对第4类型数据长度赋值
							Type4_Num = Number;					//对第4类数据包序号赋值
							#ifdef DebugDisplay
								Prints(DisplayPort,"第四类数据存放地址为：第22页到第47页，每包数据占用1页。\n");
							#endif
							//将第4类数据存储到对应地址      第4类数据 最大4.5k字节，最多占用26页 从22页开始存储
							Flash_Write_Page(ReceivedData,Number+21,200);
						}
						#ifdef DebugDisplay
							Prints(DisplayPort,"收到第 ");
						  PrintHex(DisplayPort,ReceivedData[3]);
							Prints(DisplayPort," 类型数据，第 ");
							PrintHex(DisplayPort,Number);
//							PrintShortIntHex(DisplayPort,Number);
							Prints(DisplayPort," 包数据，该类数据总长 ");
							PrintLongInt(DisplayPort,Temp_DataLen);
							Prints(DisplayPort," \n");
							Prints(DisplayPort,"数据校验正确，返回包存储正确指令！\n");
						#endif
						clear_screen();
						display_graphic_16x16_page(2,Shou,Dao,Zheng,Chang,Shu,Ju,Empty,Empty);			//显示"收到正常数据"
						//向PC返回包存储正确指令
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x05);
						UART_SendByte(DisplayPort,0xCE);
					}
					else				//校验和不正确
					{
						Sum=0;												//清零校验和
						#ifdef DebugDisplay
							Prints(DisplayPort,"校验和错误！返回包存储错误指令！\n");
						#endif
						clear_screen();
						display_graphic_16x16_page(2,Shou,Dao,Yi4,Chang,Shu,Ju,Empty,Empty);			//显示"收到异常数据"
						//向PC返回包存储错误指令
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0x7E);
						UART_SendByte(DisplayPort,0xCD);
						UART_SendByte(DisplayPort,0x06);
						UART_SendByte(DisplayPort,0xCF);
					}
				}
				Key_Push = KEY_Scan();
				if(Key_Push == 3)	{				//按下回退按键
					System_Status = 1;									//系统状态：选择工作模式
					clear_screen();
					Position = 1;												//光标位置第一行
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);	
					Type1_Len=0,Type2_Len=0,Type3_Len=0,Type4_Len=0;// 各类型数据总长清零
					Type1_Num=0,Type2_Num=0,Type3_Num=0,Type4_Num=0;//各类型数据接收到的最后一包序号 清零
					Number=0;						//当前包序号清零
					#ifdef DebugDisplay
						Prints(DisplayPort,"请选择工作模式！\n");
						Prints(DisplayPort,"->  进入数据接收模式！\n");
						Prints(DisplayPort,"    进入数据发送模式！\n");
					#endif
				}
				else	{										//其它按键  无反应
					System_Status = 2;
					Position = 1;
				}
				Key_Push=0;
			}//数据接收模式
			case 3:		{//数据发送选择模式
				Key_Push = KEY_Scan();
				if(Key_Push == 1)		{	//按下方向键
					Position++;
					if(Position ==2){		//光标将移到第二行
						display_graphic_16x16(1,1,Empty);			//清除第一行光标
						display_graphic_16x16(3,1,Triangle);	//显示第二行光标
						#ifdef DebugDisplay
							Prints(DisplayPort,"选择第二类数据是否发送！\n");
						#endif
					}
					else if(Position == 3){		//光标将移到第3行
						display_graphic_16x16(3,1,Empty);			//清除第2行光标
						display_graphic_16x16(5,1,Triangle);	//显示第3行光标
						#ifdef DebugDisplay
							Prints(DisplayPort,"选择第三类数据是否发送！\n");
						#endif
					}
					else if(Position == 4){		//光标将移到第4行
						display_graphic_16x16(5,1,Empty);			//清除第3行光标
						display_graphic_16x16(7,1,Triangle);	//显示第4行光标
						#ifdef DebugDisplay
							Prints(DisplayPort,"选择第四类数据是否发送！\n");
						#endif
					}
					else if(Position == 5){		//光标将移到第5行
						#ifdef DebugDisplay
							Prints(DisplayPort,"确认数据发送选择，开始发送数据！\n");
						#endif
						if((Send_Type & 0x2) == 0x02)		{//第2类型数据发送
							display_graphic_16x16_page(1,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						else	{	//第2类数据不发送
							display_graphic_16x16_page(1,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x4) == 0x04)		{//第3类型数据发送
							display_graphic_16x16_page(2,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
						}
						else	{	//第3类数据不发送
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x8) == 0x08)		{//第4类型数据发送
							display_graphic_16x16_page(3,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						else	{	//第4类数据不发送
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						display_graphic_16x16_page(4,Triangle,Shu,Ju,Fa,Song,Empty,Empty,Empty);
					}
					else if(Position ==6){			//光标将移动到第6行  光标返回第一行，显示重置到数据发送选择状态
						#ifdef DebugDisplay
							Prints(DisplayPort,"选择第一类数据是否发送！\n");
						#endif
						Position=1;
						if((Send_Type & 0x1) == 0x01)		{//第1类型数据发送
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
						}
						else	{	//第1类数据不发送
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x2) == 0x02)		{//第2类型数据发送
							display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						else	{	//第2类数据不发送
							display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x4) == 0x04)		{//第3类型数据发送
							display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
						}
						else	{	//第3类数据不发送
							display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
						}
						if((Send_Type & 0x8) == 0x08)		{//第4类型数据发送
							display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
						}
						else	{	//第4类数据不发送
							display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
						}
					}
				}
				else if(Key_Push == 2){	//按下确认键
					if(Position == 1)		{		//光标在第一行，将第一类数据发送与不发送互换，实心五角与空心五角互换
						if((Send_Type & 0x1) == 0x01)		{//	第1类型数据原本为发送  改为不发送，实心五角改为空心
							Send_Type &=  0xFE;						//Send_Type的bit0清零
							display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第一类数据不发送！\n");
							#endif
						}
						else	{//第一类型数据原本为不发送，改为发送，空心五角改为实心五角
							Send_Type |=  0x01;						//Send_Type的bit0置1
							display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第一类数据发送！\n");
							#endif
						}
					}
					else if(Position == 2){		//光标在第2行，将第2类数据发送与不发送互换，实心五角与空心五角互换
						if((Send_Type & 0x02) == 0x02)		{//	第2类型数据原本为发送  改为不发送，实心五角改为空心
							Send_Type &=  0xFD;						//Send_Type的bit1清零
							display_graphic_16x16_page(2,Triangle,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第二类数据不发送！\n");
							#endif
						}
						else	{//第2类型数据原本为不发送，改为发送，空心五角改为实心五角
							Send_Type |=  0x02;						//Send_Type的bit1置1
							display_graphic_16x16_page(2,Triangle,FullStart,Di,Er,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第二类数据发送！\n");
							#endif
						}
					}
					else if(Position ==3){		//光标在第3行，将第3类数据发送与不发送互换，实心五角与空心五角互换
						if((Send_Type & 0x04) == 0x04)		{//	第3类型数据原本为发送  改为不发送，实心五角改为空心
							Send_Type &=  0xFB;						//Send_Type的bit2清零
							display_graphic_16x16_page(3,Triangle,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第三类数据不发送！\n");
							#endif
						}
						else	{//第2类型数据原本为不发送，改为发送，空心五角改为实心五角
							Send_Type |=  0x04;						//Send_Type的bit1置1
							display_graphic_16x16_page(3,Triangle,FullStart,Di,San,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第三类数据发送！\n");
							#endif
						}
					}
					else if(Position ==4){		//光标在第4行，将第4类数据发送与不发送互换，实心五角与空心五角互换
						if((Send_Type & 0x08) == 0x08)		{//	第4类型数据原本为发送  改为不发送，实心五角改为空心
							Send_Type &=  0xF7;						//Send_Type的bit3清零
							display_graphic_16x16_page(4,Triangle,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第四类数据不发送！\n");
							#endif
						}
						else	{//第2类型数据原本为不发送，改为发送，空心五角改为实心五角
							Send_Type |=  0x08;						//Send_Type的bit3置1
							display_graphic_16x16_page(4,Triangle,FullStart,Di,Si,Lei,Shu,Ju,Empty);
							#ifdef DebugDisplay
								Prints(DisplayPort,"第四类数据发送！\n");
							#endif
						}
					}
					else if(Position ==5){		//光标在第5行，进入数据开始发送   屏显“数据发送中”
						System_Status = 4;
						Position = 1;
						clear_screen();
						display_graphic_16x16_page(1,Shu,Ju,Fa,Song,Zhong,Empty,Empty,Empty);
						#ifdef DebugDisplay
							Prints(DisplayPort,"发送数据列表！！！\n");
							if((Send_Type & 0x01) == 0x01)		{
								Prints(DisplayPort,"  第1类数据发送！\n");
							}
							else	{
								Prints(DisplayPort,"  第1类数据不发送！\n");
							}
							if((Send_Type & 0x02) == 0x02)		{
								Prints(DisplayPort,"  第2类数据发送！\n");
							}
							else	{
								Prints(DisplayPort,"  第2类数据不发送！\n");
							}
							if((Send_Type & 0x04) == 0x04)		{
								Prints(DisplayPort,"  第3类数据发送！\n");
							}
							else	{
								Prints(DisplayPort,"  第3类数据不发送！\n");
							}
							if((Send_Type & 0x08) == 0x08)		{
								Prints(DisplayPort,"  第4类数据发送！\n");
							}
							else	{
								Prints(DisplayPort,"  第4类数据不发送！\n");
							}
							Prints(DisplayPort,"开始发送选择数据！\n");
						#endif
					}
				}
				else if (Key_Push ==3){	//按下回退键   返回选择工作模式
					System_Status = 1;									//系统状态：选择工作模式
					Send_Type=0x0F;											//默认发送所有数据
					clear_screen();
					Position = 1;												//光标位置第一行
					display_graphic_16x16_page(1,Triangle,Jin,Ru,Shu,Ju,Jie,Shou,Empty);
					display_graphic_16x16_page(2,Empty,Jin,Ru,Shu,Ju,Fa,Song,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"请选择工作模式！\n");
						Prints(DisplayPort,"->  进入数据接收模式！\n");
						Prints(DisplayPort,"    进入数据发送模式！\n");
					#endif
				}
				break;
			}
			case 4:{	//数据发送中		不按照发送选择进行发送，实际发送所有存储区内数据，不发送的可以不读取嘛！
				y=0;																			//作为等待KFB返回超时控制
				WriteNum = 0;															//写入计数清零
				for(Tempx=1;Tempx<39;Tempx++)		{					//数据存储区域--第一页到第38页
					ReturnData = 0;						//KFB返回状态清零
					//将1页存储区数据读取200个到缓存ReceivedData[]中
					Flash_Read(ReceivedData,Tempx,200);
					for(WriteFrameNum=0;WriteFrameNum<46;WriteFrameNum++)		{
						WriteNum++;
						for(Retry=0;Retry<10;Retry++)		{		//KFB返回数据与写入数据不同，重试次数
							USART_KFB_Write(WriteFrameNum,ReceivedData[WriteFrameNum*4+10],ReceivedData[WriteFrameNum*4+11],ReceivedData[WriteFrameNum*4+12],ReceivedData[WriteFrameNum*4+13]);
							y=0;
							while(ReturnData == 0)	{				//等待KFB返回数据		写入时返回数据不重要，直接无视
								y++;
								delay_ms(100);
								if(y>100)		{									//超过1s无返回，直接报错，进入死循环，屏显--数据发送异常
									#ifdef DebugDisplay
										Prints(DisplayPort,"CR95写入返回超时，系统错误！！！重启！！！\n");
									#endif
									clear_screen();
									display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
								}
							}
							ReturnData = 0;									//KFB返回状态清零
							#ifdef DebugDisplay
								Prints(DisplayPort,"CR95-写入命令返回数据为：");
								for(x=0;x<6;x++){
									PrintHex(DisplayPort,Response[x]);
									Prints(DisplayPort,"  ");
								}
								Prints(DisplayPort,"\n");
							#endif
							USART_KFB_Read(WriteFrameNum);							//读取KFB的WriteFrameNum地址值
							y=0;
							while(ReturnData == 0)	{				//等待KFB返回数据   读取数据进行比较，为写入数据，继续写入，不是写入数据，重试10次
								y++;
								delay_ms(10);
								if(y>100)		{									//超过1s无返回，直接报错，进入死循环，屏显--数据发送异常
									#ifdef DebugDisplay
										Prints(DisplayPort,"CR95读取返回超时，系统错误！！！重启！！！\n");
									#endif
									clear_screen();
									display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
									while(1);
								}
							}
							ReturnData = 0;									//KFB返回状态清零
							#ifdef DebugDisplay
								Prints(DisplayPort,"CR95-读取命令返回数据为：");
								for(x=0;x<6;x++){
									PrintHex(DisplayPort,Response[x]);
									Prints(DisplayPort,"  ");
								}
								Prints(DisplayPort,"\n");
							#endif
							if((Response[2] == ReceivedData[WriteFrameNum*4+10]) &&
								 (Response[3] == ReceivedData[WriteFrameNum*4+11]) &&
								 (Response[4] == ReceivedData[WriteFrameNum*4+12]) &&
								 (Response[5] == ReceivedData[WriteFrameNum*4+13]) )	{	//KFB返回数据与写入数相同，直接跳出重试步骤。
								Retry=20;						//设置Retry值超范围，不再重试							
							}
							else	{							//KFB返回数据与写入数据不同，重试。
								;
							}
							if(Retry > 15)		{		//数据写入正确。
//								#ifdef DebugDisplay
									PrintShortIntHex(DisplayPort,WriteNum);
									Prints(DisplayPort,"数据写入正确！\n");
//								#endif
								clear_screen();
								display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Zheng,Chang,Empty,Empty);
							}
							else						{			//数据写入不正确。进入死循环，屏显--数据发送异常
								#ifdef DebugDisplay
									Prints(DisplayPort,"数据写入异常，多次重试都异常，重新启动！\n");
								#endif
								clear_screen();
								display_graphic_16x16_page(2,Shu,Ju,Fa,Song,Yi4,Chang,Empty,Empty);
							}
						}
					}
				}
				//数据全部发送完成，进入数据发送选择状态！
				System_Status = 3;									//系统状态改为数据发送选择状态
				clear_screen();
				Position=1;
				#ifdef DebugDisplay
					Prints(DisplayPort,"数据发送完成，进入数据发送选择模式！\n");
				#endif
				if((Send_Type & 0x1) == 0x01)		{//第1类型数据发送
					display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"-> 第一类数据 发送\n");
					#endif
				}
				else	{	//第1类数据不发送
					display_graphic_16x16_page(1,Triangle,EmptyStart,Di,Yi,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"-> 第一类数据 不发送\n");
					#endif
				}
				if((Send_Type & 0x2) == 0x02)		{//第2类型数据发送
					display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第二类数据 发送\n");
					#endif
				}
				else	{	//第2类数据不发送
					display_graphic_16x16_page(2,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第二类数据 不发送\n");
					#endif
				}
				if((Send_Type & 0x4) == 0x04)		{//第3类型数据发送
					display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第三类数据 发送\n");
					#endif
				}
				else	{	//第3类数据不发送
					display_graphic_16x16_page(3,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第三类数据 不发送\n");
					#endif
				}
				if((Send_Type & 0x8) == 0x08)		{//第4类型数据发送
					display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第四类数据 发送\n");
					#endif
				}
				else	{	//第4类数据不发送
					display_graphic_16x16_page(4,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
					#ifdef DebugDisplay
						Prints(DisplayPort,"   第四类数据 不发送\n");
					#endif
				}
			}
		}
	}	 
}

