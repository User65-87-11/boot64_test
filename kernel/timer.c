 
#include <stdint.h>
 
 
#include "display.h"
#include "mem.h"
#include "isr.h"
#include "timer.h"
#include "ports.h"

 
 



uint32_t timer_tick ; 

uint32_t timer_total_seconds ; 

uint32_t timer_wait_seconds ;


 
//c23c
//418B - 416C = 1F
// file visibility
static void timer_callback( registers_t regs) {
    timer_tick++;
  

 
}

uint32_t timer_get_seconds(){
	return  timer_total_seconds;
}

 

void timer_init(uint32_t freq) {
    /* Install the function we just wrote */
	 
    register_interrupt_handler(IRQ0, timer_callback);

	
	/*
 
		1193182 / div = freq

		1193182 / 14551	 = 82

		1193182 / freq = div
	

	*/

	
 

    /* Get the PIT value: hardware clock at 1193182 Hz */

	uint32_t divisor = 1193182 / freq;
	 
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);
    /* Send the command */
    port_8_out(0x43, 0x34); /* Command port */
	

    port_8_out(0x40, low);
    port_8_out(0x40, high);


	 
}

 