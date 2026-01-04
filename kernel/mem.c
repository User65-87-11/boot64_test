 
#include <stdint.h>
#include "mem.h"
 
 
uint32_t free_mem_addr = -1;//fix this

uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr) {
    /* Pages are aligned to 4K, or 0x1000 */
    if (align == 1 && (free_mem_addr & 0x0FFF)) {
        free_mem_addr &= 0xFFFFF000; // find a base and add to the new alignment
        free_mem_addr += 0x1000;
    }
    /* Save also the physical address */
    if (phys_addr) *phys_addr = free_mem_addr;

    uint32_t ret = free_mem_addr;
    free_mem_addr += size; /* Remember to increment the pointer */
    return ret;
}

void itoa(int value,char  buff[12]){
// buffer size 
	#define I_BUFF_SIZE 12	
// 	
	
	int val;
 
    int negative;
    char buffer[I_BUFF_SIZE];
    char *pos;
    int digit;
	mem_set32(buff,0,I_BUFF_SIZE);
 
    if (value < 0 ) {
    	negative = 1;
        val = -value;
    } else {
    	negative = 0;
        val = value;
	 
    } /* if */
 
    pos = &buffer[I_BUFF_SIZE - 1];
    *pos = '\0';
                               
    do {
   	 	digit = val % 10;
    	val = val / 10;
		*--pos = '0' + digit;
		
		 
    } while (val != 0L);
 
    if (negative) {
    *--pos = '-';
    } /* if */

	int num  = &buffer[I_BUFF_SIZE - 1] - pos ;
	for(int i=0;i< num; i++){
		buff[i] = pos[i];
	}
 //   memcpy(string, pos, &buffer[64] - pos + 1);
   // return string;
	 
}

void xtoa(uint32_t value,char  buff[9]){
// buffer size 
	#define X_BUFF_SIZE 9	
// 	
	
	uint32_t val;
 

    char buffer[X_BUFF_SIZE];
    char *pos;
    int digit;
	mem_set32(buff,'0',X_BUFF_SIZE);
 

    val = value;
	 
 
 
    pos = &buffer[X_BUFF_SIZE - 1];
    *pos = '\0';
                               
    do {
   	 	digit = val % 16;
    	val = val / 16;
		if (digit < 10) {
			*--pos = '0' + digit;
		
		} else {
			*--pos = 'a' + digit - 10;
		} /* if */
    } while (val != 0L);
  

 
	 int num  = pos - buffer;
	 for(int i=num;i< X_BUFF_SIZE; i++){
	 	buff[i] = buffer[i];
	 }

 
}
 
 
 