
 
 
#include <stdint.h>

#include "display.h"
#include "mem.h"
#include "isr.h"
#include "boot_const.h"
#include "memory_map.h"
#include "pic.h"

#include "timer.h"
#include "keyboard.h"

#include "cmd.h"
#include "ports.h"
#include "loop.h"
#include "rsdt.h"
#include "boot_const2.h"
 
 

// #define MEMORY_MAP_NUM_ENTRUES 10
// #define MEMORY_MAP_BUFFER  (0x7000 + 16)
// #define MEMORY_MAP_MARGEST_POINTER  0x7000 + 16 + (24 * MEMORY_MAP_NUM_ENTRUES)


// MEMORY_MAP_BUFFER equ GLOBALS  + 16
// 	MEMORY_MAP_BUFFER_SIZE_OFFSET equ GLOBALS
// 	MEMORY_MAP_BUFFER_ENTRY_SIZE equ 24
// 	MEMORY_MAP_BUFFER_MAX_ENTRIES equ 20

// MEMORY_MAP_LARGEST_ENTRY_OFFSET equ MEMORY_MAP_BUFFER + (MEMORY_MAP_BUFFER_ENTRY_SIZE * MEMORY_MAP_BUFFER_MAX_ENTRIES)

void loop();



struct Boot_globals * boot_globals;

void kernel_main(){
	boot_globals = (struct Boot_globals*) GLOBALS;
	boot_globals-> mem_map_start = (Memmap_t *) MEMORY_MAP_BUFFER1;
	boot_globals->mem_map_long_entry_pos = memory_map_largest32();
	boot_globals->mem_map_high_entry_pos = memory_map_highest32();
	boot_globals->RSDP = rsdt_find_rsdp32();
	

	


	PIC_remap(PIC_VEC_OFFSET_MASTER_REMAPPED, PIC_VEC_OFFSET_SLAVE_REMAPPED);
	isr_install();
	cmd_init();
	
	// divisor 14551
	timer_init(TIMER_PIC_FREQUENCY);
	init_keyboard();
	kb_set_scancode_handler(cmd_key_callback);
	
	__asm__ volatile("sti");

 

 
	 

	clear_scr(0x0a);
	set_cur_pos(0);
	print_cstring("Protected mode\n");

	boot_globals->ECM = rsdt_find_ecm32(boot_globals->RSDP);

	print_cstring_len(boot_globals->ECM->header.Signature,4);
	print_cstring("@");
	vga_print_hex((uint32_t)boot_globals->ECM);
	print_cstring("\n");

	
	cmd_print_cmd();
	// create_interrupts_table();

  

	halt_loop();
	// loop();
}
 