#ifndef _FLASH_H
#define _FLASH_H

#include "My_Config.h"

//------ЦёБо
#define FLASH_Write_Enable						0x06
#define FLASH_Write_Disable						0x04
#define FLASH_Read_Status_Reg					0x05
#define FLASH_Write_Status_Reg				0x01
#define FLASH_Read_Data								0x03
#define FLASH_Fast_Read								0x0B
#define FLASH_Fast_Read_Dual					0x3B
#define FLASH_Page_Program						0x02
#define FLASH_Block_Erase							0xD8
#define FLASH_Sector_Erase						0x20
#define FLASH_Chip_Erase							0xC7
#define FLASH_Power_Down							0xB9
#define FLASH_Device_ID								0x90



#define FLASH_NCS_LOW() 					GPIO_ResetBits(FLASH_NCS_GPIO_PORT, FLASH_NCS_PIN)
#define FLASH_NCS_HIGH()  				GPIO_SetBits  (FLASH_NCS_GPIO_PORT, FLASH_NCS_PIN)

void SPI_FLASH_Init(void);
u8 FLASH_Read_ID(u8 *pResponse );
void Flash_Format(void);
void Flash_Writ_En(void);
void Flash_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void Flash_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead) ;
#endif
