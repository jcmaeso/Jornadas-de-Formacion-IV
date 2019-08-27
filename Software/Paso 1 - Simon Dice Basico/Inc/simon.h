#include <stdint.h>
#include "simon_flags.h"


typedef struct simon{
	int level;
	int display_speed;
	uint8_t game_flags;
	int led_sequence[SIMON_MAX_LEVEL];
	int user_sequence[SIMON_MAX_LEVEL];
	int user_sequence_position;
	void (*_IRQ_BTN_handler)(uint16_t);
}simon_t;

simon_t* simon_init(long int random_seed);
void simon_play(simon_t* game_data);
void simon_reset(simon_t* game_data);



