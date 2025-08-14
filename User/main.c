#include "stm32f10x.h"
#include "stdlib.h"
#include "Delay.h"
#include "usart.h"
#include "Motor.h"
#include "OLED.h"
#include "HCSR04.h"

volatile int8_t Rspeed = 0;
volatile int8_t Lspeed = 0;
volatile uint8_t isBlue = 0;

uint16_t distance;
uint16_t rstate, lstate;

void TCRT_Init(void);
uint8_t read_stable_state(uint16_t pin);
void update_motor_speed(void);
void show_status(void);
void Warning_Init(void);

void move_forward(void)    { Rspeed = 50;  Lspeed = 50;  update_motor_speed(); }
void move_backward(void)   { Rspeed = -50; Lspeed = -50; update_motor_speed(); }
void turn_left(void)       { Rspeed = 0;   Lspeed = 50;  update_motor_speed(); }
void turn_right(void)      { Rspeed = 50;  Lspeed = 0;   update_motor_speed(); }
void stop_motors(void)     { Rspeed = 0;   Lspeed = 0;   update_motor_speed(); }

int main(void) {
    Usart3_Init();//蓝牙串口
    OLED_Init();//显示屏
    Motor_Init();//电机
    HCSR04_Init();//超声波
    TCRT_Init();//红外
    Warning_Init();//检测到前方由障碍时点亮PC3

    srand(SysTick->VAL);
    while (1) {
        distance = HCSR04_GetValue();
        rstate = read_stable_state(GPIO_Pin_4);
        lstate = read_stable_state(GPIO_Pin_5);
        
        if (isBlue == 0) {
            if (distance <= 15) {

                GPIO_ResetBits(GPIOC,GPIO_Pin_13);
                stop_motors(); Delay_ms(200);
                move_backward(); Delay_ms(300);
                if (rand() % 2) turn_left();
                else turn_right();
                Delay_ms(500);

                move_forward();
            } else {
                GPIO_SetBits(GPIOC,GPIO_Pin_13);
                if (rstate == 0 && lstate == 0) move_forward();
                else if (rstate == 0 && lstate == 1) turn_right();
                else if (rstate == 1 && lstate == 0) turn_left();
                else stop_motors();
            }

            update_motor_speed();  
        } else {

            update_motor_speed(); 
        }

        show_status();
        Delay_ms(20);
    }
}

void USART3_IRQHandler(void) {
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) {
        char cmd = USART_ReceiveData(USART3);
        switch (cmd) {
            case '0': move_forward(); break;
            case '1': stop_motors(); break;
            case '2': move_backward(); break;
            case '3': turn_left(); break;
            case '4': turn_right(); break;
            case '5': isBlue = 1; break;  
            case '6': isBlue = 0; break; 
            case '7': Rspeed += 10; break;
            case '8': Lspeed += 10; break;
            case '9': Rspeed -= 10; break;
            case 'a': Lspeed -= 10; break;//这里不能用'10'每获取一次数据，只能获取一个字符
            case 'b': Lspeed = 50;Rspeed=50; break;
        }

        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
    }
}
      
void update_motor_speed(void) {
    Motor_SetPA11Speed(Lspeed);
    Motor_SetPA8Speed(Rspeed);
}

void show_status(void) {
    OLED_ShowString(1, 1, "Mode:");
    OLED_ShowString(1, 6, isBlue ? "Blue." : "Track");

    OLED_ShowString(2, 1, "Distance:");
    OLED_ShowSignedNum(2, 10, distance, 3);

    OLED_ShowString(3, 1, "L:");
    OLED_ShowNum(3, 3, lstate, 1);
    OLED_ShowString(3, 5, "R:");
    OLED_ShowNum(3, 7, rstate, 1);

    OLED_ShowString(4, 1, "Lp/Rp:");
    OLED_ShowSignedNum(4, 7, Lspeed, 3);
    OLED_ShowSignedNum(4, 12, Rspeed, 3);
}

void TCRT_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void Warning_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitTypeDef initc;
    initc.GPIO_Pin = GPIO_Pin_13;
    initc.GPIO_Mode = GPIO_Mode_Out_PP;
    initc.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &initc);
}
uint8_t read_stable_state(uint16_t pin) {
    uint8_t count = 0;
    for (uint8_t i = 0; i < 5; i++) {
        if (GPIO_ReadInputDataBit(GPIOB, pin)) count++;
        Delay_ms(1);
    }
    return (count >= 3) ? 1 : 0;
}
