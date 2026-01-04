#pragma once
#include <stdint.h>


#define VGA_MEM 0xb8000
#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#define CUR_X(POS) (POS % VGA_WIDTH)
#define CUR_Y(POS) (POS / VGA_WIDTH)

 
unsigned short get_cur_pos();

void print_memory(int addr, int len);
void put_char(char c, int pos );
void print_cstring_at(char *s,int x,int y);
void print_cstring(char * s);
void print_cstring_len(char * s,int num);
void print_cstring_end(char * s,char *e);
void clear_scr( uint8_t col);
void set_cur_pos(int pos);
void set_cur_xy(int x,int y);
void set_cur_x(int x);
void set_cur_y(int y);
void scroll_by_v3( int y);
void vga_clear_line();
void vga_clear_line_at(int x,int w, int y);
void vga_print_int_at(int i,int x,int y);
void vga_print_hex_at(uint32_t hex,int x,int y);
void vga_print_int(int i);
void vga_print_hex(uint32_t hex);


char get_char_at(int pos);
void rem_char_before_cur();
void rem_char_before_cur2(int stop_x,int num);
void rem_char_before_cur3(int num);
void append_char_at_cur(char c);

