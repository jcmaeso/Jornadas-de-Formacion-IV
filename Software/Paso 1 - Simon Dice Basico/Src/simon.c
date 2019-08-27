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


static void simon_generate_random_sequence(simon_t* game_data);
static void simon_display_leds(simon_t* game_data);
static void simon_check_user_sequence(simon_t* game_data);


simon_t* simon_init(long int random_seed){
	static simon_t game_data[1];
	srand48(random_seed);
	game_data->level = 0;
	game_data->game_flags = SIMON_GAME_ON;
	game_data->display_speed = 2;
	game_data->_IRQ_BTN_handler = simon_LL_init(&game_data->game_flags,game_data->user_sequence,&game_data->user_sequence_position);
	return game_data;
}

void simon_reset(simon_t* game_data){
	game_data->level = 0;
	game_data->game_flags = SIMON_GAME_ON;
}

void simon_play(simon_t* game_data){
	game_data->level = 0;
	while(game_data->game_flags & SIMON_GAME_ON){
		game_data->level++;
		simon_generate_random_sequence(game_data);
		simon_display_leds(game_data);
		game_data->game_flags |= SIMON_LISTEN_TO_SECUENCE;
		while(game_data->user_sequence_position < game_data->level);
		game_data->game_flags ^= SIMON_LISTEN_TO_SECUENCE;
		simon_check_user_sequence(game_data);
		game_data->user_sequence_position = 0;
	}
}


static void simon_generate_random_sequence(simon_t* game_data){
	int i;
	for(i = 0; i < game_data->level; i++)
		game_data->led_sequence[i] = lrand48()%3;
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
			game_data->game_flags ^= SIMON_GAME_ON;
		}
	}
}

