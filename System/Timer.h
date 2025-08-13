#ifndef __TIMER_H
#define __TIMER_H

#define Trig_Port 		GPIOA
#define Trig_Pin 		GPIO_Pin_3
#define Trig_RCC		RCC_APB2Periph_GPIOA

#define Echo_Port 		GPIOA
#define Echo_Pin 		GPIO_Pin_2
#define Echo_RCC		RCC_APB2Periph_GPIOA

void Timer_Init(void);

#endif
