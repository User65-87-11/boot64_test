#pragma once
#include <stdint.h>



/**
0xE0
	cnt++
	0x10
	cnt --

;; skip 1
0xE0, 0x10	(multimedia) previous track pressed						
0xE0, 0x19	(multimedia) next track pressed				
0xE0, 0x1C	(keypad) enter pressed	
0xE0, 0x1D	right control pressed				
0xE0, 0x20	(multimedia) mute pressed	
0xE0, 0x21	(multimedia) calculator pressed	
0xE0, 0x22	(multimedia) play pressed		
0xE0, 0x24	(multimedia) stop pressed						
0xE0, 0x2E	(multimedia) volume down pressed		
0xE0, 0x30	(multimedia) volume up pressed			
0xE0, 0x32	(multimedia) WWW home pressed		
0xE0, 0x35	(keypad) / pressed				
0xE0, 0x38	right alt (or altGr) pressed						
0xE0, 0x47	home pressed
0xE0, 0x48	cursor up pressed	
0xE0, 0x49	page up pressed			
0xE0, 0x4B	cursor left pressed
0xE0, 0x4D	cursor right pressed			
0xE0, 0x4F	end pressed
0xE0, 0x50	cursor down pressed	
0xE0, 0x51	page down pressed	
0xE0, 0x52	insert pressed	
0xE0, 0x53	delete pressed
0xE0, 0x5B	left GUI pressed
0xE0, 0x5C	right GUI pressed	
0xE0, 0x5D	"apps" pressed	
0xE0, 0x5E	(ACPI) power pressed	
0xE0, 0x5F	(ACPI) sleep pressed
0xE0, 0x63	(ACPI) wake pressed
0xE0, 0x65	(multimedia) WWW search pressed	
0xE0, 0x66	(multimedia) WWW favorites pressed	
0xE0, 0x67	(multimedia) WWW refresh pressed
0xE0, 0x68	(multimedia) WWW stop pressed	
0xE0, 0x69	(multimedia) WWW forward pressed	
0xE0, 0x6A	(multimedia) WWW back pressed	
0xE0, 0x6B	(multimedia) my computer pressed
0xE0, 0x6C	(multimedia) email pressed	
0xE0, 0x6D	(multimedia) media select pressed				
0xE0, 0x90	(multimedia) previous track released						
0xE0, 0x99	(multimedia) next track released				
0xE0, 0x9C	(keypad) enter released	
0xE0, 0x9D	right control released				
0xE0, 0xA0	(multimedia) mute released	
0xE0, 0xA1	(multimedia) calculator released	
0xE0, 0xA2	(multimedia) play released		
0xE0, 0xA4	(multimedia) stop released						
0xE0, 0xAE	(multimedia) volume down released		
0xE0, 0xB0	(multimedia) volume up released			
0xE0, 0xB2	(multimedia) WWW home released		
0xE0, 0xB5	(keypad) / released				
0xE0, 0xB8	right alt (or altGr) released						
0xE0, 0xC7	home released
0xE0, 0xC8	cursor up released	
0xE0, 0xC9	page up released			
0xE0, 0xCB	cursor left released
0xE0, 0xCD	cursor right released			
0xE0, 0xCF	end released
0xE0, 0xD0	cursor down released	
0xE0, 0xD1	page down released	
0xE0, 0xD2	insert released	
0xE0, 0xD3	delete released
0xE0, 0xDB	left GUI released
0xE0, 0xDC	right GUI released	
0xE0, 0xDD	"apps" released	
0xE0, 0xDE	(ACPI) power released	
0xE0, 0xDF	(ACPI) sleep released
0xE0, 0xE3	(ACPI) wake released
0xE0, 0xE5	(multimedia) WWW search released	
0xE0, 0xE6	(multimedia) WWW favorites released	
0xE0, 0xE7	(multimedia) WWW refresh released
0xE0, 0xE8	(multimedia) WWW stop released	
0xE0, 0xE9	(multimedia) WWW forward released	
0xE0, 0xEA	(multimedia) WWW back released	
0xE0, 0xEB	(multimedia) my computer released
0xE0, 0xEC	(multimedia) email released	
0xE0, 0xED	(multimedia) media select released	

skip  2x1
0xE0, 0x2A, 0xE0, 0x37	print screen pressed
0xE0, 0xB7, 0xE0, 0xAA	print screen released

skip 2x2
0xE1, 0x1D, 0x45, 0xE1, 0x9D, 0xC5	pause pressed				

skip = 0
uint8_t ex[6];
	l = 0;
switch(response)
	case 0xE0:
		skip =1
		ex[len++]= response
		return;
	break;
	case 0xE1:
		skip = 2
		ex[len++]= response
		return;
	break;
	default:
		if(skip > 0)
			skip --
			ex[len++]= response
			return
		ex[len++]= response
	break;


handle
	ex[len]
	reset ex[len]

*/



#define KEY_RESPONSE_ACK 0xFA
#define KEY_RESPONSE_RET 0xFE
#define KEY_RESPONSE_EX1 0xE0
#define KEY_RESPONSE_EX2 0xE1
#define KEY_RESPONSE_ERROR1 0x00
#define KEY_RESPONSE_ERROR2 0xFF
#define KEY_RESPONSE_SELF_TEST_PASSED 0xAA
#define KEY_RESPONSE_SELF_TEST_FAILED1 0xFC
#define KEY_RESPONSE_SELF_TEST_FAILED2 0xFD

#define SC_MAX 57
#define BACKSPACE 0x0E
#define ENTER 0x1C

 
typedef struct{
	uint8_t s[6];
	uint8_t num;
} kb_scancode_t;




typedef struct {
	uint8_t scancode;
	uint8_t extended;
	uint8_t key;
	uint8_t pressed;
}kb_keycode_t;




 

typedef  void ( * kb_handler )(kb_keycode_t response);



void init_keyboard();

void kb_set_scancode_handler(kb_handler handler );
 

void kb_set_set(uint8_t id,void (*call_back)(uint8_t data));

 
 

// void kbb_append(char s[], char n) ;
// void kbb_backspace(char s[]);
// void user_input(char *input);

 