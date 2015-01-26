#ifndef __USART_H
#define __USART_H
#include "stdio.h"
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////
//********************************************************************************
////////////////////////////////////////////////////////////////////////////////// 

void UART2PC_Init(u32 bound);										//与PC通讯的USART1初始化
void UART2CR95_Init(u32 bound);									//与CR95通讯的USART2初始化
void uart3_init(u32 bound);

void UART_SendByte(USART_TypeDef* USARTx, uint8_t data);
uint8_t UART_ReceiveByte( USART_TypeDef* USARTx );
void UART_SendBuffer(USART_TypeDef* USARTx, uc8 *pCommand, uint8_t length);
void UART_ReceiveBuffer(USART_TypeDef* USARTx, uint8_t *pResponse, uint8_t length) ;
void Prints(USART_TypeDef* USARTx,u8 *pd);
void PrintShortIntHex(USART_TypeDef* USARTx,u16 x);
void PrintLongInt(USART_TypeDef* USARTx,u32 x);
void PrintHex(USART_TypeDef* USARTx,u8 x);

void USART_KFB_Write(u16 Add,u8 Data1,u8 Data2, u8 Data3,u8 Data4);       //写入开发板指定地址、指定数据
void USART_KFB_Read(u16 Add);					//读取开发板指定地址的数据







#endif
