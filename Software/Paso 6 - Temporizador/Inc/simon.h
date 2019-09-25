#include <stdint.h>
#include "simon_flags.h"
#include "fsm.h"

typedef void (*IRQ_fcn_t)(void*);


typedef enum fsm_state {
	WAIT_START = 0,
	START_LEVEL = 1,
	WAIT_USER_SEQUENCE = 2,
	CHECK_USER_SEQUENCE = 3,
}simon_fsm_state_t;

typedef struct simon{
	fsm_t fsm;
	int level;
	int display_speed;
	uint8_t game_flags;
	int led_sequence[SIMON_MAX_LEVEL];
	int user_sequence[SIMON_MAX_LEVEL];
	int user_sequence_position;
	IRQ_fcn_t* _IRQ_Handlers;
}simon_t;

simon_t* simon_init(void* htim, long int random_seed);



