#include "stm32f10x.h"                  // Device header
#include "I2C.h"
#include "OLED.h"

static uint32_t i2c_timeout = 1000;

static ErrorStatus spl_i2c_wait_event(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT, uint32_t timeout)
{
	volatile uint32_t local_timeout = timeout;
	while(local_timeout != 0) {
		if (I2C_CheckEvent(I2Cx, I2C_EVENT) == SUCCESS) {
			return SUCCESS;
		}
		--local_timeout;
	}
	
	return ERROR;
}

int8_t spl_i2c_mem_read(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint8_t data_size)
{
	int i;
	ErrorStatus status;

	I2C_GenerateSTART(I2Cx, ENABLE);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, i2c_timeout);
	if (status == ERROR)
		return 1;
	
	I2C_Send7bitAddress(I2Cx, dev_addr, I2C_Direction_Transmitter);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, i2c_timeout);
	if (status == ERROR)
		return 2;
	
	I2C_SendData(I2Cx, mem_addr);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, i2c_timeout);
	if (status == ERROR)
		return 3;

	I2C_GenerateSTART(I2Cx, ENABLE);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, i2c_timeout);
	if (status == ERROR)
		return 4;
	
	I2C_Send7bitAddress(I2Cx, dev_addr, I2C_Direction_Receiver);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, i2c_timeout);
	if (status == ERROR)
		return 5;
	
	for (i = 0; i < data_size-1; ++i) {
		status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, i2c_timeout);
		if (status != SUCCESS)
			return 6;
		*data++ = I2C_ReceiveData(I2Cx);
	}
	
	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, i2c_timeout);
	if (status != SUCCESS)
		return 7;
	*data++ = I2C_ReceiveData(I2Cx);
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);
	
	return 0;
}

int8_t spl_i2c_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t data_size)
{
	int i;
	ErrorStatus status;
	I2C_GenerateSTART(I2Cx, ENABLE);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, i2c_timeout);
	if (status != SUCCESS)
		return 1;
	
	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Receiver);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED, i2c_timeout);
	if (status != SUCCESS)
		return 2;
	
	for (i = 0; i < data_size-1; ++i) {
		/* ??????,??????????,??????? */
		status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, i2c_timeout);
		if (status != SUCCESS)
			return 3;
		*data++ = I2C_ReceiveData(I2Cx);
	}
	

	I2C_AcknowledgeConfig(I2Cx, DISABLE);
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED, i2c_timeout);
	if (status != SUCCESS)
		return 3;
	*data++ = I2C_ReceiveData(I2Cx);
	
	I2C_AcknowledgeConfig(I2Cx, ENABLE);

	return 0;
}


int8_t spl_i2c_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t data_size)
{
	int i;
	ErrorStatus status;
	I2C_GenerateSTART(I2Cx, ENABLE);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_MODE_SELECT, i2c_timeout);
	if (status == ERROR)
		return 1;
	
	I2C_Send7bitAddress(I2Cx, addr, I2C_Direction_Transmitter);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED, i2c_timeout);
	if (status == ERROR)
		return 2;
	
	for (i = 0; i < data_size-1; ++i) {
		I2C_SendData(I2Cx, *data++);
		status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING, i2c_timeout);
		if (status == ERROR)
			return 3;
	}
	
	I2C_SendData(I2Cx, *data++);
	status = spl_i2c_wait_event(I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED, i2c_timeout);
	if (status == ERROR)
		return 4;
	
	I2C_GenerateSTOP(I2Cx, ENABLE);
	
	return 0;
}

#define GW_GRAY_ANALOG_BASE_ 0xDD
#define GW_GRAY_ANALOG_MODE  (GW_GRAY_ANALOG_BASE_ + 0)

// 读取前置灰度
void Gray_Read_Ahead(uint8_t *data)
{
	spl_i2c_mem_read(I2C2, 0xAF, 0xA0, data, 8);
	uint8_t data_10 = data[0];
	for (uint8_t i = 0 ; i < 8; i++) data[i] = !((data_10 >> i) & 0x01);
}

// 读取后置灰度
void Gray_Read_Back(uint8_t *data)
{
	spl_i2c_mem_read(I2C1, 0xAF, 0xA0, data, 8);
	uint8_t data_10 = data[0];
	for (uint8_t i = 0 ; i < 8; i++) data[i] = !((data_10 >> i) & 0x01);
	
}

// 读取 颜色
void Color_Read(uint8_t *color)
{
	spl_i2c_mem_read(I2C2 , 0x4C << 1 , 0xD0, color, 3);
	OLED_ShowNum(3, 6, color[0], 3);
}


// 读取陀螺仪角度
uint32_t Angle_Read(uint8_t *angle)
{
	uint32_t Angle;
	spl_i2c_mem_read(I2C1, 0xA0 , 0x3f, angle, 2);
	Angle = (((angle[1] << 8) | angle[0]) * 180) / 32768;
	return Angle;

}





