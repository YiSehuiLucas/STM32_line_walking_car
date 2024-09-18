#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Delay.h"
#include "I2C.h"
#include "OLED.h"
#include "Switch.h"
float Position_KP=20.00,Position_KI=0.000,Position_KD=200.0; /* PIDϵ�� */

// PA11��PA12���Ƶ��ұ����������ת
void Right_Motor_Ahead(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_11);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
}

// PA11��PA12���Ƶ��ұ����������ת
void Right_Motor_Back(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_12);
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
}

// PB1��PB5���Ƶ�������������ת
void Left_Motor_Ahead(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

// PB1��PB5���Ƶ�������������ת
void Left_Motor_Back(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
}

// ֹͣ
void Motor_Stop(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_11);
	GPIO_ResetBits(GPIOA, GPIO_Pin_12);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1);
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

// ǰ��
void Ahead(void)
{
	Right_Motor_Ahead();
	Left_Motor_Ahead();
}

// ����
void Back(void)
{
	Right_Motor_Back();
	Left_Motor_Back();
}

// ��һ��ʱ��ǰ��
void Ahead_Time_Speed(int8_t time_ms, uint8_t speed)
{
	Ahead();
	Set_Speed(speed);
	Delay_ms(time_ms);
	Set_Speed(0);
}

// ����һ��ʱ�����
void Back_Time_Speed(int8_t time_ms, uint8_t speed)
{
	Back();
	Set_Speed(speed);
	Delay_ms(time_ms);
	Set_Speed(0);
}

// �ĸ����������ͬ���ٶ�
void Set_Speed(uint8_t speed)
{
	TIM_SetCompare1(TIM2, speed);
	TIM_SetCompare2(TIM2, speed);
	TIM_SetCompare3(TIM2, speed);
	TIM_SetCompare4(TIM2, speed);
}

// ǰ��Ѳ��
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
		// ����ƫerrorΪ�� ����ƫerrorΪ��
		// uint8_t k = 10;
		float error;
		// ֱ����ʻ 0001 1000
		if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
			Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// ��ƫ0.1 0011 1000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// ��ƫ1 0011 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 1; 
		// ��ƫ2 0111 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// ��ƫ3 0110 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 3; 
		// ��ƫ4 1110 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// ��ƫ5 1100 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 5;
		// ��ƫ6 1000 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 6;
		
		// ��ƫ0.1 0001 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -0.1; 
		// ��ƫ1 0000 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -1; 
		// ��ƫ2 0000 1110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -2;
		// ��ƫ3 0000 0110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -3; 
		// ��ƫ4 0000 0111
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -4;
		// ��ƫ5 0000 0011
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -5;
		// ��ƫ6 0000 0001
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 1)
			error = -6;
		// �������
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
		else if (error > 0) // ��ƫ
		{
			TIM_SetCompare1(TIM2, speed);
			TIM_SetCompare2(TIM2, speed);
			TIM_SetCompare3(TIM2, speed-P*error);
			TIM_SetCompare4(TIM2, speed-P*error);
		}
		else if (error < 0) // ��ƫ
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


// ����Ѳ��
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
		// ����ƫerrorΪ�� ����ƫerrorΪ��
		// uint8_t k = 10;
		float error;
		// ֱ����ʻ 0001 1000
		if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
			Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// ��ƫ0.1 0011 1000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 0.1; 
		// ��ƫ1 0011 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 1; 
		// ��ƫ2 0111 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// ��ƫ3 0110 0000
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 3; 
		// ��ƫ4 1110 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 1 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 4;
		// ��ƫ5 1100 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 5;
		// ��ƫ6 1000 0000
		else if (Gray_Data_Ahead[0] == 1 && Gray_Data_Ahead[1] == 1 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = 6;
		
		// ��ƫ0.1 0001 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 1 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -0.1; 
		// ��ƫ1 0000 1100
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -1; 
		// ��ƫ2 0000 1110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 1 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -2; 
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 0)
			error = -2;
		// ��ƫ3 0000 0110
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -3; 
		// ��ƫ4 0000 0111
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 1 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -4;
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 0)
			error = -4;
		// ��ƫ5 0000 0011
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 1 && Gray_Data_Ahead[7] == 1)
			error = -5;
		// ��ƫ6 0000 0001
		else if (Gray_Data_Ahead[0] == 0 && Gray_Data_Ahead[1] == 0 && Gray_Data_Ahead[2] == 0 && Gray_Data_Ahead[3] == 0 &&
				 Gray_Data_Ahead[4] == 0 && Gray_Data_Ahead[5] == 0 && Gray_Data_Ahead[6] == 0 && Gray_Data_Ahead[7] == 1)
			error = -6;
		// �������
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
		else if (error > 0) // ��ƫ
		{
			TIM_SetCompare1(TIM2, speed-P*error);
			TIM_SetCompare2(TIM2, speed-P*error);
			TIM_SetCompare3(TIM2, speed);
			TIM_SetCompare4(TIM2, speed);
		}
		else if (error < 0) // ��ƫ
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

























