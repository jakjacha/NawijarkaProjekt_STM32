#ifndef HX711_H
#define HX711_H

#include "stm32f4xx_hal.h"


// Funkcje obsługi HX711
uint32_t HX711_Read(void);
void HX711_SetGain(uint8_t gain);

#endif /* HX711_H */
