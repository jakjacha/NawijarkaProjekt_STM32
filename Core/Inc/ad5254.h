#ifndef AD5254_H
#define AD5254_H

#include "stm32f4xx_hal.h"

#define AD5254_I2C_ADDRESS (0x2C << 1)  // Adres bazowy I2C

void AD5254_SetAddress(uint8_t ad1, uint8_t ad0);
HAL_StatusTypeDef AD5254_SetValue(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t value);
HAL_StatusTypeDef AD5254_GetValue(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t *value);

#endif /* AD5254_H */
