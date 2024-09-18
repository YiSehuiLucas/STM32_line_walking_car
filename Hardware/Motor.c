#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Delay.h"
#include "I2C.h"
#include "OLED.h"
#include "Switch.h"
float Position_KP=20.00,Position_KI=0.000,Position_KD=200.0; /* PID系数 */

// PA11和PA12控制的右边两个电机正转
void Right_Motor_Ahead(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

// PA11和PA12控制的右边两个电机反转
void Right_Motor_Back(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

// PB1和PB5控制的左边两个电机正转
void Left_Motor_Ahead(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

// PB1和PB5控制的左边两个电机反转
void Left_Motor_Back(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

// 停止
void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

// 前进
void Ahead(void)
{
	Right_Motor_Ahead();
	Left_Motor_Ahead();
}

// 后退
void Back(void)
{
	Right_Motor_Back();
	Left_Motor_Back();
}

// 按一定时间前进
void Ahead_Time_Speed(int8_t time_ms, uint8_t speed)
{
	Ahead();
	Set_Speed(speed);
	Delay_ms(time_ms);
	Set_Speed(0);
}

// 按照一定时间后退
void Back_Time_Speed(int8_t time_ms, uint8_t speed)
{
	Back();
	Set_Speed(speed);
	Delay_ms(time_ms);
	Set_Speed(0);
}

// 四个电机设置相同的速度
void Set_Speed(uint8_t speed)
{
	TIM_SetCompare1(TIM2, speed);
	TIM_SetCompare2(TIM2, speed);
	TIM_SetCompare3(TIM2, speed);
	TIM_SetCompare4(TIM2, speed);
}

// 前进巡线
void Ahead_Line_Walking(int speed)
{
	Ahead();
	uint8_t Gray_Data_Ahead[8];
	while(1)
	{
		Gray_Read_Ahead(Gray_Data_Ahead);
		for (uint8_t i = 8 ; i > 0; i--)
		{
			OLED_ShowNum(2, i, Gray_Data_Ahead[i-1], 1);
		}
		// 向右偏error为正 向左偏error为负
		// uint8_t k = 10;
		float error;
		// 直线行驶 0001 1000
		if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
			Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// 右偏0.1 0011 1000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// 右偏1 0011 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 1; 
		// 右偏2 0111 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// 右偏3 0110 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 3; 
		// 右偏4 1110 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// 右偏5 1100 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 5;
		// 右偏6 1000 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 6;
		
		// 左偏0.1 0001 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -0.1; 
		// 左偏1 0000 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -1; 
		// 左偏2 0000 1110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -2;
		// 左偏3 0000 0110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -3; 
		// 左偏4 0000 0111
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -4;
		// 左偏5 0000 0011
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -5;
		// 左偏6 0000 0001
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 1)
			error = -6;
		// 其他情况
		else 
			error = 10; 
		
		float P = 9.5;
		if (speed==70) P = 9.5;
		if (speed==50) P = 9.5;
		float last_error;
		if (error==10)
		{
			TIM_SetCompare1(TIM2, 0);
			TIM_SetCompare2(TIM2, 0);
			TIM_SetCompare3(TIM2, 0);
			TIM_SetCompare4(TIM2, 0);
			break;
		}
		else if (error > 0) // 右偏
		{
			TIM_SetCompare1(TIM2, speed);
			TIM_SetCompare2(TIM2, speed);
			TIM_SetCompare3(TIM2, speed-P*error);
			TIM_SetCompare4(TIM2, speed-P*error);
		}
		else if (error < 0) // 左偏
		{
			TIM_SetCompare1(TIM2, speed+P*error);
			TIM_SetCompare2(TIM2, speed+P*error); 
			TIM_SetCompare3(TIM2, speed);
			TIM_SetCompare4(TIM2, speed);
		}
		else 
		{
			TIM_SetCompare1(TIM2, speed);
			TIM_SetCompare2(TIM2, speed);
			TIM_SetCompare3(TIM2, speed);
			TIM_SetCompare4(TIM2, speed);
		}
	}
}


// 后退巡线
void Back_Line_Walking(int speed)
{
	Back();
	uint8_t Gray_Data_Ahead[8];
	while(1)
	{
		Gray_Read_Back(Gray_Data_Ahead);
		for (uint8_t i = 8 ; i > 0; i--)
		{
			OLED_ShowNum(2, i, Gray_Data_Ahead[i-1], 1);
		}
		// 向右偏error为正 向左偏error为负
		// uint8_t k = 10;
		float error;
		// 直线行驶 0001 1000
		if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
			Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// 右偏0.1 0011 1000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// 右偏1 0011 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 1; 
		// 右偏2 0111 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// 右偏3 0110 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 3; 
		// 右偏4 1110 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// 右偏5 1100 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 5;
		// 右偏6 1000 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 6;
		
		// 左偏0.1 0001 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -0.1; 
		// 左偏1 0000 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -1; 
		// 左偏2 0000 1110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -2;
		// 左偏3 0000 0110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -3; 
		// 左偏4 0000 0111
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -4;
		// 左偏5 0000 0011
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -5;
		// 左偏6 0000 0001
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 1)
			error = -6;
		// 其他情况
		else 
			error = 10; 
		
		float P = 9.5;
		if (speed==70) P = 10;
		if (speed==50) P = 9.5;
		float last_error;
		if (error==10)
		{
			TIM_SetCompare1(TIM2, 0);
			TIM_SetCompare2(TIM2, 0);
			TIM_SetCompare3(TIM2, 0);
			TIM_SetCompare4(TIM2, 0);
			break;
		}
		else if (error > 0) // 右偏
		{
			TIM_SetCompare1(TIM2, speed-P*error);
			TIM_SetCompare2(TIM2, speed-P*error);
			TIM_SetCompare3(TIM2, speed);
			TIM_SetCompare4(TIM2, speed);
		}
		else if (error < 0) // 左偏
		{
			TIM_SetCompare1(TIM2, speed);
			TIM_SetCompare2(TIM2, speed); 
			TIM_SetCompare3(TIM2, speed+P*error);
			TIM_SetCompare4(TIM2, speed+P*error);
		}
		else 
		{
			TIM_SetCompare1(TIM2, speed);
			TIM_SetCompare2(TIM2, speed);
			TIM_SetCompare3(TIM2, speed);
			TIM_SetCompare4(TIM2, speed);
		}
	}
}

void Turn_Left(uint8_t angle)
{
	Right_Motor_Ahead();
	Left_Motor_Back();
	uint8_t Angle[2];
	uint8_t current_angle = Angle_Read(Angle);
	OLED_ShowNum(4, 1, current_angle, 3);
	int target_angle = current_angle + angle;
	OLED_ShowNum(4, 6, target_angle, 3);
	if(target_angle > 359) target_angle = target_angle - 360;
	while(Angle_Read(Angle)!=target_angle)
	{
		Set_Speed(60);
		OLED_ShowNum(4, 10, Angle_Read(Angle), 3);
	}
	Set_Speed(0);
}

void Turn_Right(uint8_t angle)
{
	Right_Motor_Back();
	Left_Motor_Ahead();
	uint8_t Angle[2];
	uint8_t current_angle = Angle_Read(Angle);
	OLED_ShowNum(4, 1, current_angle, 3);
    int target_angle = current_angle - angle;
	OLED_ShowNum(4, 6, target_angle, 3);
	if(target_angle < 0) target_angle = -target_angle;
	while(Angle_Read(Angle)!=target_angle)
	{
		Set_Speed(60);
		OLED_ShowNum(4, 10, Angle_Read(Angle), 3);
	}
	Set_Speed(0);
}


void Turn(void)
{
	Right_Motor_Ahead();
	Left_Motor_Back();
	Set_Speed(50);
	Delay_ms(300);
	Set_Speed(0);
}

























