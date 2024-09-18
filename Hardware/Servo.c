#include "stm32f10x.h"                  // Device header
#include "Delay.h"
void Servo_SetAngle(uint8_t number, float Angle)
{
    if (number == 1)
        TIM_SetCompare1(TIM3, Angle / 180 * 2000 + 500);
    else if (number == 2)
        TIM_SetCompare2(TIM3, Angle / 180 * 2000 + 500);
    else
        TIM_SetCompare3(TIM3, Angle / 180 * 2000 + 500);
}

void Hand(void)
{
	Delay_ms(500);
	Servo_SetAngle(2, 0);
	Delay_ms(500);
	Servo_SetAngle(2, 180);
}