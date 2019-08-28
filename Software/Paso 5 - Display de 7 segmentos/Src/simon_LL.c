#include "simon_LL.h"
#include "stm32f0xx_hal.h"
#include "simon_flags.h"
#include <stdint.h>

#define SIMON_BTN_1 0
#define SIMON_BTN_2 1
#define SIMON_BTN_3 2

#define N_LEDS 3


typedef void (*DISPLAY_SHOW_fcn_t)(void);


static void BTN_interrupt_handler(uint16_t GPIO_PIN);
static void Display_clear();

static void Display_show_zero();
static void Display_show_one();
static void Display_show_two();
static void Display_show_three();
static void Display_show_four();
static void Display_show_five();
static void Display_show_six();
static void Display_show_seven();
static void Display_show_eigth();
static void Display_show_nine();
void Display_show_e();
void Display_show_p();
void Display_show_a();



static uint8_t* flags;
static int* user_sequence;
static int* user_sequence_position;
static GPIO_TypeDef* GPIO_bank_list[N_LEDS];
static uint16_t GPIO_pin_list[N_LEDS];
static DISPLAY_SHOW_fcn_t show_fcns[10];

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
	show_fcns[0] = Display_show_zero;
	show_fcns[1] = Display_show_one;
	show_fcns[2] = Display_show_two;
	show_fcns[3] = Display_show_three;
	show_fcns[4] = Display_show_four;
	show_fcns[5] = Display_show_five;
	show_fcns[6] = Display_show_six;
	show_fcns[7] = Display_show_seven;
	show_fcns[8] = Display_show_eigth;
	show_fcns[9] = Display_show_nine;
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
		for(j = 0; j < N_LEDS; j++)
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

/*
 * 7 Segment Display
 * */



static void Display_clear(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET); //Segmento 3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); //Segmento Punto
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_RESET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); //Segmento 7
}

static void Display_show_zero(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

static void Display_show_one(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
}

static void Display_show_two(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

static void Display_show_three(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
}

static void Display_show_four(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
}

static void Display_show_five(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
}

static void Display_show_six(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); //Segmento Punto
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

static void Display_show_seven(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
}

static void Display_show_eigth(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

static void Display_show_nine(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
}

void Display_show_e(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 4
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

void Display_show_p(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

void Display_show_a(){
	Display_clear();
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 1
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Segmento 2
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); //Segmento 3
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET); //Segmento 5
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); //Segmento 6
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET); //Segmento 7
}

void Display_number(int number){
	if(number > 9 || number < 0){
		return; //TODO: Error Handling
	}
	show_fcns[number]();
}

void Display_ALL(){
	Display_show_zero();
	Display_clear();
	Display_show_one();
	Display_clear();
	Display_show_two();
	Display_clear();
	Display_show_three();
	Display_clear();
	Display_show_four();
	Display_clear();
	Display_show_five();
	Display_clear();
	Display_show_six();
	Display_clear();
	Display_show_seven();
	Display_clear();
	Display_show_eigth();
	Display_clear();
	Display_show_nine();
	Display_clear();
	Display_show_e();
	Display_clear();
	Display_show_p();
	Display_clear();
	Display_show_a();
	Display_clear();
}



