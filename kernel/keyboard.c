 
#include <stdint.h>

#include "keyboard.h"
#include "ports.h"
#include "isr.h"
#include "display.h"
#include "mem.h"
#include "timer.h"
// #include "loop.h"
  




// const char *sc_name[] = { 
// 	"ERROR", "Esc", "1", "2", "3", 
// 	"4", "5", "6", "7", "8", 
// 	"9", "0", "-", "=", "Backspace", 
// 	"Tab", "Q", "W", "E", "R", 
// 	"T", "Y", "U", "I", "O", 
// 	"P", "[", "]", "Enter", "Lctrl", 
//     "A", "S", "D", "F", "G", 
// 	"H", "J", "K", "L", ";", 
// 	"'", "`", "LShift", "\\", "Z", 
// 	"X", "C", "V", "B", "N", 
// 	"M", ",", ".", "/", "RShift", 
// 	"Keypad *", "LAlt", "Spacebar"
// };

#define KEY_SHIFT_LEFT 	0x100
#define KEY_SHIFT_RIGHT 0x101
#define KEY_CTRL_LEFT 	0x102
#define KEY_CTRL_RIGHT 	0x103
#define KEY_ALT_LEFT 	0x104
#define KEY_ALT_RIGHT 	0x105
#define KEY_KEYPAD_STAR 0x106
#define KEY_CAPS_LOCK	0x107
#define KEY_F1			0x108
#define KEY_F2			0x109
#define KEY_F3			0x10a
#define KEY_F4			0x10b
#define KEY_F5			0x10c
#define KEY_F6			0x10d
#define KEY_F7			0x10e
#define KEY_F8			0x10f
#define KEY_F9			0x110
#define KEY_F10			0x111
#define KEY_F11			0x112
#define KEY_F12			0x113
#define KEY_NUM_LOCK	0x114
#define KEY_SCROLL_LOCK	0x115

#define KEY_NUMPAD_ASCII_SLASH '/'

//keyboards are 104 keys = 208 states
uint8_t sc_ascii_0[] = { 
	'?', '?', '1', '2', '3', 
	'4','5', '6','7', '8', 
	'9', '0', '-', '=', '?', 
	'?', 'Q', 'W', 'E', 'R', 
	'T', 'Y','U', 'I', 'O', 
	'P', '[', ']', '?', '?', 
	'A', 'S', 'D', 'F', 'G',
	'H', 'J', 'K', 'L', ';', 
	'\'', '`', '?', '\\', 'Z', 
	'X', 'C', 'V', 'B', 'N', 
	'M', ',', '.', '/', '?', 
	'*', '?', ' ',
	//0x3a
	//caps and keypad
	'?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','7','8','9','-',
	'4','5','6','+','1',
	'2','3','0','.',
	'?','?','?','?','?','?',
	'?','?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','?','?',
	//0x81 released
	'?', '1', '2', '3', 
	'4','5', '6','7', '8', 
	'9', '0', '-', '=', '?', 
	'?', 'Q', 'W', 'E', 'R', 
	'T', 'Y','U', 'I', 'O', 
	'P', '[', ']', '?', '?', 
	'A', 'S', 'D', 'F', 'G',
	'H', 'J', 'K', 'L', ';', 
	'\'', '`', '?', '\\', 'Z', 
	'X', 'C', 'V', 'B', 'N', 
	'M', ',', '.', '/', '?', 
	'*', '?', ' ',
	//0xb9
	'?','?',
	'?','?','?','?','?',
	'?','?','?','?','?',
	'?','7','8','9','-',
	'4','5','6','+','1',
	'2','3','0','.',
	'?','?','?','?','?',
	//0xD8

	// '?','?','?','?','?','?','?',
	// //0xE0 pressed
	// '?',
};



// uint8_t sc_ex_ascii[] = { 
// 	'?','?','?','?','?','?','?','?','?','?','?','?','?','?','?','?',
// 	//0x10
// 	'?',
// 	'?','?','?','?','?','?','?','?',
// 	//0x19
// 	'?',
// };



// void kbb_append(char s[], char n) {
//     uint32_t len = strz_len32(s);
// 	if(len >= MAX_KB_BUFF_LEN) return;
// 	buff_size++;
//     s[len] = n;
//     s[len+1] = '\0';
// }

// void kbb_backspace(char s[]) {
//     int len = strz_len32(s);
// 	if(len == 0) return ;

//     s[len-1] = '\0';
// 	buff_size-=1;
// 	if(buff_size < 0)
// 		buff_size = 0;
// }




static kb_handler scancode_handler  = nullptr;
 
// static kb_keycode_t kb_keycode ={};
static kb_scancode_t sc ={};

static uint32_t skip = 0;




void kb_set_scancode_handler(kb_handler handler ){
	scancode_handler = handler;	
}
 

static void keyboard_callback(registers_t regs) {
    /* The PIC leaves us the scancode in port 0x60 */
   
 

	/*
		if [command queie]
			KEY_RESPONSE_ACK:
				next 
	*/
 
		//normal keys

	uint8_t response = port_8_in(0x60);

	
	// vga_print_hex(response);
	// print_cstring(" ");
	// vga_print_hex(skip);
	// print_cstring(" ");
	// vga_print_hex(sc.num);
	// print_cstring("\n");

	switch(response)
	{
		case KEY_RESPONSE_EX1:
			skip = 1;
			sc.s[sc.num++]= response;
			return;
		
		case KEY_RESPONSE_EX2:
			skip = 2;
			sc.s[sc.num++]= response;
			return;
		break;
		default:
			if(skip > 0)
			{
				skip --;
				sc.s[sc.num++]= response;
				return;
			}else
			{
				
				sc.s[sc.num++]= response;
			}
		break;
	}
	




	if(sc.s[0] == KEY_RESPONSE_EX1 || sc.s[0] == KEY_RESPONSE_EX2){
		//not done
		sc.num = 0;
		return;
	}


	kb_keycode_t kk={};
	kk.scancode = response;
	kk.key = sc_ascii_0[response];
	kk.pressed = response <= 0x80? true:false;

	if(kk.key == '?')
		kk.key = 0;


	scancode_handler(kk);

	//reset scancode queue
	sc.num = 0;


 
	

	
	(void)(regs);
}


void init_keyboard() {
	//  __asm__ volatile("cli");
	 
	//418b
	// skip = 0;
	// sc =(kb_scancode_t){0};

	register_interrupt_handler(IRQ1, keyboard_callback); 
    // __asm__ volatile("sti");
	


}
