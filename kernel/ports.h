#pragma once
#include <stdint.h>


uint8_t port_8_in (uint16_t port) ;

void port_8_out (uint16_t port, uint8_t data);

uint16_t port_16_in (uint16_t port) ;

void port_16_out (uint16_t port, uint16_t data) ;

 
uint16_t port_32_in(uint16_t port) ;

void port_32_out (uint16_t port, uint32_t data) ;
