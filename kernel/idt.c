
#include <stdint.h>
#include "boot_const.h"
#include "boot_const2.h"
#include "mem.h"
#include "idt.h"
 
 

 
 

void set_idt_gate32(int n, uint32_t handler) {
	// InterruptDescriptor32_t*	idt =  ((InterruptDescriptor32_t *) INTERRUPT_TABLE_32) + (sizeof(InterruptDescriptor32_t) * n);
	 

    // idt->offset_1 = low_16(handler);
    // idt->selector = CODE_SEG;
    // idt->zero = 0;
    // idt->type_attributes = 0x8E; 
    // idt->offset_2 = high_16(handler);

	InterruptDescriptor32_t*	idt =  ((InterruptDescriptor32_t *) INTERRUPT_TABLE_32);

	idt[n].offset_1 = low_16(handler);
    idt[n].selector = CODE_SEG;
    idt[n].zero = 0;
    idt[n].type_attributes = 0x8E; 
    idt[n].offset_2 = high_16(handler);
	
}

void set_idt32() {
	IdtRegister * idtr = (IdtRegister * )(&boot_globals->IDT_DESCR);
	mem_set32(idtr, 0, sizeof(IdtRegister));
	idtr->base = INTERRUPT_TABLE_32;
    idtr->limit = 256 * sizeof(InterruptDescriptor32_t) - 1;
 
    /* Don't make the mistake of loading &idt -- always load &idt_reg */
    __asm__ __volatile__("lidt (%0)" : : "r" (idtr));
}

 

/*

[External events] trigger an interrupt 
	- Interrupt Service Routine (ISR) is called
	- [hardwar interrupts]
		- Triggered by hardware
	- [software interrupts]
		- triggered by INT XXh

[ISR]
	- called directly by the CPU
	- has to end with [iret]/[iretq] opcode

Keyboard triggers IRQ 1
*/