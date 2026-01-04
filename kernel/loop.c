#include <stdint.h>


#include "ports.h"
#include "display.h"
#include "timer.h"
#include "loop.h"
#include "mem.h"


 
void halt_loop(){
 
	while(true)
	{ 
		
		__asm__ volatile("hlt");
	}
}

void halt_cli(){
 
	
	__asm__ volatile("cli");
	 print_cstring(" Terminated\n");
	__asm__ volatile("hlt");
	 
}