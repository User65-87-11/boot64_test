 
#include <stdint.h>

#include "keyboard.h"
 
 
#include "display.h"
#include "mem.h"
 
  


#define MAX_KB_BUFF_LEN 256
#define MAX_CMD_HISTORY 80*25*2



typedef struct{
	bool shift_down ;
	bool alt_down ;
	bool ctrl_down ;
	uint8_t ascii;
	uint8_t shift_keycode;
	uint8_t selected_set;

	uint16_t scancode;
	
}kb_state_t;





void cmd_kbb_append(char s[], char n); 

void cmd_kbb_backspace(char s[]);
void cmd_init();

void user_input(char *input) ;

void cmd_key_callback(kb_keycode_t scancode);

void print_letter(uint8_t scancode) ;

void cmd_print_cmd();