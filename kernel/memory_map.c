#include <stdint.h>
#include "boot_const.h"
#include "display.h"
#include "memory_map.h"

 

void memory_map_print(){
	Memmap_t * map = ( Memmap_t *) (MEMORY_MAP_BUFFER1);
	print_cstring("MEM MAP \n");
	print_cstring("low add| high add| low size| high size | status\n");
	for(int  i = 0;i<MEMORY_MAP_BUFFER_MAX_ENTRIES;i++){
		

		vga_print_hex(map[i].ladd);
		print_cstring(" ");

		vga_print_hex(map[i].hadd);
		print_cstring(" ");

		vga_print_hex(map[i].llen);
		print_cstring(" ");

		vga_print_hex(map[i].hlen);
		print_cstring(" ");

		vga_print_hex(map[i].type);
		print_cstring("\n");

	}
}


Memmap_t* memory_map_largest32(){
	Memmap_t * map = ( Memmap_t *) (MEMORY_MAP_BUFFER1);
	
	Memmap_t* ret = map;
	for(int  i = 1;i<MEMORY_MAP_BUFFER_MAX_ENTRIES;i++){
		// if(ret ->hlen < map[i].hlen)
		// {
		// 	ret = &map[i];
		// }else 
		if(ret->llen < map[i].llen)
		{
			ret = &map[i];
		}
		
	}
	return ret;
}

Memmap_t* memory_map_highest32(){
	Memmap_t * map = ( Memmap_t *) (MEMORY_MAP_BUFFER1);
	
	Memmap_t* ret = memory_map_largest32();
	
	for(int  i = 0;i<MEMORY_MAP_BUFFER_MAX_ENTRIES;i++){
		// if(ret ->hadd < map[i].hadd)
		// {
		// 	ret = &map[i];
		// }else 
		if(ret->ladd + ret->llen ==   map[i].ladd)
		{
			ret = &map[i];
		}
		
	}
	return ret;
}