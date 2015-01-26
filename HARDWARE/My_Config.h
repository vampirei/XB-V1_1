#ifndef MY_CONFIG_H
#define MY_CONFIG_H

#include "stm32f10x.h"
#include "sys.h"
#include "usart.h"
#include "Timer.h"
#include "CR95.h"
#include "delay.h"
#include "spi.h"
#define DebugDisplay  1

#define MIN(x,y) 					((x < y)? x : y) 
#define MAX(x,y) 					((x > y)? x : y)
#define ABS(x) 						((x)>0 ? (x) : -(x)) 
#define RESULTOK							0
#define ERRORCODE_GENERIC 		1 

#define DisplayPort      	USART1
#define CR95Port				 	USART2

//----------CR95
#define CR95HF_FACE_PORT								GPIOC
#define CR95HF_FACE_PORT_RCC						RCC_APB2Periph_GPIOC
#define CR95HF_FACE_GPIO_Pin						GPIO_Pin_5
#define CR95HF_FACE_GPIO_Speed					GPIO_Speed_50MHz
#define CR95HF_FACE_GPIO_Mode						GPIO_Mode_Out_PP

#define CR95HF_CS_PORT									GPIOA
#define CR95HF_CS_PORT_RCC							RCC_APB2Periph_GPIOA
#define CR95HF_CS_GPIO_Pin							GPIO_Pin_4
#define CR95HF_CS_GPIO_Speed						GPIO_Speed_50MHz
#define CR95HF_CS_GPIO_Mode							GPIO_Mode_Out_PP

#define CR95HF_UART			        				USART2
#define CR95HF_UART_CLK		        			RCC_APB1Periph_USART2

#define CR95HF_UART_TX_PIN         			GPIO_Pin_2                 
#define CR95HF_UART_TX_GPIO_PORT   			GPIOA                       
#define CR95HF_UART_TX_GPIO_CLK    			RCC_APB2Periph_GPIOA 
 
#define CR95HF_UART_RX_PIN        			GPIO_Pin_3                 
#define CR95HF_UART_RX_GPIO_PORT  			GPIOA                       
#define CR95HF_UART_RX_GPIO_CLK   			RCC_APB2Periph_GPIOA 

#define CR95HF_SPI											SPI1
#define CR95HF_SPI_CLK		    					RCC_APB2Periph_SPI1

#define CR95HF_SPI_SCK_PIN        			GPIO_Pin_5                 
#define CR95HF_SPI_SCK_GPIO_PORT   			GPIOA                       
#define CR95HF_SPI_SCK_GPIO_CLK    			RCC_APB2Periph_GPIOA 
 
#define CR95HF_SPI_MISO_PIN        			GPIO_Pin_6                 
#define CR95HF_SPI_MISO_GPIO_PORT  			GPIOA                       
#define CR95HF_SPI_MISO_GPIO_CLK   			RCC_APB2Periph_GPIOA 
 
#define CR95HF_SPI_MOSI_PIN        			GPIO_Pin_7                 
#define CR95HF_SPI_MOSI_GPIO_PORT  			GPIOA                       
#define CR95HF_SPI_MOSI_GPIO_CLK   			RCC_APB2Periph_GPIOA 

#define CR95HF_NCS_PIN             			GPIO_Pin_4                  
#define CR95HF_NCS_GPIO_PORT       			GPIOA                       
#define CR95HF_NCS_GPIO_CLK        			RCC_APB2Periph_GPIOA


//--------TIMER

#define TIMER_STANDALONE												TIM2
#define TIMER_STANDALONE_PERIOD									500
#define TIMER_STANDALONE_PRESCALER							7200
#define TIMER_STANDALONE_CLOCK									RCC_APB1Periph_TIM2
#define TIMER_STANDALONE_PREEMPTION_PRIORITY		2
#define TIMER_STANDALONE_SUB_PRIORITY						1
#define TIMER_STANDALONE_IRQ_CHANNEL						TIM2_IRQn

#define TIMER_TIMEOUT														TIM3
#define TIMER_TIMEOUT_PERIOD										300
#define TIMER_TIMEOUT_PRESCALER									7200
#define TIMER_TIMEOUT_CLOCK											RCC_APB1Periph_TIM3
#define TIMER_TIMEOUT_PREEMPTION_PRIORITY				0
#define TIMER_TIMEOUT_SUB_PRIORITY							0
#define TIMER_TIMEOUT_IRQ_CHANNEL								TIM3_IRQn

#define TIMER_DELAY															TIM4
#define TIMER_DELAY_PERIOD											1000
#define TIMER_DELAY_PRESCALER										72
#define TIMER_DELAY_CLOCK												RCC_APB1Periph_TIM4
#define TIMER_DELAY_PREEMPTION_PRIORITY					2
#define TIMER_DELAY_SUB_PRIORITY								2
#define TIMER_DELAY_IRQ_CHANNEL									TIM4_IRQn


//************FLASH
#define FLASH_SPI																SPI1
#define FLASH_SPI_PORT													GPIOA
#define FLASH_SPI_CLK		    										RCC_APB2Periph_SPI1
#define FLASH_SPI_GPIO_CLK											RCC_APB2Periph_GPIOA

#define FLASH_SPI_SCK_PIN        								GPIO_Pin_5                 
#define FLASH_SPI_SCK_GPIO_PORT   							GPIOA                       
#define FLASH_SPI_SCK_GPIO_CLK    							RCC_APB2Periph_GPIOA 
 
#define FLASH_SPI_MISO_PIN        							GPIO_Pin_6                 
#define FLASH_SPI_MISO_GPIO_PORT  							GPIOA                       
#define FLASH_SPI_MISO_GPIO_CLK   							RCC_APB2Periph_GPIOA 
 
#define FLASH_SPI_MOSI_PIN        							GPIO_Pin_7                 
#define FLASH_SPI_MOSI_GPIO_PORT  							GPIOA                       
#define FLASH_SPI_MOSI_GPIO_CLK   							RCC_APB2Periph_GPIOA 

#define FLASH_NCS_PIN             							GPIO_Pin_4                 
#define FLASH_NCS_GPIO_PORT       							GPIOA                       
#define FLASH_NCS_GPIO_CLK        							RCC_APB2Periph_GPIOA

//************Key







#endif
















