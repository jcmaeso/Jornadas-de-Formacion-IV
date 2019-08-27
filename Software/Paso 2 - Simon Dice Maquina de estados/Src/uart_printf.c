#include "uart_printf.h"

#include <string.h>
#include "stm32f0xx_hal.h"

extern UART_HandleTypeDef huart2;

void uart_printf(char* str){
	HAL_UART_Transmit(&huart2, (uint8_t*)str, strlen(str), 0xFFFF);
}
