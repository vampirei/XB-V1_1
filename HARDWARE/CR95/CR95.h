#ifndef _CR95_H
#define _CR95_H

#include "stm32f10x.h"
#include "My_Config.h"

typedef enum {
	CR95HF_INTERFACE_UART = 0,
	CR95HF_INTERFACE_SPI,
	CR95HF_INTERFACE_TWI
}CR95HF_INTERFACE;

typedef enum {
	SPI_POLLING = 0,
	SPI_INTERRUPT,
}SPI_MODE;

typedef enum {
	CR95HF_STATE_UNKNOWN = 0,
	CR95HF_STATE_HIBERNATE ,
	CR95HF_STATE_SLEEP,
	CR95HF_STATE_POWERUP,
	CR95HF_STATE_TAGDETECTOR,
	CR95HF_STATE_READY,
	CR95HF_STATE_READER,
	CR95HF_STATE_TAGHUNTING,
}CR95HF_STATE;

typedef struct {
	
	CR95HF_INTERFACE 		Interface;
	CR95HF_STATE 				State;
	SPI_MODE 						SpiMode;
	int8_t							CurrentProtocol;
}ReaderConfigStruct;




#define CR95HF_IRQIN_LOW() 				GPIO_ResetBits(CR95HF_UART_RX_GPIO_PORT, CR95HF_UART_RX_PIN)	
#define CR95HF_IRQIN_HIGH()  			GPIO_SetBits  (CR95HF_UART_RX_GPIO_PORT, CR95HF_UART_RX_PIN)

#define CR95HF_NSS_LOW() 					GPIO_ResetBits(CR95HF_NCS_GPIO_PORT, CR95HF_NCS_PIN)
#define CR95HF_NSS_HIGH()  				GPIO_SetBits  (CR95HF_NCS_GPIO_PORT, CR95HF_NCS_PIN)

//------CR95HF 命令代码
#define SPI_RESPONSEBUFFER_SIZE											255
#define MAX_BUFFER_SIZE															256
#define CR95HF_COMMAND_SEND													0x00
#define CR95HF_COMMAND_RESET												0x01
#define CR95HF_COMMAND_RECEIVE											0x02
#define CR95HF_COMMAND_POLLING											0x03
#define ECHORESPONSE																0x55
#define DUMMY_BYTE																	0xFF
#define IDN																					0x01
#define PROTOCOL_SELECT 														0x02
#define POLL_FIELD 																	0x03
#define SEND_RECEIVE																0x04
#define LISTEN																			0x05
#define SEND																				0x06
#define IDLE																				0x07
#define READ_REGISTER																0x08
#define WRITE_REGISTER															0x09
#define BAUD_RATE																		0x0A
#define SUB_FREQ_RES																0x0B
#define AC_FILTER																		0x0D
#define TEST_MODE																		0x0E
#define SLEEP_MODE																	0x0F
#define ECHO																				0x55

#define READERREPLY_STATUSOFFSET										0x00
#define PSEUDOREPLY_OFFSET													0x00
#define IDN_RESULTSCODE_OK													0x00




//------CR95HF 状态
#define CR95HF_FLAG_DATA_READY											0x08
#define CR95HF_FLAG_DATA_READY_MASK									0xF8
#define CR95HF_POLLING_CR95HF												0x00
#define CR95HF_POLLING_TIMEOUT											0x01
#define CR95HF_SUCCESS_CODE													0x00
#define CR95HF_NOREPLY_CODE													0x01
//------------协议状态
#define PROTOCOLSELECT_LENGTH												0x02
#define PROTOCOLSELECT_RESULTSCODE_OK								0x00
#define PROTOCOLSELECT_ERRORCODE_CMDLENGTH					0x82
#define PROTOCOLSELECT_ERRORCODE_INVALID						0x83
//------------协议--发送接收字段状态
#define SENDRECV_RESULTSCODE_OK									0x80
#define SENDRECV_ERRORCODE_COMERROR							0x86
#define SENDRECV_ERRORCODE_FRAMEWAIT						0x87
#define SENDRECV_ERRORCODE_SOF									0x88
#define SENDRECV_ERRORCODE_OVERFLOW							0x89
#define SENDRECV_ERRORCODE_FRAMING							0x8A
#define SENDRECV_ERRORCODE_EGT									0x8B
#define SENDRECV_ERRORCODE_LENGTH								0x8C
#define SENDRECV_ERRORCODE_CRC									0x8D
#define SENDRECV_ERRORCODE_RECEPTIONLOST				0x8E
//------------协议--IDLE字段状态
#define IDLE_RESULTSCODE_OK											0x00
#define IDLE_ERRORCODE_LENGTH										0x82
//------------协议--读取寄存器字段状态
#define READREG_RESULTSCODE_OK									0x00
#define READREG_ERRORCODE_LENGTH								0x82
//------------协议--写寄存器字段状态
#define WRITEREG_RESULTSCODE_OK									0x00
//------CR95HF 错误代码
#define PROTOCOL_SET_CORRECT												0x00
#define PROTOCOL_SET_ERROR													0x01
#define	CR95HF_ERRORCODE_DEFAULT										0xFE
#define	CR95HF_ERRORCODE_TIMEOUT										0xFD
#define	CR95HF_ERRORCODE_UARTDATARATEUNCHANGED			0xFC
#define	CR95HF_ERRORCODE_UARTDATARATEPROCESS				0xFB
#define	CR95HF_ERRORCODE_FILENOTFOUND								0xFA
#define	CR95HF_ERRORCODE_READALLMEMORY							0xF9
#define	CR95HF_ERRORCODE_TAGDETECTINGCALIBRATION		0xF8
#define	CR95HF_ERRORCODE_CUSTOMCOMMANDUNKNOWN				0xF7
#define	CR95HF_ERRORCODE_TAGDETECTING								0xF5
#define	CR95HF_ERRORCODE_NOTAGFOUND									0xF4
#define CR95HF_ERROR_CODE														0x40
#define CR95HF_ERRORCODE_PARAMETERLENGTH						0x41
#define CR95HF_ERRORCODE_PARAMETER									0x42
#define CR95HF_ERRORCODE_COMMANDUNKNOWN							0x43
#define CR95HF_ERRORCODE_PORERROR										0x44


//-----命令及数据偏置
#define CR95HF_COMMAND_OFFSET												0x00
#define CR95HF_LENGTH_OFFSET												0x01
#define CR95HF_DATA_OFFSET													0x02

//-----M24LR64命令代码
#define M24XX_REQUEST_FLAGS													0x02
#define M24XX_READ_SINGLE_BLOCK											0x20
#define M24XX_WRITE_SINGLE_BLOCK										0x21
//-----M24XX错误代码
#define M24XX_COMMAND_ERROR													0x02
#define M24XX_OPTION_ERROR													0x03
#define M24XX_NO_INFORMATION_ERROR									0x0F
#define M24XX_BLOCK_ERROR														0x10
#define M24XX_BLOCK_LOCKED													0x11
#define M24XX_BLOCK_CANT_CHANGE											0x12
#define M24XX_BLOCK_NOT_PROGRAM											0x13
#define M24XX_BLOCK_NOT_LOCKED											0x14
#define M24XX_BLOCK_PROTECTED												0x15

int8_t InitCR95HFConfigStructure (ReaderConfigStruct *ReaderConfig);
void SPI_CR95HF_Init(void);			//与CR95通讯用SPI初始化
void User_SerialInterfaceInitilisation ( void );			//CR95HF通讯端口初始化
int8_t CR95HF_PORsequence( void );										//检测CR95是否存在
void CR95HF_Send_IRQIN_NegativePulse(void);						//向CR95HF发送IRQ_IN低脉冲		SPI工作模式
int8_t CR95HF_Echo(uint8_t *pResponse);								//向CR95HF发送ECHO
int8_t SpyTransaction(uc8 *pCommand, uint8_t *pResponse);	
void CR95HF_Send_SPI_ResetSequence(void);
int8_t CR95HF_IsReaderResultCodeOk (uint8_t CmdCode,uc8 *ReaderReply);
void CR95HF_SendComand(u8 Command);										//向CR95HF发送单个命令
void CR95HF_SendCommandBuff(u8 *Command);							//向CR95HF发送命令数组


u8 CR95HF_Poll(void);
void CR95HF_ReadIDN(u8 *pIDN);												//读取CR95HF的IDN   数组长度17
u8 CR95HF_SET_PROTOCOL_15693(u8 *pResponse);					//设置CR95HF的通讯协议-使用15693
void M24XX_ReadSingleBlock(u16 Address);
void M24XX_ReadIDN(void);
#endif

