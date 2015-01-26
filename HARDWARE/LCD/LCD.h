#ifndef _LCD_12864_H
#define _LCD_12864_H
#include "stm32f10x.h"
#include "My_config.h"
//Mini STM32开发板
/*字库-----使用PCtoLCD2002制作  列行式 逆向*/
/*"入",0*/
extern unsigned char Ru[];
/*"进",0*/
extern unsigned char Jin[];
/*"系",0*/
extern unsigned char Xi[];
/*"统",1*/
extern unsigned char Tong[];
/*"模"*/
extern unsigned char Mo[];
/*"式"*/
extern unsigned char Shi[];
/*"接"*/
extern unsigned char Jie[];
/*"收"*/
extern unsigned char Shou[];
/*"发"*/					
extern unsigned char Fa[];
/*"送"*/
extern unsigned char Song[];		
/*"第"*/
extern unsigned char Di[];
/*"一"*/
extern unsigned char Yi[];
/*"二"*/
extern unsigned char Er[];
/*"三"*/
extern unsigned char San[];
/*"四"*/
extern unsigned char Si[];	
/*"类"*/
extern unsigned char Lei[];		
/*"数"*/
extern unsigned char Shu[];
/*"据"*/
extern unsigned char Ju[];
/*"中"*/
extern unsigned char Zhong[];
/*" "*/
extern unsigned char Empty[];//清空一个汉字位置
/*"空心五角形"*/
extern unsigned char EmptyStart[];	
/*"实心五角形"*/
extern unsigned char FullStart[];
/*"三角形"*/
extern unsigned char Triangle[];

/********************************************************************/
/*LCD端口定义*/
//分开的两板定义：LCD_CS--PB9  	LCD_RESET--PB8  	LCD_RS--PB7  	LCD_SID--PB6  	LCD_SCK--PB5
//合起来定义V1.1：LCD_CS--PB14  LCD_RESET--PB13  	LCD_RS--PB12  LCD_SID--PB11  	LCD_SCK--PB10
#define BOARD_VERSION
#ifdef BOARD_VERSION				//V1.1定义
	#define LCD_CS_High  			GPIO_SetBits(GPIOB, GPIO_Pin_14 )				// PB14 LCD的CS
	#define LCD_CS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_14 )			// PB14 LCD的CS
	#define LCD_RST_High 			GPIO_SetBits(GPIOB, GPIO_Pin_13 )				// PB13 LCD的RESET
	#define LCD_RST_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_13 )			// PB31 LCD的RESET
	#define LCD_RS_High	 			GPIO_SetBits(GPIOB, GPIO_Pin_12 )				// PB12 LCD的RS
	#define LCD_RS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_12 )			// PB12 LCD的RS
	#define LCD_SID_High 			GPIO_SetBits(GPIOB, GPIO_Pin_11 )				// PB11 LCD的SID	
	#define LCD_SID_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_11 )			// PB11 LCD的SID
	#define LCD_SCK_High 			GPIO_SetBits(GPIOB, GPIO_Pin_10 )				// PB10 LCD的SCLK
	#define LCD_SCK_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_10 )			// PB10 LCD的SCLK
#else												//旧版定义
	#define LCD_CS_High  			GPIO_SetBits(GPIOB, GPIO_Pin_9 )				// PB9 LCD的CS
	#define LCD_CS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_9 )			// PB9 LCD的CS
	#define LCD_RST_High 			GPIO_SetBits(GPIOB, GPIO_Pin_8 )				// PB8 LCD的RESET
	#define LCD_RST_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_8 )			// PB8 LCD的RESET
	#define LCD_RS_High	 			GPIO_SetBits(GPIOB, GPIO_Pin_7 )				// PB7 LCD的RS
	#define LCD_RS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_7 )			// PB7 LCD的RS
	#define LCD_SID_High 			GPIO_SetBits(GPIOB, GPIO_Pin_6 )				// PB6 LCD的SID	
	#define LCD_SID_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_6 )			// PB6 LCD的SID
	#define LCD_SCK_High 			GPIO_SetBits(GPIOB, GPIO_Pin_5 )				// PB5 LCD的SCLK
	#define LCD_SCK_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_5 )			// PB5 LCD的SCLK
#endif

void LCD12864_GPIO_Config(void);					//端口初始化
void initial_lcd(void);										//LCD初始化
void transfer_command_lcd(int data1)  ;		//写指令到LCD模块
void transfer_data_lcd(u8 data1);					//写数据到LCD模块
void lcd_address(u8 page,u8 column);			//设置显示地址
void clear_screen(void);									//清屏
void display_graphic_16x16(u8 page,u8 column, u8 *dp);			//显示16*16图形
void display_graphic_16x16_page(u8 page,u8 *Word1,u8 *Word2,u8 *Word3,u8 *Word4,u8 *Word5,u8 *Word6,u8 *Word7,u8 *Word8);//显示一行16*16图形，

void display_string_5x8(u8 page,u8 column,u8 *text);
void display_GB2312_string(u8 page,u8 column,u8 *text);
void get_and_write_5x8(u32 fontaddr,u8 page,u8 column) ;
void get_and_write_8x16(u32 fontaddr,u8 page,u8 column);
void get_and_write_16x16(u32 fontaddr,u8 page,u8 column);
void send_command_to_ROM( u8 datu );
void display_graphic_5x8(u8 page,u8 column, u8 *dp);
//void display_graphic_8x16(u8 page,u8 col, u8 *dp);
//void display_128x64(u8 *dp);
//void lcd_address(unsigned int page,unsigned int column);
u8 display_menu(u8 menu);																							//显示菜单
u8 display_Error(void);					//多次尝试不能写入数据后显示
u8 display_Right(void);					//数据写入完成后显示


#endif
