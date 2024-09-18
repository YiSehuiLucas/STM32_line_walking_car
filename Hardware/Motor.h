#ifndef __MOTOR_H
#define __MOTOR_H

#define Dead_Voltage 900      /* ËÀÇøµçÑ¹ */

void Right_Motor_Ahead(void);
void Right_Motor_Back(void);
void Left_Motor_Ahead(void);
void Left_Motor_Back(void);
void Motor_Stop(void); 

void Ahead(void);
void Back(void);

void Ahead_Time_Speed(int8_t time_ms, uint8_t speed);
void Back_Time_Speed(int8_t time_ms, uint8_t speed);

void Ahead_Line_Walking(int speed);
void Back_Line_Walking(int speed);

void Turn_Left(uint8_t angle);
void Turn_Right(uint8_t angle);

void Set_Speed(uint8_t speed);

void Turn(void);
#endif


