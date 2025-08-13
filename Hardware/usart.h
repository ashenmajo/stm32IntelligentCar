#ifndef __USART_H__
#define __USART_H__

/*********************************
 * 头文件包含区
*********************************/
#include "stm32f10x.h"
#include <stdio.h>

/*********************************
 * 全局变量声明区
*********************************/
extern int RecvLen;
extern char RecvData[1024];

void Usart3_Init(void); 
void USART3_IRQHandler(void);
void USART_SendDatas(USART_TypeDef *USARTx,unsigned char *SendBuf,uint8_t len); 
#endif

