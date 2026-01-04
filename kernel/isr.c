 

#include <stdint.h>

#include "idt.h"
#include "ports.h"
#include "display.h"

 
#include "display.h"
#include "isr.h"
#include "mem.h"

typedef struct registers registers_t;



 
 
/* Struct which aggregates many registers */







isr_t interrupt_handlers[256];

/* Can't do this with a loop because we need the address
 * of the function names */
void isr_install() {
	

    set_idt_gate32(0, (uint32_t)isr0);
    set_idt_gate32(1, (uint32_t)isr1);
    set_idt_gate32(2, (uint32_t)isr2);
    set_idt_gate32(3, (uint32_t)isr3);
    set_idt_gate32(4, (uint32_t)isr4);
    set_idt_gate32(5, (uint32_t)isr5);
    set_idt_gate32(6, (uint32_t)isr6);
    set_idt_gate32(7, (uint32_t)isr7);
    set_idt_gate32(8, (uint32_t)isr8);
    set_idt_gate32(9, (uint32_t)isr9);
    set_idt_gate32(10, (uint32_t)isr10);
    set_idt_gate32(11, (uint32_t)isr11);
    set_idt_gate32(12, (uint32_t)isr12);
    set_idt_gate32(13, (uint32_t)isr13);
    set_idt_gate32(14, (uint32_t)isr14);
    set_idt_gate32(15, (uint32_t)isr15);
    set_idt_gate32(16, (uint32_t)isr16);
    set_idt_gate32(17, (uint32_t)isr17);
    set_idt_gate32(18, (uint32_t)isr18);
    set_idt_gate32(19, (uint32_t)isr19);
    set_idt_gate32(20, (uint32_t)isr20);
    set_idt_gate32(21, (uint32_t)isr21);
    set_idt_gate32(22, (uint32_t)isr22);
    set_idt_gate32(23, (uint32_t)isr23);
    set_idt_gate32(24, (uint32_t)isr24);
    set_idt_gate32(25, (uint32_t)isr25);
    set_idt_gate32(26, (uint32_t)isr26);
    set_idt_gate32(27, (uint32_t)isr27);
    set_idt_gate32(28, (uint32_t)isr28);
    set_idt_gate32(29, (uint32_t)isr29);
    set_idt_gate32(30, (uint32_t)isr30);
    set_idt_gate32(31, (uint32_t)isr31);

// // Remap the PIC
//     port_8_out(0x20, 0x11);
//     port_8_out(0xA0, 0x11);

//     port_8_out(0x21, 0x20);
//     port_8_out(0xA1, 0x28);
// //--REMAP END
//     port_8_out(0x21, 0x04);
//     port_8_out(0xA1, 0x02);

//     port_8_out(0x21, 0x01);
//     port_8_out(0xA1, 0x01);

// 	//// Unmask both PICs.
//     port_8_out(0x21, 0x0);
//     port_8_out(0xA1, 0x0); 

    // Install the IRQs
    set_idt_gate32(32, (uint32_t)irq0);
    set_idt_gate32(33, (uint32_t)irq1);
    set_idt_gate32(34, (uint32_t)irq2);
    set_idt_gate32(35, (uint32_t)irq3);
    set_idt_gate32(36, (uint32_t)irq4);
    set_idt_gate32(37, (uint32_t)irq5);
    set_idt_gate32(38, (uint32_t)irq6);
    set_idt_gate32(39, (uint32_t)irq7);
    set_idt_gate32(40, (uint32_t)irq8);
    set_idt_gate32(41, (uint32_t)irq9);
    set_idt_gate32(42, (uint32_t)irq10);
    set_idt_gate32(43, (uint32_t)irq11);
    set_idt_gate32(44, (uint32_t)irq12);
    set_idt_gate32(45, (uint32_t)irq13);
    set_idt_gate32(46, (uint32_t)irq14);
    set_idt_gate32(47, (uint32_t)irq15);


 	set_idt32();  
}

/* To print the message which defines every exception */
char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isr_handler(registers_t r) {
    print_cstring("received interrupt: ");
    char s[14];
    itoa(r.int_no, s);
    print_cstring(s);
    print_cstring("\n");
    print_cstring(exception_messages[r.int_no]);
    print_cstring("\n");
}


void register_interrupt_handler(uint8_t n, isr_t handler) {
    interrupt_handlers[n] = handler;
}

void irq_handler(registers_t r) {
    /* After every interrupt we need to send an EOI to the PICs
     * or they will not send another interrupt again */
    if (r.int_no >= 40) 
		port_8_out(0xA0, 0x20); /* slave */
    port_8_out(0x20, 0x20); /* master */

    /* Handle the interrupt in a more modular way */
    if (interrupt_handlers[r.int_no] != 0) {
        isr_t handler = interrupt_handlers[r.int_no];
        handler(r);
    }
}

 