#include "LCD.h"
#include "stm32f10x.h"
#include "usart.h"

u8 i;
extern __IO	u8 SYSTEM_STATUS;
extern u8 Send_Select;	

//LCD12864G IO¿ÚÅäÖÃ
void LCD12864_GPIO_Config(void)					
{
	GPIO_InitTypeDef GPIO_InitStructure;													//¶Ë¿Ú½á¹¹Ìå¶¨Òå
	/*LCD¶Ë¿ÚÅäÖÃ*/
	//PB.5:LCD_SCK	PB.6:LCD_SID	PB.7:LCD_RS 	PB.8:LCD_RESET	PB.9:LCD_CS	
	//·Ö¿ªµÄÁ½°å¶¨Òå£ºLCD_CS--PB9  	LCD_RESET--PB8  	LCD_RS--PB7  	LCD_SID--PB6  	LCD_SCK--PB5
	//ºÏÆğÀ´¶¨ÒåV1.1£ºLCD_CS--PB14  LCD_RESET--PB13  	LCD_RS--PB12  LCD_SID--PB11  	LCD_SCK--PB10

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);					//¶Ë¿ÚBÊ±ÖÓÔÊĞí
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12| GPIO_Pin_11 | GPIO_Pin_10;	 //Ê¹ÓÃ¶Ë¿ÚÖ¸¶¨ 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						//¶Ë¿ÚËÙ¶ÈÖ¸¶¨
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 						//¶Ë¿ÚÄ£Ê½--ÍÆÍì
	GPIO_Init(GPIOB,&GPIO_InitStructure);												//³õÊ¼»¯¶Ë¿Ú
	LCD_CS_High;																								//ÉèÖÃCSÎª¸ß
	LCD_SID_Low;																								//LCD_SIDÎªµ
	LCD_RST_Low;																								//ÉèÖÃLCD¸´Î»
	delay_ms(10);																								//ÑÓÊ±10ms
	LCD_RST_High;																								//LCD¸´Î»½áÊø
}
//LCDÄ£¿é³õÊ¼»¯
void initial_lcd()
{
	LCD12864_GPIO_Config();
	LCD_RST_Low;								//LCD¸´Î»
	delay_ms(10);								//ÑÓÊ±
	LCD_RST_High;		    				//¸´Î»½áÊø
	delay_ms(10);        				//ÑÓÊ±
	transfer_command_lcd(0xe2);	 //·¢ËÍÃüÁî 0xE2  Èí¸´Î»
	delay_ms(5);
	transfer_command_lcd(0x2c);  //ÉıÑ¹²½Öè1
	delay_ms(50);	
	transfer_command_lcd(0x2e);  //ÉıÑ¹²½Öè2
	delay_ms(50);
	transfer_command_lcd(0x2f);  //ÉıÑ¹²½Öè3
	delay_ms(5);
	transfer_command_lcd(0x23);  //´Öµ÷¶Ô±È¶È ÉèÖÃ·¶Î§0x20~0x27
	transfer_command_lcd(0x81);  //Î¢µ÷¶Ô±È¶È
	transfer_command_lcd(0x28);  //Î¢µ÷¶Ô±È¶ÈµÄÖµ ÉèÖÃ·¶Î§0x00~0x3f
	transfer_command_lcd(0xa2);  //1/9Æ«Ñ¹±È(bias)
	transfer_command_lcd(0xc8);  //ĞĞÉ¨ÃèË³Ğò£º´ÓÉÏµ½ÏÂ
	transfer_command_lcd(0xa0);  //ÁĞÉ¨ÃèË³Ğò£º´Ó×óµ½ÓÒ
	transfer_command_lcd(0x40);  //ÆğÊ¼ĞĞ£ºµÚÒ»ĞĞ
	transfer_command_lcd(0xaf);  //¿ªÊ¼ÏÔÊ¾
}

//Ğ´Ö¸Áîµ½LCDÄ£¿é
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

//Ğ´Êı¾İµ½LCDÄ£¿é
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

//ÉèÖÃµØÖ·
void lcd_address(u8 page,u8 column)
{
	column=column-0x01;
	transfer_command_lcd(0xb0+page-1);   						//ÉèÖÃÒ³µØÖ·£¬8ĞĞÎªÒ»Ò³£¬È«ÆÁ¹²64ĞĞ£¬¹²¼Æ8Ò³
	transfer_command_lcd(0x10+(column>>4&0x0f));		//ÉèÖÃÁĞµØÖ·¸ßËÄÎ»
	transfer_command_lcd(column & 0x0f);						//ÉèÖÃÁĞµØÖ·µÍËÄÎ»
}

//ÇåÆÁ
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

//ÏÔÊ¾128x64µãÕóÍ¼Ïñ
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

//ÏÔÊ¾16x16µãÕóÍ¼Ïñ
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

//ÏÔÊ¾8x16µãÕóÍ¼Ïñ
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

//ÏÔÊ¾5X8µãÕóÍ¼Ïñ
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



//ÏÔÊ¾´íÎóÍË³ö--ÔÚ¶à´Î³¢ÊÔ²»ÄÜĞ´ÈëÊı¾İºóÏÔÊ¾
u8 display_Error(void)
{
//	u8 hang,lie;
	clear_screen();
	display_graphic_16x16_page(1,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart,EmptyStart);
//	hang=1;	lie=1;
//	display_graphic_16x16(hang,lie,EmptyStart);				//ÏÔÊ¾¿ÕĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//ÏÔÊ¾¿ÕĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//ÏÔÊ¾¿ÕĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//ÏÔÊ¾¿ÕĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,EmptyStart);				//ÏÔÊ¾¿ÕĞÄÎå½ÇĞÇ
	return 0;
}

//ÏÔÊ¾ÕıÈ·ÍË³ö--Êı¾İĞ´ÈëÍê³ÉºóÏÔÊ¾
u8 display_Right(void )
{
//	u8 hang,lie;
	clear_screen();
	display_graphic_16x16_page(1,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart,FullStart);
//	hang=1;	lie=1;
//	display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ÊµĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ÊµĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ÊµĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ÊµĞÄÎå½ÇĞÇ
//	lie=lie+16;
//	display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ÊµĞÄÎå½ÇĞÇ
	return 0;
}

//ÏÔÊ¾²Ëµ¥
u8 display_menu(u8 STATUS)
{
//	u8 hang,lie;
	clear_screen();
	clear_screen();
	switch (STATUS)
	{
		case 0:	//ÏÔÊ¾Ä£Ê½²Ëµ¥£ºÄ¬ÈÏÑ¡ÔñÈı½ÇÔÚµÚÒ»¸öÑ¡Ïî£¨½øÈë½ÓÊÕÄ£Ê½£©
		{
			display_graphic_16x16_page(1,Triangle,Jin,Ru,Jie,Shou,Empty,Empty,Empty);
//			hang=1;	lie=1;																		//µÚÒ»ĞĞ
//			display_graphic_16x16(hang,lie,Triangle);					//ÏÔÊ¾Ñ¡ÔñÈı½Ç  Ä¬ÈÏÎ»ÖÃ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jin);							//ÏÔÊ¾¡°½ø¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ru);								//ÏÔÊ¾¡°Èë¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jie);							//ÏÔÊ¾¡°½Ó¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shou);							//ÏÔÊ¾¡°ÊÕ¡±
			display_graphic_16x16_page(2,Empty,Jin,Ru,Fa,Song,Empty,Empty,Empty);
//			hang=hang+2;																			//µÚ¶şĞĞ
//			lie=1;lie=lie+16;
//			display_graphic_16x16(hang,lie,Jin);							//ÏÔÊ¾¡°½ø¡±						
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ru);								//ÏÔÊ¾¡°Èë¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Fa);								//ÏÔÊ¾¡°·¢¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Song);							//ÏÔÊ¾¡°ËÍ¡±
			break;
		}
		case 1:	//ÏÔÊ¾½ÓÊÕÊı¾İ
		{
			display_graphic_16x16_page(1,Shu,Ju,Jie,Shou,Zhong,Empty,Empty,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Shu);							//ÏÔÊ¾¡°Êı¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);								//ÏÔÊ¾¡°¾İ¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Jie);							//ÏÔÊ¾¡°½Ó¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shou);							//ÏÔÊ¾¡°ÊÕ¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Zhong);						//ÏÔÊ¾¡°ÖĞ¡±
			break;
		}
		case 2:	//ÏÔÊ¾·¢ËÍÊı¾İ²Ëµ¥  Î´Ñ¡ÔñÎª¿ÕĞÄÎå½ÇĞÇ Ä¬ÈÏÎªÈ«²¿Ñ¡ÖĞ
		{
			display_graphic_16x16_page(1,Triangle,FullStart,Di,Yi,Lei,Shu,Ju,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Triangle);					//ÏÔÊ¾ Ñ¡ÔñÈı½ÇĞÎ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ ÊµĞÄÎå½ÇĞÇ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//ÏÔÊ¾ ¡°µÚÒ»ÀàÊı¾İ¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Yi);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(2,Empty,FullStart,Di,Er,Lei,Shu,Ju,Empty);
//			hang=3;	lie=1;																		//µÚ¶şĞĞ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ ÊµĞÄÎå½ÇĞÇ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//ÏÔÊ¾ ¡°µÚ¶şÀàÊı¾İ¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Er);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(3,Empty,FullStart,Di,San,Lei,Shu,Ju,Empty);
//			hang=5;	lie=1;																		//µÚÈıĞĞ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ ÊµĞÄÎå½ÇĞÇ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//ÏÔÊ¾ ¡°µÚÈıÀàÊı¾İ¡±
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,San);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Lei);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Shu);
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Ju);
			display_graphic_16x16_page(4,Empty,FullStart,Di,Si,Lei,Shu,Ju,Empty);
//			hang=7;	lie=1;																		//µÚËÄĞĞ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,FullStart);				//ÏÔÊ¾ ÊµĞÄÎå½ÇĞÇ
//			lie=lie+16;
//			display_graphic_16x16(hang,lie,Di);								//ÏÔÊ¾  ¡°µÚËÄÀàÊı¾İ¡±
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
		case 3:	//ÏÔÊ¾·¢ËÍÊı¾İ
		{
			display_graphic_16x16_page(1,Shu,Ju,Fa,Song,Zhong,Empty,Empty,Empty);
//			hang=1;	lie=1;
//			display_graphic_16x16(hang,lie,Shu);						//ÏÔÊ¾  ¡°Êı¾İ·¢ËÍÖĞ¡±
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
		case 10:	//ÉÏÒÆÏÔÊ¾·¢ËÍÊı¾İ²Ëµ¥£¬Ôö¼ÓÈ·¶¨
		{
			if((Send_Select & 0x02)==0x02)										//Èç¹ûµÚ¶şÀàÊı¾İ·¢ËÍÑ¡Ôñ£¬ÏÔÊ¾£ºÊÇĞÄÎå½ÇĞÇ+¡°µÚ¶şÀàÊı¾İ¡±
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
			else																							//Èç¹ûµÚ¶şÀàÊı¾İÑ¡Ôñ²»·¢ËÍ£¬ÏÔÊ¾£º¿ÕĞÄÎå½ÇĞÇ+¡°µÚ¶şÀàÊı¾İ¡±
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

//ÏÔÊ¾Ò»ÕûĞĞÊı¾İ
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









