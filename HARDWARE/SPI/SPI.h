#ifndef _SPI_H
#define _SPI_H

#include "My_Config.h"

int8_t SPI_SendReceive(uc8 *pCommand, uint8_t *pResponse);						//通过SPI发送 并将返回数据存入
static void CR95HF_Send_SPI_Command(uc8 *pData);											//通过SPI发送命令
uint8_t SPI_SendReceiveByte(SPI_TypeDef* SPIx, uint8_t data);					//SPI发送接收一个字节
void SPI_SendReceiveBuffer(SPI_TypeDef* SPIx, 
														uc8 *pCommand, 
														uint8_t length, 
														uint8_t *pResponse);											//SPI发送接收缓存数据
static int8_t CR95HF_PollingCommand( void );													//查询CR95HF  Polling
void CR95HF_Receive_SPI_Response(uint8_t *pData);											//读取CR95HF数据
void SPI_Two_Init(void);




#endif
