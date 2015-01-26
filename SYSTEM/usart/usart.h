#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
////////////////////////////////////////////////////////////////////////////////// 

void UART2PC_Init(u32 bound);										//��PCͨѶ��USART1��ʼ��
void UART2CR95_Init(u32 bound);									//��CR95ͨѶ��USART2��ʼ��
void uart3_init(u32 bound);

void UART_SendByte(USART_TypeDef* USARTx, uint8_t data);
uint8_t UART_ReceiveByte( USART_TypeDef* USARTx );
void UART_SendBuffer(USART_TypeDef* USARTx, uc8 *pCommand, uint8_t length);
void UART_ReceiveBuffer(USART_TypeDef* USARTx, uint8_t *pResponse, uint8_t length) ;
void Prints(USART_TypeDef* USARTx,u8 *pd);
void PrintShortIntHex(USART_TypeDef* USARTx,u16 x);
void PrintLongInt(USART_TypeDef* USARTx,u32 x);
void PrintHex(USART_TypeDef* USARTx,u8 x);

void USART_KFB_Write(u16 Add,u8 Data1,u8 Data2, u8 Data3,u8 Data4);       //д�뿪����ָ����ַ��ָ������
void USART_KFB_Read(u16 Add);					//��ȡ������ָ����ַ������







#endif
