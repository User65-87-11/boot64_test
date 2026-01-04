#pragma once
#include <stdint.h>
#include "memory_map.h"
#include "rsdt.h"

typedef struct GDT_DESCR GDT_DESCR; 

typedef struct IDT_DESCR IDT_DESCR; 


struct GDT_DESCR{
	uint16_t gdt_size;
	uint32_t gdt_addr_low;
	uint32_t gdt_addr_high;

};

struct IDT_DESCR{
	uint16_t gdt_size;
	uint32_t gdt_addr_low;
	uint32_t gdt_addr_high;

};

struct Boot_globals{

	GDT_DESCR GDT_DESCR;
	IDT_DESCR IDT_DESCR;

	Memmap_t * mem_map_start; 
	Memmap_t * mem_map_long_entry_pos;
	Memmap_t *  mem_map_high_entry_pos;

	//somewhere higher at the end


	//Enhanced Configuration Mechanism
	ACPI_Table * ECM;

	//0x000E0000 to 0x000FFFFF @Extended BIOS Data Area "RSD PTR "(8)
	RSDP * RSDP;

	
	RSDT * RSDT;
 
};

extern struct Boot_globals* boot_globals;