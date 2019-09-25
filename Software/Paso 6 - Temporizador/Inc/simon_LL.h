#include <stdint.h>

typedef void (*IRQ_fcn_t)(void*);
typedef void (*PRINT_fcn_t)(void);

IRQ_fcn_t* simon_LL_init(void* _htim,uint8_t* _flags, int* _user_sequence, int* _user_sequence_position);
void LED_display(int led_number,int display_speed);
void LED_toggle_effect();
void LED_win_effect();

void Display_ALL();
void Display_number(int number);
void Display_show_e();
void Display_show_p();
void Display_show_a();
unsigned short int* RTC_get_tick();

void TIM1_launch();
