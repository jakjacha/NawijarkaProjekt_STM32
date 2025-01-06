#ifndef CDC_CONTROL_H
#define CDC_CONTROL_H


#include "usbd_cdc_if.h"

// Deklaracje zmiennych globalnych
#define PWM_CHANNELS 2
extern uint16_t pwm_values[PWM_CHANNELS];

// Inicjalizacja CDC
void CDC_Init(void);

// Inicjalizacja PWM na odpowiednich kanałach
void CDC_PWM_Init(void);

// Ustawienie wypełnienia PWM dla odpowiedniego kanału
void CDC_SetPWMDutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel, uint16_t duty_cycle, int8_t PWMPort);

// Odczytanie wypełnienia PWM dla odpowiedniego kanału
void CDC_GetPWMDutyCycle(TIM_HandleTypeDef *htim, uint32_t Channel, uint8_t PWMPort);

// Inicjalizacja pinów LED
void CDC_LED_Init(void);

// Przetwarzanie komend odebranych przez USB CDC
void CDC_ProcessCommand(const char* command, uint32_t Len);

void CDC_GPIOHandler(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin, char *device, char *type, int value);

// Callback dla odbioru danych przez USB CDC
void CDC_ReceiveCallback(uint8_t* Buf, uint32_t Len);

int parse_command(const char *command, char *device, char *type, int *value);

void trim_command(char *command);

#endif /* CDC_CONTROL_H */
