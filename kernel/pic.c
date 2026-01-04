#include <stdint.h>
#include "ports.h"
#include "pic.h"

 

void PIC_sendEOI(uint8_t irq)
{
	if(irq >= 8)
		port_8_out(PIC2_COMMAND,PIC_EOI);
	
	port_8_out(PIC1_COMMAND,PIC_EOI);
}

void PIC_remap(int offset1, int offset2)
{

	/*
	Master PIC	0 to 7	0x08	0x08 to 0x0F
	Slave PIC	8 to 15	0x70	0x70 to 0x77

	*/
	port_8_out(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	
	port_8_out(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	
	port_8_out(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	
	port_8_out(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	
	port_8_out(PIC1_DATA, 1 << CASCADE_IRQ);        // ICW3: tell Master PIC that there is a slave PIC at IRQ2
	
	port_8_out(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	
	
	port_8_out(PIC1_DATA, ICW4_8086);               // ICW4: have the PICs use 8086 mode (and not 8080 mode)
	
	port_8_out(PIC2_DATA, ICW4_8086);
	

	// Unmask both PICs.
	port_8_out(PIC1_DATA, 0);
	port_8_out(PIC2_DATA, 0);
}

void PIC_disable(void) {
    port_8_out(PIC1_DATA, 0xff);
    port_8_out(PIC2_DATA, 0xff);
}

void PIC_IRQ_set_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_8_in(port) | (1 << IRQline);
    port_8_out(port, value);        
}

void PIC_IRQ_clear_mask(uint8_t IRQline) {
    uint16_t port;
    uint8_t value;

    if(IRQline < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        IRQline -= 8;
    }
    value = port_8_in(port) & ~(1 << IRQline);
    port_8_out(port, value);        
}


 