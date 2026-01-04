 
#include <stdint.h>

#include "keyboard.h"
#include "cmd.h"
#include "memory_map.h"
#include "display.h"
#include "mem.h"
#include "pcie.h"
  
#include "rsdt.h"
  


 
static char key_buffer[MAX_KB_BUFF_LEN]={};
 
static char cmd_tag[]="[cmd]:";
 
static char history [MAX_CMD_HISTORY] = {};
static uint32_t key_buff_frame_start = 0;
static uint32_t key_buff_frame_end = 0;
 
static uint32_t cmd_tag_len = 0;

static kb_state_t key_state;



 

void cmd_kbb_append(char s[], char n) {
    uint32_t len = strz_len32(s);
	if(len >= MAX_KB_BUFF_LEN) return;
 
    s[len] = n;
    s[len+1] = '\0';

}

void cmd_kbb_backspace(char s[]) {
    int len = strz_len32(s);
	if(len == 0) return ;
	 
    s[len-1] = '\0';
	
 
}
void cmd_print_cmd(){
	
	set_cur_x(0);
	vga_clear_line();
	print_cstring(cmd_tag);
	 
	
	int len = strz_len32(key_buffer);

 
	if(len > 60)
	{
		key_buff_frame_end = len -1;
		key_buff_frame_start = key_buff_frame_end - 60;
	}
	else
	{
		key_buff_frame_end = len - 1;
		key_buff_frame_start = 0;
	}
	print_cstring_end(&key_buffer[key_buff_frame_start],&key_buffer[key_buff_frame_end]);
	
}
void cmd_init(){
	/*
		1.manages what is on the screen
		2.can scroll
		3.no going passsed possition
		4.starts with [cmd]:
		5.scrolls horizontally with left and right
		6.ctrl+left positions on a space ater a word or before a word
		7.up-right scrolls vertically
		8.starts at where it left or curssor(0)
	*/

	cmd_tag_len = strz_len32(cmd_tag);


	
}
 
void user_input(char *input) {



	if (mem_eq32(input, "END",3)) {
        print_cstring("Stopping the CPU. Bye!\n");


        __asm__ volatile("hlt");

	}

	if(mem_eq32(input, "REBOOT",6))
	{
		uint16_t a=0;
		 __asm__ volatile(
			"lidt %0\n"
			"int $3"
			:
    		: "m"(a));
	}
	if(mem_eq32(input,"TIMER",5))
	{
		char page_str[12] = "NOT IMPL...";
 
     
		print_cstring("Timer: ");
		print_cstring(page_str);
		print_cstring("\n");
		goto end;

	}
	if(mem_eq32(input,"MEM MAP",7))
	{
		memory_map_print();
		goto end;

	}

	if(mem_eq32(input,"MS",2))
	{
		print_cstring("Search for: \n");
		int len = strz_len32(input+2);
		if(len > 0)
		{
			char * end = input + 2 +1;
			char * start = end;
			while(*end != 0)
			{
				end++;	

			}
			/**
			
			limited to emulator's memory


			"MCFG" where?

			"RSD PTR " non-null terminated 8 bytes ACPI first

			*/
			
			uint32_t pat_len = end - start ;
			print_cstring(start);
			print_cstring("\n pattern len: ");
			vga_print_int(pat_len);
			
			uint32_t res = 	mem_search32_end((void *) 0x00000, (void *) 0xa0000000, start, pat_len);
			if((char * )res == start)
			{
				res = 	mem_search32_end((void *) start +pat_len, (void *) 0xa0000000, start, pat_len);
			}
			// res = 	mem_search32((void *) 20, 0x0fffffff, start, 4);
			
		 	print_cstring("\n addr:");
			vga_print_hex(res);
			// print_cstring_end(start, end);
			print_cstring("\n");
			print_cstring((char*)res);
			 
			print_cstring("\n");
		}
		// memory_map_print();
		goto end;

	}
	
	if(mem_eq32(input,"PCIE",4))
	{
		uint16_t vendor = pciConfigReadWord(0, 0, 0, 0);
		uint16_t device = 0;
		if (vendor != 0xFFFF) {
       		device = pciConfigReadWord(0, 0, 0, 0);
		}
		print_cstring("PCIe\n");
		if(device != 0)
		{
			
			print_cstring("device:");
			vga_print_hex(device);
			print_cstring(" vendor:");
			vga_print_hex(vendor);
			
		}else
		{
			print_cstring("NO device:");
		}
		print_cstring("\n");
		goto end;

	}
	if(mem_eq32(input,"RSDT",4))
	{
			char pat[] = "RSD PTR \n";
			
			uint32_t len = 8;
			uint32_t res = 	mem_search32_end((void *) 0x00080000, (void *) 	0x000FFFFF, pat, len);
			if((char * )res == pat)
			{
				//res = 	mem_search32_end((void *) pat +len, (void *) 0xa00000, pat, len);
			}

			print_cstring("Address: 0x");	
			vga_print_hex(res);
			print_cstring("\n");
		 

			if(res != 0xffffffff)
			{
				// rsdt_print((void *)0xf5280);
				
				RSDP * rsdp = (void*)res;
				ACPISDTHeader * h = (ACPISDTHeader*)rsdp->RsdtAddress;


				rsdt_print((void*)res);

 
				//below 4 GB
				FADT * facp = rsdt_findFACP(h);
				
				print_cstring("FADT:");	
				vga_print_hex((uint32_t)facp);
				print_cstring("\n");	

				rsdt_print_FACP_address_spaces(facp);


				// rsdt_print_acpi_tabled((void*)facp->X_PM2ControlBlock.Address);
			}
			
	}

	/**
	rsdt_print
	
	*/

	if(mem_eq32(input,"HELP",4)){

		print_cstring("----CMD HELP ---\n");
		print_cstring("END: hlt\n");
		print_cstring("REBOOT: triple fault\n");
		print_cstring("MEM MAP: shows memory map\n");
		print_cstring("PCIE: show info about PCIE\n");
		print_cstring("MS PATTERN: Searches range 0x9_0000 - 0xa0_0000 (10meg)\n");
		print_cstring("RSDT: Searches range 0x9_0000 - 0xa0_0000 (10meg)\n");

		goto end;
	}

	/**
	
	Flow

Interrupt happens → ISR runs → queues event → iret

CPU resumes where it was interrupted

If it was in hlt, execution continues after it

Kernel loop sees queued events and runs handlers
	*/


    // if (mem_eq32(input, "END",3)) {
    //     print_cstring("Stopping the CPU. Bye!\n");


    //     __asm__ volatile("hlt");
	// }else if(mem_eq32(input, "REBOOT",3)){
	// 	uint16_t a=0;
	// 	 __asm__ volatile(
	// 		"lidt %0\n"
	// 		"int $3"
	// 		:
    // 		: "m"(a)
	// 	);
	// 			// lidt [bad_idt]

    // }else if (mem_eq32(input,"PAGE",4)){
	// 	uint32_t phys_addr;
    //     uint32_t page = kmalloc(1000, 1, &phys_addr);
    //     char page_str[16] = "";
    //     itoa(page, page_str,16);
    //     char phys_str[16] = "";
    //     itoa(phys_addr, phys_str,16);
    //     print_cstring("Page: ");
    //     print_cstring(page_str);
    //     print_cstring(", physical address: ");
    //     print_cstring(phys_str);
    //     print_cstring("\n");

	// }else if(mem_eq32(input,"TIMER",5))
	// {
	// 	char page_str[12] = "";
 
     
	// 	print_cstring("Timer: ");
	// 	print_cstring(page_str);
	// 	print_cstring("\n");

	// }
    // print_cstring("You said: ");
    // print_cstring(input);
    // print_cstring("\n> ");

	end:

}
 

void cmd_key_callback(kb_keycode_t sc){
	int scancode = sc.scancode;

 
	if (scancode > SC_MAX) return ;
    if (scancode == BACKSPACE) {
        cmd_kbb_backspace(key_buffer);
        // rem_char_before_cur2(2,1);
		
    } else if (scancode == ENTER) {
		// append_char_at_cur('\n');
        //print_cstring("\n");
		set_cur_x(0);
		vga_clear_line();
		print_cstring(cmd_tag);
		print_cstring(key_buffer);
		print_cstring("\n");
	 
        user_input(key_buffer); /* kernel-controlled function */
        key_buffer[0] = '\0';
		// print_cstring(">");
    }	 else	{

	 

		
		char letter = sc.key;
		/* Remember that kprint only accepts char[] */
		// char str[2] = {letter, '\0'};

		 
		
		cmd_kbb_append(key_buffer, letter);
	
        	// print_cstring(str);
	  
		//set_cur_x(1);
	//	print_cstring(&key_buffer[0]);
	
		
    }
	cmd_print_cmd();
}


void print_letter(uint8_t scancode) {
    switch (scancode) {
        case 0x0:
            print_cstring("ERROR");
            break;
        case 0x1:
            print_cstring("ESC");
            break;
        case 0x2:
            print_cstring("1");
            break;
        case 0x3:
            print_cstring("2");
            break;
        case 0x4:
            print_cstring("3");
            break;
        case 0x5:
            print_cstring("4");
            break;
        case 0x6:
            print_cstring("5");
            break;
        case 0x7:
            print_cstring("6");
            break;
        case 0x8:
            print_cstring("7");
            break;
        case 0x9:
            print_cstring("8");
            break;
        case 0x0A:
            print_cstring("9");
            break;
        case 0x0B:
            print_cstring("0");
            break;
        case 0x0C:
            print_cstring("-");
            break;
        case 0x0D:
            print_cstring("+");
            break;
        case 0x0E:
            print_cstring("Backspace");
            break;
        case 0x0F:
            print_cstring("Tab");
            break;
        case 0x10:
            print_cstring("Q");
            break;
        case 0x11:
            print_cstring("W");
            break;
        case 0x12:
            print_cstring("E");
            break;
        case 0x13:
            print_cstring("R");
            break;
        case 0x14:
            print_cstring("T");
            break;
        case 0x15:
            print_cstring("Y");
            break;
        case 0x16:
            print_cstring("U");
            break;
        case 0x17:
            print_cstring("I");
            break;
        case 0x18:
            print_cstring("O");
            break;
        case 0x19:
            print_cstring("P");
            break;
		case 0x1A:
			print_cstring("[");
			break;
		case 0x1B:
			print_cstring("]");
			break;
		case 0x1C:
			print_cstring("ENTER");
			break;
		case 0x1D:
			print_cstring("LCtrl");
			break;
		case 0x1E:
			print_cstring("A");
			break;
		case 0x1F:
			print_cstring("S");
			break;
        case 0x20:
            print_cstring("D");
            break;
        case 0x21:
            print_cstring("F");
            break;
        case 0x22:
            print_cstring("G");
            break;
        case 0x23:
            print_cstring("H");
            break;
        case 0x24:
            print_cstring("J");
            break;
        case 0x25:
            print_cstring("K");
            break;
        case 0x26:
            print_cstring("L");
            break;
        case 0x27:
            print_cstring(";");
            break;
        case 0x28:
            print_cstring("'");
            break;
        case 0x29:
            print_cstring("`");
            break;
		case 0x2A:
			print_cstring("LShift");
			break;
		case 0x2B:
			print_cstring("\\");
			break;
		case 0x2C:
			print_cstring("Z");
			break;
		case 0x2D:
			print_cstring("X");
			break;
		case 0x2E:
			print_cstring("C");
			break;
		case 0x2F:
			print_cstring("V");
			break;
        case 0x30:
            print_cstring("B");
            break;
        case 0x31:
            print_cstring("N");
            break;
        case 0x32:
            print_cstring("M");
            break;
        case 0x33:
            print_cstring(",");
            break;
        case 0x34:
            print_cstring(".");
            break;
        case 0x35:
            print_cstring("/");
            break;
        case 0x36:
            print_cstring("Rshift");
            break;
        case 0x37:
            print_cstring("Keypad *");
            break;
        case 0x38:
            print_cstring("LAlt");
            break;
        case 0x39:
            print_cstring("Spc");
            break;
        default:
            /* 'keuyp' event corresponds to the 'keydown' + 0x80 
             * it may still be a scancode we haven't implemented yet, or
             * maybe a control/escape sequence */
            if (scancode <= 0x7f) {
                print_cstring("Unknown key down");
            } else if (scancode <= 0x39 + 0x80) {
                print_cstring("key up ");
                print_letter(scancode - 0x80);
            } else print_cstring("Unknown key up");
            break;
    }
	
}
 