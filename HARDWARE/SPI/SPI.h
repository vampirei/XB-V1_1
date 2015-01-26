#ifndef _SPI_H
#define _SPI_H

#include "My_Config.h"

int8_t SPI_SendReceive(uc8 *pCommand, uint8_t *pResponse);						//ͨ��SPI���� �����������ݴ���
static void CR95HF_Send_SPI_Command(uc8 *pData);											//ͨ��SPI��������
uint8_t SPI_SendReceiveByte(SPI_TypeDef* SPIx, uint8_t data);					//SPI���ͽ���һ���ֽ�
void SPI_SendReceiveBuffer(SPI_TypeDef* SPIx, 
														uc8 *pCommand, 
														uint8_t length, 
														uint8_t *pResponse);											//SPI���ͽ��ջ�������
static int8_t CR95HF_PollingCommand( void );													//��ѯCR95HF  Polling
void CR95HF_Receive_SPI_Response(uint8_t *pData);											//��ȡCR95HF����
void SPI_Two_Init(void);




#endif
