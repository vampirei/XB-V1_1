#ifndef _LCD_12864_H
#define _LCD_12864_H
#include "stm32f10x.h"
#include "My_config.h"
//Mini STM32������
/*�ֿ�-----ʹ��PCtoLCD2002����  ����ʽ ����*/
/*"��",0*/
extern unsigned char Ru[];
/*"��",0*/
extern unsigned char Jin[];
/*"ϵ",0*/
extern unsigned char Xi[];
/*"ͳ",1*/
extern unsigned char Tong[];
/*"ģ"*/
extern unsigned char Mo[];
/*"ʽ"*/
extern unsigned char Shi[];
/*"��"*/
extern unsigned char Jie[];
/*"��"*/
extern unsigned char Shou[];
/*"��"*/					
extern unsigned char Fa[];
/*"��"*/
extern unsigned char Song[];		
/*"��"*/
extern unsigned char Di[];
/*"һ"*/
extern unsigned char Yi[];
/*"��"*/
extern unsigned char Er[];
/*"��"*/
extern unsigned char San[];
/*"��"*/
extern unsigned char Si[];	
/*"��"*/
extern unsigned char Lei[];		
/*"��"*/
extern unsigned char Shu[];
/*"��"*/
extern unsigned char Ju[];
/*"��"*/
extern unsigned char Zhong[];
/*" "*/
extern unsigned char Empty[];//���һ������λ��
/*"���������"*/
extern unsigned char EmptyStart[];	
/*"ʵ�������"*/
extern unsigned char FullStart[];
/*"������"*/
extern unsigned char Triangle[];

/********************************************************************/
/*LCD�˿ڶ���*/
//�ֿ������嶨�壺LCD_CS--PB9  	LCD_RESET--PB8  	LCD_RS--PB7  	LCD_SID--PB6  	LCD_SCK--PB5
//����������V1.1��LCD_CS--PB14  LCD_RESET--PB13  	LCD_RS--PB12  LCD_SID--PB11  	LCD_SCK--PB10
#define BOARD_VERSION
#ifdef BOARD_VERSION				//V1.1����
	#define LCD_CS_High  			GPIO_SetBits(GPIOB, GPIO_Pin_14 )				// PB14 LCD��CS
	#define LCD_CS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_14 )			// PB14 LCD��CS
	#define LCD_RST_High 			GPIO_SetBits(GPIOB, GPIO_Pin_13 )				// PB13 LCD��RESET
	#define LCD_RST_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_13 )			// PB31 LCD��RESET
	#define LCD_RS_High	 			GPIO_SetBits(GPIOB, GPIO_Pin_12 )				// PB12 LCD��RS
	#define LCD_RS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_12 )			// PB12 LCD��RS
	#define LCD_SID_High 			GPIO_SetBits(GPIOB, GPIO_Pin_11 )				// PB11 LCD��SID	
	#define LCD_SID_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_11 )			// PB11 LCD��SID
	#define LCD_SCK_High 			GPIO_SetBits(GPIOB, GPIO_Pin_10 )				// PB10 LCD��SCLK
	#define LCD_SCK_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_10 )			// PB10 LCD��SCLK
#else												//�ɰ涨��
	#define LCD_CS_High  			GPIO_SetBits(GPIOB, GPIO_Pin_9 )				// PB9 LCD��CS
	#define LCD_CS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_9 )			// PB9 LCD��CS
	#define LCD_RST_High 			GPIO_SetBits(GPIOB, GPIO_Pin_8 )				// PB8 LCD��RESET
	#define LCD_RST_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_8 )			// PB8 LCD��RESET
	#define LCD_RS_High	 			GPIO_SetBits(GPIOB, GPIO_Pin_7 )				// PB7 LCD��RS
	#define LCD_RS_Low   			GPIO_ResetBits(GPIOB, GPIO_Pin_7 )			// PB7 LCD��RS
	#define LCD_SID_High 			GPIO_SetBits(GPIOB, GPIO_Pin_6 )				// PB6 LCD��SID	
	#define LCD_SID_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_6 )			// PB6 LCD��SID
	#define LCD_SCK_High 			GPIO_SetBits(GPIOB, GPIO_Pin_5 )				// PB5 LCD��SCLK
	#define LCD_SCK_Low  			GPIO_ResetBits(GPIOB, GPIO_Pin_5 )			// PB5 LCD��SCLK
#endif

void LCD12864_GPIO_Config(void);					//�˿ڳ�ʼ��
void initial_lcd(void);										//LCD��ʼ��
void transfer_command_lcd(int data1)  ;		//дָ�LCDģ��
void transfer_data_lcd(u8 data1);					//д���ݵ�LCDģ��
void lcd_address(u8 page,u8 column);			//������ʾ��ַ
void clear_screen(void);									//����
void display_graphic_16x16(u8 page,u8 column, u8 *dp);			//��ʾ16*16ͼ��
void display_graphic_16x16_page(u8 page,u8 *Word1,u8 *Word2,u8 *Word3,u8 *Word4,u8 *Word5,u8 *Word6,u8 *Word7,u8 *Word8);//��ʾһ��16*16ͼ�Σ�

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
u8 display_menu(u8 menu);																							//��ʾ�˵�
u8 display_Error(void);					//��γ��Բ���д�����ݺ���ʾ
u8 display_Right(void);					//����д����ɺ���ʾ


#endif
