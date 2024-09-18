#ifndef __I2C_H
#define __I2C_H


static ErrorStatus spl_i2c_wait_event(I2C_TypeDef *I2Cx, uint32_t I2C_EVENT, uint32_t timeout);
int8_t spl_i2c_mem_read(I2C_TypeDef *I2Cx, uint8_t dev_addr, uint8_t mem_addr, uint8_t *data, uint8_t data_size);
int8_t spl_i2c_read(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t data_size);
int8_t spl_i2c_write(I2C_TypeDef *I2Cx, uint8_t addr, uint8_t *data, uint8_t data_size);

void Gray_Read_Ahead(uint8_t *data);
void Gray_Read_Back(uint8_t *data);
void Color_Read(uint8_t *color);
uint32_t Angle_Read(uint8_t *angle);
#endif

