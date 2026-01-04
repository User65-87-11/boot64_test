#pragma once
#include "ports.h"
#include <stdint.h>


#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC



uint16_t pciConfigReadWord(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) ;