#include "cdc_control.h"
#include "stm32f4xx_ll_tim.h"
#include "stm32f4xx_hal_gpio.h"
#include <string.h>
#include <stdlib.h>  // atoi
#include "ad5254.h"
#include "main.h"
#include "hx711.h"

// Zmienne globalne
int message_ready = 1;  // Flaga gotowości wiadomości


// Funkcja odczytu wypełnienie PWM na określonym kanale timera
void CDC_GetPWMDutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel,
		uint8_t PWMPort) {
	int16_t pwm_val = __HAL_TIM_GET_COMPARE(htim, Channel);
	//int16_t pwm_val = pwm_values_tab[0];
	char response[30];
	snprintf(response, sizeof(response), "pwm%d=%d\r\n", PWMPort, pwm_val);
	CDC_Transmit_FS((uint8_t*) response, strlen(response));
}

void CDC_SendResponse(const char *response) {
	//char buffer[100];
	//snprintf(buffer, sizeof(buffer), "%s", response);
	CDC_Transmit_FS((uint8_t*) response, strlen(response));
}

// Funkcja wywoływana przy odebraniu danych przez USB CDC
void CDC_ReceiveCallback(uint8_t *Buf, uint32_t Len) {

	// Przetwarzanie otrzymanej komendy
	Buf[Len] = '\0';
	if (message_ready == 1) {
		message_ready = 0;
		CDC_ProcessCommand((char*) Buf, Len);
	}
}

void CDC_Init(void) {
	CDC_PWM_Init();
	CDC_LED_Init();
}

void trim_command(char *trim) {
	int i, j;
	for (i = 0, j = 0; trim[i] != '\0'; i++) {
		if (trim[i] != ' ' && trim[i] != '\r' && trim[i] != '\n') {
			trim[j++] = trim[i];
		}
	}
	trim[j] = '\0';
}

int parse_command(const char *command, char *device, char *type, int *value) {
	if (!command || !device || !type || !value) {
		return -1;  // Błąd: nieprawidłowe argumenty
	}

	char buffer[100];
	strncpy(buffer, command, sizeof(buffer));
	buffer[sizeof(buffer) - 1] = '\0';  // Zapewnienie zakończenia null

	// Rozdzielanie stringa
	char *token = strtok(buffer, "_");
	if (!token)
		return -2;
	strncpy(device, token, 16);  // Kopiowanie nazwy urządzenia

	token = strtok(NULL, "_");
	if (!token)
		return -3;
	strncpy(type, token, 16);  // Kopiowanie typu urządzenia

	token = strtok(NULL, "_");
	if (!token)
		return 0;
	*value = atoi(token);  // Konwersja wartości na liczbę

	return 0;  // Sukces

}

void CDC_GPIOHandler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char *device,
		char *type, int value) {
	char response[50];
	if (value == -1) {

		// Odczyt stanu pinu
		GPIO_PinState pinState = HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
		snprintf(response, sizeof(response), "%s_%s val=%s done ok\n\r", device,
				type, pinState == GPIO_PIN_SET ? "1" : "0");
		CDC_Transmit_FS((uint8_t*) response, strlen(response));
	} else {
		// Zapis stanu pinu
		if (value == 0 || value == 1) {
			GPIO_PinState pinState =
					(value == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
			HAL_GPIO_WritePin(GPIOx, GPIO_Pin, pinState);
			snprintf(response, sizeof(response), "%s_%s done ok\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		} else {
			snprintf(response, sizeof(response), "%s_%s fail\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		}
	}
}

void CDC_POTHandler(I2C_HandleTypeDef *hi2c, uint8_t channel, char *device,
		char *type, int value) {
	char response[50];
	if (value == -1) {
		// Odczyt stanu pinu
		uint8_t valResp;
		if (AD5254_GetValue(hi2c, channel, &valResp) == HAL_OK) {
			snprintf(response, sizeof(response), "%s_%s val=%d done ok\n\r",
					device, type, valResp);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		} else {
			snprintf(response, sizeof(response), "%s_%s fail\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		}
	} else {
		if (AD5254_SetValue(hi2c, channel, value) == HAL_OK) {
			snprintf(response, sizeof(response), "%s_%s done ok\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		} else {
			snprintf(response, sizeof(response), "%s_%s fail\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		}
	}

}

void CDC_HX711Handler(I2C_HandleTypeDef *hi2c, uint8_t channel, char *device,
		char *type, int value) {
	char response[50];
	if (value == -1) {
		// Odczyt stanu pinu
		uint8_t valResp;
		if (AD5254_GetValue(hi2c, channel, &valResp) == HAL_OK) {
			snprintf(response, sizeof(response), "%s_%s val=%d done ok\n\r",
					device, type, valResp);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		} else {
			snprintf(response, sizeof(response), "%s_%s fail\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		}
	} else {
		if (AD5254_SetValue(hi2c, channel, value) == HAL_OK) {
			snprintf(response, sizeof(response), "%s_%s done ok\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		} else {
			snprintf(response, sizeof(response), "%s_%s fail\n\r", device,
					type);
			CDC_Transmit_FS((uint8_t*) response, strlen(response));
		}
	}

}

// Funkcja do przetwarzania komend otrzymywanych przez CDC
void CDC_ProcessCommand(const char *command, uint32_t Len) {
	char device[20];
	char type[20];
	int value = -1;
	char buffer[50];
// Rozdziel komendę na części
	strncpy(buffer, command, Len);
	buffer[Len] = '\0';
	trim_command(buffer);

	if (strlen(buffer) == 0) {
		//CDC_SendResponse("Error: Empty command received\n\r");
		message_ready = 1;
		return;
	}
	int errorValidation = parse_command(buffer, device, type, &value);
	if (errorValidation != 0) {
		// Wysyłanie komunikatu o błędzie
		char errorResponse[100];
		snprintf(errorResponse, sizeof(errorResponse),
				"Error:(%d) Invalid command: %s\n\r", errorValidation, buffer);
		CDC_SendResponse(errorResponse);
		message_ready = 1;
		return;
		//return parseResult; // Zwracamy kod błędu
	}

	char response[100];
	snprintf(response, sizeof(response), "%s_%s", device, type);
	response[sizeof(response)] = '\0';
// Obsługa urządzeń smc124
	if (strcmp(device, "smc124") == 0) {
		if (strcmp(type, "clk") == 0) {
			if (value == -1) {
				int16_t pwm_val = __HAL_TIM_GET_COMPARE(&htim5, TIM_CHANNEL_1);
				snprintf(response, sizeof(response), "%s_%s val=%d done ok\n\r",
									device, type, pwm_val);
				CDC_SendResponse(response);
			}else{
				__HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, value);
				snprintf(response, sizeof(response), "%s_%s val=%d done ok\n\r",
									device, type, value);
							CDC_SendResponse(response);
			}

		} else if (strcmp(type, "dir") == 0) {
			CDC_GPIOHandler(SMC124_DIR_GPIO_Port, SMC124_DIR_Pin, device, type,
					value);
		} else if (strcmp(type, "en") == 0) {
			CDC_GPIOHandler(SMC124_EN_GPIO_Port, SMC124_EN_Pin, device, type,
					value);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
// Obsługa SM1
	else if (strcmp(device, "sm1") == 0) {
		if (strcmp(type, "sd") == 0) {
			CDC_GPIOHandler(SM1_SD_GPIO_Port, SM1_SD_Pin, device, type, value);
		} else if (strcmp(type, "ccw") == 0) {
			CDC_GPIOHandler(SM1_CCW_GPIO_Port, SM1_CCW_Pin, device, type,
					value);
		} else if (strcmp(type, "cw") == 0) {
			CDC_GPIOHandler(SM1_CW_GPIO_Port, SM1_CW_Pin, device, type, value);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
// Obsługa SM2
	else if (strcmp(device, "sm2") == 0) {
		if (strcmp(type, "sd") == 0) {
			CDC_GPIOHandler(SM2_SD_GPIO_Port, SM2_SD_Pin, device, type, value);
		} else if (strcmp(type, "ccw") == 0) {
			CDC_GPIOHandler(SM2_CCW_GPIO_Port, SM2_CCW_Pin, device, type,
					value);
		} else if (strcmp(type, "cw") == 0) {
			CDC_GPIOHandler(SM2_CW_GPIO_Port, SM2_CW_Pin, device, type, value);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
// Obsluga potencjometrow
	else if (strcmp(device, "pot") == 0) {
		if (strcmp(type, "1") == 0) {
			CDC_POTHandler(&hi2c1, 0x01, device, type, value);
		} else if (strcmp(type, "2") == 0) {
			CDC_POTHandler(&hi2c1, 0x00, device, type, value);
		} else if (strcmp(type, "3") == 0) {
			CDC_POTHandler(&hi2c1, 0x03, device, type, value);
		} else if (strcmp(type, "4") == 0) {
			CDC_POTHandler(&hi2c1, 0x02, device, type, value);
		} else if (strcmp(type, "wp") == 0) {
			CDC_GPIOHandler(POT_WP_GPIO_Port, POT_WP_Pin, device, type, value);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
	// Obsługa HX711
	else if (strcmp(device, "hx") == 0) {
		if (strcmp(type, "gain") == 0) {
			HX711_SetGain(value);
			snprintf(response, sizeof(response), "%s_%s val=%d done ok\n\r",
								device, type, value);
						CDC_SendResponse(response);
		} else if (strcmp(type, "read") == 0) {
			uint32_t HX711_val = HX711_Read();
			snprintf(response, sizeof(response), "%s_%s val=%ld done ok\n\r",
					device, type, HX711_val);
			CDC_SendResponse(response);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
	// Obsługa encoderów
	else if (strcmp(device, "encoder") == 0) {
		if (strcmp(type, "1") == 0) {
			int32_t encoder_value = __HAL_TIM_GET_COUNTER(&htim2);
			snprintf(response, sizeof(response), "%s_%s val=%ld done ok\n\r",
					device, type, encoder_value);
			CDC_SendResponse(response);
		} else if (strcmp(type, "2") == 0) {
			int32_t encoder_value = __HAL_TIM_GET_COUNTER(&htim3);
			snprintf(response, sizeof(response), "%s_%s val=%ld done ok\n\r",
					device, type, encoder_value);
			CDC_SendResponse(response);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
	// Obsługa ledów
	else if (strcmp(device, "led") == 0) {
		if (strcmp(type, "blue") == 0) {
			CDC_GPIOHandler(LED1_GPIO_Port, LED1_Pin, device, type, value);
		} else if (strcmp(type, "green") == 0) {
			CDC_GPIOHandler(LED2_GPIO_Port, LED2_Pin, device, type, value);
		} else {
			snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
					device, type);
			CDC_SendResponse(response);
		}
	}
	// Obsługa zer
	else if (strcmp(device, "zero") == 0) {
			if (strcmp(type, "1") == 0) {
				CDC_GPIOHandler(ZERO1_GPIO_Port, ZERO1_Pin, device, type, value);
			} else if (strcmp(type, "2") == 0) {
				CDC_GPIOHandler(ZERO1_GPIO_Port, ZERO1_Pin, device, type, value);
			} else {
				snprintf(response, sizeof(response), "Unknown %s type %s\n\r",
						device, type);
				CDC_SendResponse(response);
			}
		}
// Obsługa innych przypadków
	else {
		snprintf(response, sizeof(response), "Unknown device %s or type %s\n\r",
				device, type);
		CDC_SendResponse(response);
	}
	message_ready = 1;
}
