#pragma once
#include <stdint.h>

//#include "types.h"


/*
	for frequencies > 1000Hz
*/

#define TIMER_PIC_BASE_FREQUENCY 1193182
#define TIMER_PIC_FREQUENCY 50 // 82 = around 1 second
#define TIMER_DIVISOR (TIMER_PIC_BASE_FREQUENCY / TIMER_PIC_FREQUENCY) //14551
 


 

extern uint32_t timer_tick ; 

extern uint32_t timer_total_seconds ; 

extern uint32_t timer_wait_seconds ;



void timer_init(uint32_t freq);


uint32_t timer_get_seconds();

 
 






