#include "hx711.h"
#include "main.h"
// Domyślny gain (128 dla kanału A)
static uint8_t HX711_gain = 128;

/**
 * @brief Inicjalizacja HX711.
 */

/**
 * @brief Ustawia wzmocnienie HX711 (GAIN).
 * @param gain Wartość wzmocnienia: 128, 64, lub 32.
 */
void HX711_SetGain(uint8_t gain) {
    HX711_gain = gain;
}

/**
 * @brief Odczytuje wartość z HX711.
 * @retval Odczytana wartość 24-bitowa.
 */
uint32_t HX711_Read(void) {
    uint32_t data = 0;
    uint32_t iterator =0;
    // Czekaj na gotowość HX711 (DT == 0)
   while (HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port, HX711_DOUT_Pin) == GPIO_PIN_SET || iterator>=100){
	   iterator++;
	   HAL_Delay(10);
   }

    // Odczytaj 24 bity danych
    for (int i = 0; i < 24; i++) {
        HAL_GPIO_WritePin(HX711_SCK_GPIO_Port, HX711_SCK_Pin, GPIO_PIN_SET);
        data = (data << 1) | HAL_GPIO_ReadPin(HX711_DOUT_GPIO_Port, HX711_DOUT_Pin);
        HAL_GPIO_WritePin(HX711_SCK_GPIO_Port, HX711_SCK_Pin, GPIO_PIN_RESET);
    }

    // Dodatkowe impulsy zegara dla ustawienia wzmocnienia
    for (int i = 0; i < (HX711_gain == 128 ? 1 : (HX711_gain == 64 ? 3 : 2)); i++) {
        HAL_GPIO_WritePin(HX711_SCK_GPIO_Port, HX711_SCK_Pin, GPIO_PIN_SET);
        HAL_GPIO_WritePin(HX711_SCK_GPIO_Port, HX711_SCK_Pin, GPIO_PIN_RESET);
    }

    // Konwersja na wartość 24-bitową signed
    if (data & 0x800000) {
        data |= 0xFF000000;  // Rozszerzenie znaku dla wartości ujemnych
    }

    return data;
}
