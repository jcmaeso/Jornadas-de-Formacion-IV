/*
 * simon.c
 *
 *  Created on: 26 ago. 2019
 *      Author: jcala
 */
#include "simon.h"
#include "simon_LL.h"
#include <stdlib.h>
#include <stdint.h>
#include "uart_printf.h"

static void simon_generate_random_sequence(simon_t* game_data);
static void simon_display_leds(simon_t* game_data);
static void simon_check_user_sequence(simon_t* game_data);

static int input_condition_func_start_level(fsm_t* fsm);
static int input_condition_launch_level(fsm_t* fsm);
static int input_condition_check_sequence_len(fsm_t* fsm);
static int input_condition_func_user_sequence_right(fsm_t* fsm);
static int input_condition_func_user_sequence_wrong(fsm_t* fsm);

static void  output_func_update_level(fsm_t* fsm);
static void  output_func_launch_level(fsm_t* fsm);
static void  output_func_restore(fsm_t* fsm);
static void  output_func_check_sequence(fsm_t* fsm);



static fsm_trans_t simon_transition_table[] = {
		{WAIT_START, input_condition_func_start_level,START_LEVEL,output_func_update_level},
		{START_LEVEL,input_condition_launch_level,WAIT_USER_SEQUENCE,output_func_launch_level},
		{WAIT_USER_SEQUENCE,input_condition_check_sequence_len,CHECK_USER_SEQUENCE,output_func_check_sequence},
		{CHECK_USER_SEQUENCE,input_condition_func_user_sequence_right,START_LEVEL,output_func_update_level},
		{CHECK_USER_SEQUENCE,input_condition_func_user_sequence_wrong,WAIT_START,output_func_restore},
		{-1, NULL, -1, NULL },
};

static int input_condition_func_start_level(fsm_t* fsm){
	simon_t* game_data = (simon_t*)fsm;
	return (game_data->game_flags & SIMON_GAME_ON);
}

static int input_condition_launch_level(fsm_t* fsm){
	return 1;
}

static int input_condition_check_sequence_len(fsm_t* fsm){
	simon_t* game_data = (simon_t*)fsm;
	return game_data->user_sequence_position >= game_data->level;
}


static int input_condition_func_user_sequence_right(fsm_t* fsm){
	simon_t* game_data = (simon_t*)fsm;
	return (game_data->game_flags & SIMON_SECUENCE_RIGHT);
}

static int input_condition_func_user_sequence_wrong(fsm_t* fsm){
	simon_t* game_data = (simon_t*)fsm;
	return !(game_data->game_flags & SIMON_SECUENCE_RIGHT);
}


static void  output_func_update_level(fsm_t* fsm){
	uart_printf("START_LEVEL \n");
	simon_t* game_data = (simon_t*)fsm;
	game_data->level++;
	game_data->game_flags ^= SIMON_SECUENCE_RIGHT;
	simon_generate_random_sequence(game_data);
}

static void  output_func_launch_level(fsm_t* fsm){
	uart_printf("WAIT_USER_SEQUENCE! \n");
	simon_t* game_data = (simon_t*)fsm;
	simon_display_leds(game_data);
	game_data->game_flags |= SIMON_LISTEN_TO_SECUENCE;
}

static void  output_func_check_sequence(fsm_t* fsm){
	uart_printf("WAIT_USER_SEQUENCE! \n");
	simon_t* game_data = (simon_t*)fsm;
	game_data->game_flags ^= SIMON_LISTEN_TO_SECUENCE;
	game_data->user_sequence_position = 0;
	simon_check_user_sequence(game_data);
}

static void  output_func_restore(fsm_t* fsm){
	uart_printf("WAIT_START! \n");
	simon_t* game_data = (simon_t*)fsm;
	game_data->level = 0;
	game_data->game_flags = 0;
}

static void simon_generate_random_sequence(simon_t* game_data){
	int i;
	for(i = 0; i < game_data->level; i++)
		game_data->led_sequence[i] = nrand48(RTC_get_tick())%3;
}

static void simon_display_leds(simon_t* game_data){
	for(int i = 0; i < game_data->level; i++){
		LED_display(game_data->led_sequence[i],game_data->display_speed);
	}
	LED_toggle_effect();
}

static void simon_check_user_sequence(simon_t* game_data){
	int i;
	for(i = 0; i < game_data->level;i++){
		if(game_data->user_sequence[i] != game_data->led_sequence[i]){
			return;
		}
	}
	game_data->game_flags |= SIMON_SECUENCE_RIGHT;
}

simon_t* simon_init(long int random_seed){
	static simon_t game_data[1];
	game_data->fsm.tt = simon_transition_table;
	game_data->level = 0;
	game_data->game_flags = SIMON_GAME_ON;
	game_data->display_speed = 2;
	game_data->_IRQ_BTN_handler = simon_LL_init(&game_data->game_flags,game_data->user_sequence,&game_data->user_sequence_position);
	return game_data;
}

