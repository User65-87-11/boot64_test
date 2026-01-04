#pragma once

#include <stdint.h>
#include "boot_const.h"



typedef struct Memmap{
	uint32_t ladd;
	uint32_t hadd;
	uint32_t llen;
	uint32_t hlen;
	uint32_t type;
	uint32_t acpi;

} Memmap_t;


 
void memory_map_print();

Memmap_t*  memory_map_largest32();

Memmap_t *  memory_map_highest32();