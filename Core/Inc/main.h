/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern I2C_HandleTypeDef hi2c1;

// Deklaracja handlerów dla timerów
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim2;
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define ZERO1_Pin GPIO_PIN_13
#define ZERO1_GPIO_Port GPIOC
#define ZERO2_Pin GPIO_PIN_14
#define ZERO2_GPIO_Port GPIOC
#define OSC8M_IN_Pin GPIO_PIN_0
#define OSC8M_IN_GPIO_Port GPIOH
#define OSC8M_OUT_Pin GPIO_PIN_1
#define OSC8M_OUT_GPIO_Port GPIOH
#define SMC124_PWM_Pin GPIO_PIN_0
#define SMC124_PWM_GPIO_Port GPIOA
#define ENCODER1_CH2_Pin GPIO_PIN_1
#define ENCODER1_CH2_GPIO_Port GPIOA
#define SMC124_DIR_Pin GPIO_PIN_3
#define SMC124_DIR_GPIO_Port GPIOA
#define SMC124_EN_Pin GPIO_PIN_4
#define SMC124_EN_GPIO_Port GPIOA
#define ENCODER1_CH1_Pin GPIO_PIN_5
#define ENCODER1_CH1_GPIO_Port GPIOA
#define ENCODER2_CH1_Pin GPIO_PIN_6
#define ENCODER2_CH1_GPIO_Port GPIOA
#define ENCODER2_CH2_Pin GPIO_PIN_7
#define ENCODER2_CH2_GPIO_Port GPIOA
#define SM2_SD_Pin GPIO_PIN_0
#define SM2_SD_GPIO_Port GPIOB
#define SM2_CCW_Pin GPIO_PIN_1
#define SM2_CCW_GPIO_Port GPIOB
#define SM2_CW_Pin GPIO_PIN_2
#define SM2_CW_GPIO_Port GPIOB
#define SM1_SD_Pin GPIO_PIN_13
#define SM1_SD_GPIO_Port GPIOB
#define SM1_CCW_Pin GPIO_PIN_14
#define SM1_CCW_GPIO_Port GPIOB
#define SM1_CW_Pin GPIO_PIN_15
#define SM1_CW_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_8
#define LED2_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOA
#define USB_DATAM_Pin GPIO_PIN_11
#define USB_DATAM_GPIO_Port GPIOA
#define USB_DATAP_Pin GPIO_PIN_12
#define USB_DATAP_GPIO_Port GPIOA
#define HX711_SCK_Pin GPIO_PIN_15
#define HX711_SCK_GPIO_Port GPIOA
#define HX711_DOUT_Pin GPIO_PIN_3
#define HX711_DOUT_GPIO_Port GPIOB
#define POT_AD0_Pin GPIO_PIN_4
#define POT_AD0_GPIO_Port GPIOB
#define POT_AD1_Pin GPIO_PIN_5
#define POT_AD1_GPIO_Port GPIOB
#define I2C_POT_SCL_Pin GPIO_PIN_6
#define I2C_POT_SCL_GPIO_Port GPIOB
#define I2C_POT_SDA_Pin GPIO_PIN_7
#define I2C_POT_SDA_GPIO_Port GPIOB
#define POT_WP_Pin GPIO_PIN_8
#define POT_WP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
