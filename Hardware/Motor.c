#include "stm32f10x.h"
#include "PWM.h"

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructureB;
	GPIO_InitStructureB.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructureB.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructureB.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructureB);		
	
	PWM_Init();													
}

void Motor_SetPA8Speed(int8_t Speed) //右轮
{
    if (Speed >0) {
        GPIO_SetBits(GPIOB, GPIO_Pin_12);  
        GPIO_ResetBits(GPIOB, GPIO_Pin_13); 
        PWM_SetPA8Compare1(Speed);        
    } else if(Speed<0){
        GPIO_ResetBits(GPIOB, GPIO_Pin_12); 
        GPIO_SetBits(GPIOB, GPIO_Pin_13);   
        PWM_SetPA8Compare1(-Speed);        
    }
	 else{
		  GPIO_SetBits(GPIOB, GPIO_Pin_12);  
        GPIO_SetBits(GPIOB, GPIO_Pin_13); 
	 }
}

void Motor_SetPA11Speed(int8_t Speed)//左轮
{
	if (Speed > 0)							
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	
		PWM_SetPA11Compare4(Speed);				
	}
	else if(Speed<0)								
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_15);	
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	
		PWM_SetPA11Compare4(-Speed);			
	}
	else{
		GPIO_SetBits(GPIOB, GPIO_Pin_15);	
		GPIO_SetBits(GPIOB, GPIO_Pin_14);	
	}
}

