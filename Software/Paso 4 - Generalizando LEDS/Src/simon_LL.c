#include "simon_LL.h"
#include "stm32f0xx_hal.h"
#include "simon_flags.h"
#include <stdint.h>

#define SIMON_BTN_1 0
#define SIMON_BTN_2 1
#define SIMON_BTN_3 2

#define N_LEDS 3

static void BTN_interrupt_handler(uint16_t GPIO_PIN);

static uint8_t* flags;
static int* user_sequence;
static int* user_sequence_position;
static GPIO_TypeDef* GPIO_bank_list[N_LEDS];
static uint16_t GPIO_pin_list[N_LEDS];

IRQ_fcn_t simon_LL_init(uint8_t* _flags, int* _user_sequence, int* _user_sequence_position){
	flags = _flags;
	user_sequence = _user_sequence;
	user_sequence_position = _user_sequence_position;
	//Inicializamos las listas con el orden de los GPIO que queremos
	GPIO_bank_list[0] = GPIOB;
	GPIO_pin_list[0] = GPIO_PIN_9;
	GPIO_bank_list[1] = GPIOB;
	GPIO_pin_list[1] = GPIO_PIN_8;
	GPIO_bank_list[2] = GPIOA;
	GPIO_pin_list[2] = GPIO_PIN_5;
	return BTN_interrupt_handler;
}

void LED_display(int led_number,int display_speed){
	HAL_GPIO_WritePin(GPIO_bank_list[led_number],GPIO_pin_list[led_number],GPIO_PIN_SET);
	HAL_Delay(2000/display_speed);
	HAL_GPIO_WritePin(GPIO_bank_list[led_number],GPIO_pin_list[led_number],GPIO_PIN_RESET);
	HAL_Delay(100);
}

void LED_toggle_effect(){
	int i,j;
	for(i = 0; i < 2; i++){
		HAL_Delay(200);
		for(j = 0; j < N_LEDS; j++)
			HAL_GPIO_WritePin(GPIO_bank_list[j],GPIO_pin_list[j],GPIO_PIN_SET);
		HAL_Delay(200);
		for(j = 0; j < 2; j++)
			HAL_GPIO_WritePin(GPIO_bank_list[j],GPIO_pin_list[j],GPIO_PIN_RESET);
	}
}

void LED_win_effect(){
	int i,j;
	for(i = 0; i < 2; i++){
		for(j = 0; j < N_LEDS; j++){
			HAL_GPIO_WritePin(GPIO_bank_list[j],GPIO_pin_list[j],GPIO_PIN_SET);
			HAL_Delay(200);
		}
		for(j = 2; j >= 0; j--){
			HAL_GPIO_WritePin(GPIO_bank_list[j],GPIO_pin_list[j],GPIO_PIN_RESET);
			HAL_Delay(200);
		}
	}
}

unsigned short int* RTC_get_tick(){
	static unsigned short int tick;
	tick = HAL_GetTick();
	return &tick;
}

static void BTN_interrupt_handler(uint16_t GPIO_PIN){
	if(!((*flags) & SIMON_GAME_ON)){
		(*flags) |= SIMON_GAME_ON;
		return;
	}
	if((*flags) & SIMON_LISTEN_TO_SECUENCE && (*user_sequence_position) < SIMON_MAX_LEVEL){
		switch(GPIO_PIN){
		case GPIO_PIN_4:
			user_sequence[*(user_sequence_position)] = SIMON_BTN_1;
			break;
		case GPIO_PIN_5:
			user_sequence[*(user_sequence_position)] = SIMON_BTN_2;
			break;
		case GPIO_PIN_3:
			user_sequence[*(user_sequence_position)] = SIMON_BTN_3;
			break;
		default:
			return;
		}
		*(user_sequence_position) += 1;
	}
}



