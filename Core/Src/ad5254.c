#include "ad5254.h"
#include "main.h"

/**
 * @brief Ustawia wartość rezystancji dla wybranego kanału i adresu I2C.
 */
HAL_StatusTypeDef AD5254_SetValue(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t value) {
    uint8_t data[2];
    data[0] = channel;
    data[1] = value;

    return HAL_I2C_Master_Transmit(hi2c, AD5254_I2C_ADDRESS, data, 2, HAL_MAX_DELAY);
}

/**
 * @brief Odczytuje wartość rezystancji z wybranego kanału i adresu I2C.
 */
HAL_StatusTypeDef AD5254_GetValue(I2C_HandleTypeDef *hi2c, uint8_t channel, uint8_t *value) {
    if (HAL_I2C_Master_Transmit(hi2c, AD5254_I2C_ADDRESS, &channel, 1, HAL_MAX_DELAY) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_I2C_Master_Receive(hi2c, AD5254_I2C_ADDRESS, value, 1, HAL_MAX_DELAY);
}
