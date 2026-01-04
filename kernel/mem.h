#pragma once
#include <stdint.h>


extern  void mem_copy32(void *dst,void *src,int num);
extern  void mem_set32(void *dst,char byte,int num);
extern  void mem_rev_copy32(void *dst,void *byte,int num);
extern  bool mem_eq32(void * a,void *b,int len);

extern uint32_t mem_search32(uint8_t * mem, uint32_t mem_len,uint8_t * pattern, uint32_t pattern_len);
// extern int32_t mem_search_byte_32(uint8_t * mem, uint32_t mem_len, uint8_t  pattern);

extern uint32_t mem_search32_end(uint8_t * mem, uint8_t* mem_end,uint8_t * pattern, uint32_t pattern_len);


extern void * mem_salloc32(uint32_t size);


extern uint32_t strz_len32(char * zstr);
extern uint32_t str_utf16_len32(char * zstr);





// should be starting and ending position
// uint32_t search_mem(char * pattern, uint32_t p_len ,uint32_t start, uint32_t len);
// uint32_t search_array(char * str,uint32_t lensearch, char*bytes, uint32_t lenkey) ;

uint32_t kmalloc(uint32_t size, int align, uint32_t *phys_addr);

//stack allocate
uint32_t ksalloc(uint32_t size);


void itoa(int val,char  buff[12]);//returns filled buffer and the new length
void xtoa(uint32_t val,char buff[9]);