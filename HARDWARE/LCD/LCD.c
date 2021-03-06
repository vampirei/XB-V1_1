#include "LCD.h"
#include "stm32f10x.h"
#include "usart.h"

u8 i;
extern __IO	u8 SYSTEM_STATUS;
extern u8 Send_Select;	

//LCD12864G IO口配置
void LCD12864_GPIO_Config(void)					
{
	GPIO_InitTypeDef GPIO_InitStructure;													//端口结构体定义
	/*LCD端口配置*/
	//PB.5:LCD_SCK	PB.6:LCD_SID	PB.7:LCD_RS 	PB.8:LCD_RESET	PB.9:LCD_CS	
	//分开的两板定义：LCD_CS--PB9  	LCD_RESET--PB8  	LCD_RS--PB7  	LCD_SID--PB6  	LCD_SCK--PB5
	//合起来定义V1.1：LCD_CS--PB14  LCD_RESET--PB13  	LCD_RS--PB12  LCD_SID--PB11  	LCD_SCK--PB10

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);					//端口B时钟允许
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12| GPIO_Pin_11 | GPIO_Pin_10;	 //使用端口指定 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						//端口速度指定
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 						//端口模式--推挽
	GPIO_Init(GPIOB,&GPIO_InitStructure);												//初始化端口
	LCD_CS_High;																								//设置CS为高
	LCD_SID_Low;																								//LCD_SID为�
	LCD_RST_Low;																								//设置LCD复位
	delay_ms(10);																								//延时10ms
	LCD_RST_High;																								//LCD复位结束
}
//LCD模块初始化
void initial_lcd()
{
	LCD12864_GPIO_Config();
	LCD_RST_Low;								//LCD复位
	delay_ms(10);								//延时
	LCD_RST_High;		    				//复位结束
	delay_ms(10);        				//延时
	transfer_command_lcd(0xe2);	 //发送命令 0xE2  软复位
	delay_ms(5);
	transfer_command_lcd(0x2c);  //升压步骤1
	delay_ms(50);	
	transfer_command_lcd(0x2e);  //升压步骤2
	delay_ms(50);
	transfer_command_lcd(0x2f);  //升压步骤3
	delay_ms(5);
	transfer_command_lcd(0x23);  //粗调对比度 设置范围0x20~0x27
	transfer_command_lcd(0x81);  //微调对比度
	transfer_command_lcd(0x28);  //微调对比度的值 设置范围0x00~0x3f
	transfer_command_lcd(0xa2);  //1/9偏压比(bias)
	transfer_command_lcd(0xc8);  //行扫描顺序：从上到下
	transfer_command_lcd(0xa0);  //列扫描顺序：从左到右
	transfer_command_lcd(0x40);  //起始行：第一行
	transfer_command_lcd(0xaf);  //开始显示
}

//写指令到LCD模块
void transfer_command_lcd(int data1)   
{
	char i;
	LCD_CS_Low;
	LCD_RS_Low;
	for(i=0;i<8;i++)
  {
		LCD_SCK_Low; 
		if(data1&0x80) LCD_SID_High;
		else LCD_SID_Low;
		LCD_SCK_High;
	 	data1=data1<<=1;
  }
	LCD_CS_High;
}

//写数据到LCD模块
void transfer_data_lcd(u8 data1)
{
	char i;
	LCD_CS_Low;
	LCD_RS_High;
	for(i=0;i<8;i++)
	{
		LCD_SCK_Low;
		if(data1&0x80) LCD_SID_High;
		else LCD_SID_Low;
		LCD_SCK_High;
		data1=data1<<=1;
	}
	LCD_CS_High;
}

//设置地址
void lcd_address(u8 page,u8 column)
{
	column=column-0x01;
	transfer_command_lcd(0xb0+page-1);   						//设置页地址，8行为一页，全屏共64行，共计8页
	transfer_command_lcd(0x10+(column>>4&0x0f));		//设置列地址高四位
	transfer_command_lcd(column & 0x0f);						//设置列地址低四位
}

//清屏
void clear_screen()
{
	unsigned char i,j;
	for(i=0;i<9;i++)
	{
		transfer_command_lcd(0xb0+i);
		transfer_command_lcd(0x10);
		transfer_command_lcd(0x00);
		for(j=0;j<132;j++)
		{
		  	transfer_data_lcd(0x00);
		}
	}
}

//显示128x64点阵图像
//void display_128x64(u8 *dp)
//{
//	u16 i,j;
//	
//	for(j=0;j<8;j++)
//	{
//		lcd_address(j+1,1);
//		for (i=0;i<128;i++)
//		{	
//			transfer_data_lcd(*dp);					//????LCD,?????8???????????1
//			dp++;
//		}
//	}
//}

//显示16x16点阵图像
void display_graphic_16x16(u8 page,u8 column,u8 *dp)
{
	u16 i,j;
	for(j=0;j<2;j++)
	{
		lcd_address(page+j,column);
		for (i=0;i<16;i++)
		{	
			transfer_data_lcd(*dp);					//????LCD,?????8???????????1
			dp++;
		}
	}
}

//显示8x16点阵图像
//void display_graphic_8x16(u8 page,u8 col, u8 *dp)
//{
//	u16 i,j;
//	for(j=0;j<2;j++)
//	{
//		lcd_address(page+j,col);
//		for (i=0;i<8;i++)
//		{	
//			transfer_data_lcd(*dp);					//????LCD,?????8???????????1
//			dp++;
//		}
//	}
//}

//显示5X8点阵图像
//void display_graphic_5x8(u8 page,u8 column,u8 *dp)
//{
//	u16 i;
//	lcd_address(page,column);
//	for (i=0;i<6;i++)
//	{	
//		transfer_data_lcd(*dp);
//		dp++;
//	}
//}

//****************************************************************


//u32  fontaddr=0;
//void display_GB2312_string(u8 page,u8 column,u8 *text)
//{
//	u8 i= 0;			
//	while((text[i]>0x00))
//	{
//		if(((text[i]>=0xb0) &&(text[i]<=0xf7))&&(text[i+1]>=0xa1))
//		{						
//			//????(GB2312)????????IC????????????:
//			//Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32+ BaseAdd;BaseAdd=0
//			//????8???????????,????????
//			fontaddr = (text[i]- 0xb0)*94; 
//			fontaddr += (text[i+1]-0xa1)+846;
//			fontaddr = (u32)(fontaddr*32);

////			get_and_write_16x16(fontaddr,page,column);	 //????????????????(page,column)???
//			i+=2;
//			column+=16;
//		}
//		else if(((text[i]>=0xa1) &&(text[i]<=0xa3))&&(text[i+1]>=0xa1))
//		{						
//			//????(GB2312)15x16??????????IC????????????:
//			//Address = ((MSB - 0xa1) * 94 + (LSB - 0xA1))*32+ BaseAdd;BaseAdd=0
//			//????8???????????,????????
//			fontaddr = (text[i]- 0xa1)*94; 
//			fontaddr += (text[i+1]-0xa1);
//			fontaddr = (u32)(fontaddr*32);
//	
////			get_and_write_16x16(fontaddr,page,column);	 //????????????????(page,column)???
//			i+=2;
//			column+=16;
//		}
//		else if((text[i]>=0x20) &&(text[i]<=0x7e))	
//		{									
//			fontaddr = (text[i]- 0x20);
//			fontaddr = (unsigned long)(fontaddr*16);
//			fontaddr = (unsigned long)(fontaddr+0x3cf80);			

////			get_and_write_8x16(fontaddr,page,column);	 //????????????????(page,column)???
//			i+=1;
//			column+=8;
//		}
//		else
//			i++;	
//	}
//	
//}

//void display_string_5x8(u8 page,u8 column,u8 *text)
//{
//	unsigned char i= 0;
//	while((text[i]>0x00))
//	{
//		
//		if((text[i]>=0x20) &&(text[i]<=0x7e))	
//		{						
//			fontaddr = (text[i]- 0x20);
//			fontaddr = (unsigned long)(fontaddr*8);
//			fontaddr = (unsigned long)(fontaddr+0x3bfc0);			


////			get_and_write_5x8(fontaddr,page,column);	 //????????????????(page,column)???

//			i+=1;
//			column+=6;
//		}
//		else
//		i++;	
//	}
//}



//显示错误退出--在多次尝试不能写入数据后显示
u8 display_Error(void)
{
//	u8 hang,lie;
	clear_screen();
	display_graphic_16x16_page(1,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart);
//	hang=1;	lie=1;
//	display_graphic_16x16(hang,lie,EmptyStart);				//显示空心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//显示空心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//显示空心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//显示空心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//显示空心五角星
	return 0;
}

//显示正确退出--数据写入完成后显示
u8 display_Right(void )
{
//	u8 hang,lie;
	clear_screen();
	display_graphic_16x16_page(1,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart);
//	hang=1;	lie=1;
//	display_graphic_16x16(hang,lie,FullStart);				//显示实心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//显示实心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//显示实心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//显示实心五角星
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//显示实心五角星
	return 0;
}

//显示菜单
u8 display_menu(u8 STATUS)
{
//	u8 hang,lie;
	clear_screen();
	clear_screen();
	switch (STATUS)
	{
		case 0:	//显示模式菜单：默认选择三角在第一个选项（进入接收模式）
		{
			display_graphic_16x16_page(1,Triangle,Jin,Ru,Jie,Shou,Empty,Empty,Empty);
//			hang=1;	lie=1;																		//第一行
//			display_graphic_16x16(hang,lie,Triangle);					//显示选择三角  默认位置
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jin);							//显示“进”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ru);								//显示“入”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jie);							//显示“接”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shou);							//显示“收”
			display_graphic_16x16_page(2,Empty,Jin,Ru,Fa,Song,Empty,Empty,Empty);
//			hang=hang+2;																			//第二行
//			lie=1;lie=lie+16;
//			display_graphic_16x16(hang,lie,Jin);							//显示“进”						
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ru);								//显示“入”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Fa);								//显示“发”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Song);							//显示“送”
			break;
		}
		case 1:	//显示接收数据
		{
			display_graphic_16x16_page(1,Shu,Ju,Jie,Shou,Zhong,Empty,Empty,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Shu);							//显示“数”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);								//显示“据”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jie);							//显示“接”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shou);							//显示“收”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Zhong);						//显示“中”
			break;
		}
		case 2:	//显示发送数据菜单  未选择为空心五角星 默认为全部选中
		{
			display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Triangle);					//显示 选择三角形
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//显示 实心五角星
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//显示 “第一类数据”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Yi);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
//			hang=3;	lie=1;																		//第二行
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//显示 实心五角星
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//显示 “第二类数据”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Er);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
//			hang=5;	lie=1;																		//第三行
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//显示 实心五角星
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//显示 “第三类数据”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,San);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
//			hang=7;	lie=1;																		//第四行
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//显示 实心五角星
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//显示  “第四类数据”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Si);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			break;
		}
		case 3:	//显示发送数据
		{
			display_graphic_16x16_page(1,Shu,Ju,Fa,Song,Zhong,Empty,Empty,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Shu);						//显示  “数据发送中”
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Fa);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Song);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Zhong);
			break;
		}
		case 10:	//上移显示发送数据菜单，增加确定
		{
			if((Send_Select & 0x02)==0x02)										//如果第二类数据发送选择，显示：是心五角星+“第二类数据”
			{
				display_graphic_16x16_page(1,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
//				hang=1;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,FullStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Er);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			else																							//如果第二类数据选择不发送，显示：空心五角星+“第二类数据”
			{
				display_graphic_16x16_page(1,Empty,EmptyStart,Di,Er,Lei,Shu,Ju,Empty);
//				hang=1;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,EmptyStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Er);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			if((Send_Select & 0x04)==0x04)
			{
				display_graphic_16x16_page(2,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
//				hang=3;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,FullStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,San);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			else
			{
				display_graphic_16x16_page(2,Empty,EmptyStart,Di,San,Lei,Shu,Ju,Empty);
//				hang=3;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,EmptyStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,San);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			if((Send_Select & 0x08)==0x08)
			{
				display_graphic_16x16_page(3,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
//				hang=5;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,FullStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Si);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			else
			{
				display_graphic_16x16_page(3,Empty,EmptyStart,Di,Si,Lei,Shu,Ju,Empty);
//				hang=5;	lie=1;
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,EmptyStart);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Di);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Si);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Lei);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Shu);
//				lie=lie+16;
//				display_graphic_16x16(hang,lie,Ju);
			}
			display_graphic_16x16_page(3,Triangle,Fa,Song,Empty,Empty,Empty,Empty,Empty);
//			hang=7;	lie=1;
//			display_graphic_16x16(hang,lie,Triangle);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Fa);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Song);
			break;
		}
		default:
		{
			;
		}
	}
	return 0;
}

//显示一整行数据
void display_graphic_16x16_page(u8 page,u8 *Word1,u8 *Word2,u8 *Word3,u8 *Word4,u8 *Word5,u8 *Word6,u8 *Word7,u8 *Word8)
{
	u8 hang,lie,i;
	u8 Display[32];
	hang=page;
	if(hang == 1)
		hang = 1;
	else if(hang == 2)
		hang = 3;
	else if(hang == 3)
		hang = 5;
	else if(hang == 4)
		hang = 7;
	lie=1;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word1;
		Word1++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word2;
		Word2++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word3;
		Word3++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word4;
		Word4++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word5;
		Word5++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word6;
		Word6++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word7;
		Word7++;
	}
	display_graphic_16x16(hang,lie,Display);
	lie=lie+16;
	for(i=0;i<32;i++)
	{
		Display[i]=*Word8;
		Word8++;
	}
	display_graphic_16x16(hang,lie,Display);
}









