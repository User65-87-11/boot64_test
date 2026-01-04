 
#include <stdint.h>

#include "ports.h"
#include "mem.h"

#include "display.h"

 

 

void scroll_by_v3( int y){
	char *vga_mem = (char*) VGA_MEM;

	char buffer[VGA_WIDTH*VGA_HEIGHT*2] = {};
	if( y <= 0 ) return;
	if( y > VGA_HEIGHT)
	{
		y = VGA_HEIGHT ;
	}
	mem_copy32(buffer,vga_mem,VGA_WIDTH*VGA_HEIGHT * 2);
	int distance = y * VGA_WIDTH;
	int cur_pos =	get_cur_pos();
	int new_cur_pos = cur_pos - distance ;
	if(new_cur_pos < 0) 
		new_cur_pos = 0;

	int num = (VGA_WIDTH*VGA_HEIGHT - distance ) * 2;
	
	

	clear_scr(0x0f);
	mem_copy32(vga_mem,&buffer[distance * 2],num);
	
	//mem_set32(VGA_MEM+num, 0, distance*2);
	set_cur_pos(new_cur_pos);
	//cursor is lost
 

}

unsigned short get_cur_pos(){

	port_8_out(0x3d4, 14); /* Requesting byte 14: high byte of cursor pos */
    /* Data is returned in VGA data register (0x3d5) */
    unsigned short position = port_8_in(0x3d5);
    position = position << 8; /* high byte */

    port_8_out(0x3d4, 15); /* requesting low byte */
    position += port_8_in(0x3d5);

	return position;
}
void print_memory(int addr, int len){
	char * a = (char *)addr;

	for(int i=0;i<len;i++){
		 char cl = a[i] & 0x0f;
		 char ch = (a[i] >> 4) & 0x0f;
		
		cl+=0x30;
		ch+=0x30;
		if(cl > '9')
			cl+= 7;
		if(ch > '9')
			ch+= 7;
		
		put_char(cl,i*2);
		put_char(ch,i*2 +1);
		
	}
	print_cstring("\n");
}

void put_char(char c, int pos ){
	char *vga_mem = (char*) VGA_MEM;
	vga_mem[pos*2] = c;
	vga_mem[pos*2 + 1] = 0x0e;
}
void set_cur_x(int x){
	int pos=(get_cur_pos() / VGA_WIDTH) * VGA_WIDTH  + x;
	set_cur_pos(pos);
}
void print_cstring_at(char *s,int x,int y){
	int pos = y*VGA_WIDTH + x;
	while(*s !='\0'){
		put_char(*s++,pos++);
	}
}
char get_char_at(int pos)
{
	char *vga_mem = (char*) VGA_MEM;
 
	return vga_mem[pos*2] ;
	 
}
void rem_char_before_cur(){
	unsigned short cur = get_cur_pos();
	
	if(cur <=0) return;
	
	//char c = get_char_at(cur);
	bool  skipped = false;
	while(true){
		if(cur <= 0) break;
		char c =get_char_at(--cur);
		
		if(c != 0){
			if(skipped) cur++;

			put_char(0,cur);
			// char buff[10];
			// mem_set32(buff,0, 10);
			// itoa32(c,buff,10);
			// print_cstring_at("    ", 15, 0);
			// print_cstring_at(buff, 15, 0);
			break;
		}else
		{
			skipped = true;


			// c = 1;
			// //put_char(0,cur);
			// char buff[10];
			// mem_set32(buff,0, 10);
			// itoa32(c,buff,10);
			// print_cstring_at("    ", 15, 0);
			// print_cstring_at(buff, 15, 0);
		}
		 
	}
	set_cur_pos(cur);
	
	
}

void rem_char_before_cur2(int stop_x,int num){
	unsigned short cur = get_cur_pos();
	int pos_x = CUR_X(cur);
	int pos_y = CUR_Y(cur);

	pos_x = pos_x - num;
	if(pos_x < stop_x )
		pos_x = stop_x;

	for(int i=0;i<num;i++){
		put_char(0,cur - i - 1);

	}
	set_cur_pos(pos_x + pos_y*VGA_WIDTH);
	
	// if(pos_x <= stop_pos + num) return;

	// while(pos_x > stop_pos && num > 0){
	// 	put_char(0,cur );
	// 	cur --;
	// 	pos_x --;
	// 	set_cur_pos(cur);
	// }
	//char c = get_char_at(cur);
	 
	
	
}
void append_char_at_cur(char c){
	unsigned short cur = get_cur_pos();

	
	if(c == '\n'){	
		cur = cur + (VGA_WIDTH - (cur % VGA_WIDTH))  - 1;
	}else
	{
		put_char(c,cur);
	}
	cur ++;
	set_cur_pos(cur);
	if(cur < (VGA_HEIGHT * VGA_WIDTH))
	{
		
	}else
	{
		scroll_by_v3(1);
	}
}
void print_cstring(char * s){
	
	
	while(*s!='\0')
	{
		append_char_at_cur(*s);
		s++;
	}
	
}
void print_cstring_len(char * s,int num){
	for(int i=0;i<num;i++)
	{
 
		append_char_at_cur(s[i]);
	 
	}
}
void print_cstring_end(char * s,char *e){
	while(s <= e){
		append_char_at_cur(*s++);
	}
}
void vga_clear_line_at(int x, int w, int y){
	char *vga_mem = (char*) VGA_MEM;
 
	y = (y * VGA_WIDTH ) ;
	if(w >= VGA_WIDTH)
		w = VGA_WIDTH;
	for(int i=x;i< w;i++ )
	{
			vga_mem[( y + i)<<1] = 0;
	}
}
void vga_clear_line(){
	char *vga_mem = (char*) VGA_MEM;
	int pos = get_cur_pos();
	int y = pos / VGA_WIDTH;
	y = (y * VGA_WIDTH ) ;
	for(int i=0;i< VGA_WIDTH ;i++ )
	{
			vga_mem[( y + i)<<1] = 0;
	}
}
void clear_scr( uint8_t col){
 	char *vga_mem = (char*) VGA_MEM;


	for(int i=0;i< VGA_HEIGHT*VGA_WIDTH  ;i++){
		vga_mem[i<<1] = 0;
		vga_mem[(i<<1)+1] =col;
		
	}
}

void vga_print_int(int i){
	
		char  buff[12] = {};
		itoa(i,buff);
		print_cstring(buff);
}
void vga_print_hex(uint32_t hex){

		char  buff[9] = {};
		xtoa(hex,buff);
		print_cstring(buff);
}
void vga_print_int_at(int i, int x,int y){
	
		char  buff[12] = {};
		itoa(i,buff);
		print_cstring_at(buff,x,y);
}
void vga_print_hex_at(uint32_t hex,int x,int y){

		char  buff[9] = {};
		xtoa(hex,buff);
		print_cstring_at(buff,x,y);
}

void set_cur_pos(int pos){
	//uint16_t pos = y * VGA_WIDTH + x;

	port_8_out(0x3D4, 0x0F);
	port_8_out(0x3D5, (unsigned char) (pos & 0xFF));
	port_8_out(0x3D4, 0x0E);
	port_8_out(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
void set_cur_xy(int x,int y){
	unsigned short pos = y * VGA_WIDTH + x;

	port_8_out(0x3D4, 0x0F); // low
	port_8_out(0x3D5, (unsigned char) (pos & 0xFF));
	port_8_out(0x3D4, 0x0E); //high
	port_8_out(0x3D5, (unsigned char) ((pos >> 8) & 0xFF));
}
 