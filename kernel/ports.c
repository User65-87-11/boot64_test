 
#include <stdint.h>

 

 
uint8_t port_8_in (uint16_t port) {
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void port_8_out (uint16_t port, uint8_t data) {

    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t port_16_in (uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void port_16_out (uint16_t port, uint16_t data) {
    __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
 
uint32_t port_32_in (uint16_t port) {
    uint32_t result;
    __asm__("in %%dx, %%eax" : "=a" (result) : "d" (port));
    return result;
}

void port_32_out (uint16_t port, uint32_t data) {
    __asm__("out %%eax, %%dx" : : "a" (data), "d" (port));
}
 