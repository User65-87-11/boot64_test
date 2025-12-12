FREE_SPACE equ 0x9000

SECOND_STAGE_DATA_SEG equ 0x1000
SECOND_STAGE_OFFSET equ 0x0000  ; = [ds:1000]
; by default operates with first 64 kib range
;around 0x1000 to 0x7c00 for the second stage
;16 KB 
[ORG 0x7C00]
[BITS 16]

; Main entry point where BIOS leaves us.

Main:
		
    	jmp 0x0000:.FlushCS               ; Some BIOS' may load us at 0x0000:0x7C00 while other may load us at 0x07C0:0x0000.
                                      ; Do a far jump to fix this issue, and reload CS to 0x0000.

	.FlushCS:   
		xor ax, ax
		mov [BOOT_DRIVE], dl ;
		; Set up segment registers.
		mov ss, ax
		; Set up stack so that it starts below Main.
		mov sp, Main
		
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		cld

		


		call CheckCPU                     ; Check whether we support Long Mode or not.
		jc .NoLongMode

		call load_kernel

		; Point edi to a free space bracket.
		mov edi, FREE_SPACE
		; Switch to Long Mode.
		;jmp SwitchToLongMode
		mov ax,SECOND_STAGE_DATA_SEG
		mov ds,ax
		call SECOND_STAGE_OFFSET + 4 ;header


	[BITS 16]

	.NoLongMode:
		mov bx, NoLongMode
		call print

	.Die:
		hlt
		jmp .Die

%include "boot64/boot_print_hex.asm"
%include "boot64/boot_print.asm"
%include "boot64/boot_disk_load.asm"
;%include "boot64/long_switch.asm"

[BITS 16]

load_kernel:
	;mov ds,1
    ;mov bx, SECOND_STAGE_OFFSET ; Read from disk and store at 0x1000
	;mov dx, num sectors
    mov dh, 20 ; 54 ; 6 bits ; 8 bits for heads; 10 bits fo cyls
    mov dl, [BOOT_DRIVE]
	
	call disk_geom
    call disk_load
	
	call print_nl
	mov WORD [HEX_IN], 0x1234
	call print_hex
	call print_nl
	push SECOND_STAGE_DATA_SEG
	pop ds
	mov dx,ds:[SECOND_STAGE_OFFSET]
	push 0
	pop ds
	mov [HEX_IN],dx
	call print_hex
	jmp $
    ret

[BITS 16]


NoLongMode db "N", 0x0A, 0x0D, 0


; Checks whether CPU supports long mode or not.

; Returns with carry set if CPU doesn't support long mode.

CheckCPU:
    ; Check whether CPUID is supported or not.
		pushfd                            ; Get flags in EAX register.
		
		pop eax
		mov ecx, eax  
		xor eax, 0x200000 
		push eax 
		popfd

		pushfd 
		pop eax
		xor eax, ecx
		shr eax, 21 
		and eax, 1                        ; Check whether bit 21 is set or not. If EAX now contains 0, CPUID isn't supported.
		push ecx
		popfd 

		test eax, eax
		jz .NoLongMode
		
		mov eax, 0x80000000   
		cpuid                 
		
		cmp eax, 0x80000001               ; Check whether extended function 0x80000001 is available are not.
		jb .NoLongMode                    ; If not, long mode not supported.

		mov eax, 0x80000001  
		cpuid                 
		test edx, 1 << 29                 ; Test if the LM-bit, is set or not.
		jz .NoLongMode                    ; If not Long mode not supported.

		ret

	.NoLongMode:
		stc
		ret


; Prints out a message using the BIOS.

; es:si    Address of ASCIIZ string to print.

; Print:
; 		pushad
; 	.PrintLoop:
; 		lodsb                             ; Load the value at [@es:@si] in @al.
; 		test al, al                       ; If AL is the terminator character, stop printing.
; 		je .PrintDone                  	
; 		mov ah, 0x0E	
; 		int 0x10
; 		jmp .PrintLoop                    ; Loop till the null character not found.
		
; 	.PrintDone:
; 		popad                             ; Pop all general purpose registers to save them.
; 		ret

BOOT_DRIVE db 0x00 ; 

; Pad out file.
times 510 - ($-$$) db 0
dw 0xAA55