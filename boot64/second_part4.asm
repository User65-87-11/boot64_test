


%include "boot64/const.inc"

[org KERNEL_LOADER_SECOND]


%macro zstr_nl 1
	db %1, 0x0A, 0x0D,0
%endmacro 

%macro zstr 1
	db %1, 0
%endmacro 




;;;;
; this is run in segment 0
;;;




 

;1MB 2048 sectors
;64KB 128 sectors
; starting 128 sector : 0x80
; left = 1968
; read by 48 secotrs : 
; loops = 16-1

[bits 16]
second_start:
	mov [BOOT_DRIVE],dl
	mov si,STR_SECOND_STAGE
	call print_zstr
	mov ax,0xabcd
	call print_hex

	;;skip for now
	; jmp .enter_protected_mode
	
.read_kernel_loader

	; mov cx,7 ; * 64kb

	; ; after current 64kb area
	; mov bx, 0		;offset
	; shift ds by 0x8*512

	
	mov word [DAP.off],KERNEL_LOADER
	mov word [DAP.seg],0x0
	mov word [DAP.ladd],0x800+KERNEL_LOADER_SECOND_SEC_NUM
	mov word [DAP.num],KERNEL_LOADER_SEC_NUM
 
	call read_lba

	; mov word [DAP.off],KERNEL_LOADER_SECOND
	; mov word [DAP.seg],0x0000
	; mov word [DAP.ladd],0x800
	; mov word [DAP.num],KERNEL_LOADER_SECOND_SEC_NUM
		

 
 
; .loop:
	
	;increase segment offset
	; mov ax,[DAP.seg]
	; add ax,0x1000
	; mov [DAP.seg],ax
	; add WORD [DAP.seg] , KERNEL_LOADER

	;set where the nnext 64kb block is addressed
	; mov ax,[DAP.ladd]
	; add ax,0x80
	; mov [DAP.ladd],ax

	; add WORD [DAP.ladd] , 0x80

	; call read_lba
	; loop .loop

; .check_magic_workd:

; 	push ds
; 	mov ds,[DAP.seg]
; 	mov si,65536-4
; 	call print_zstr
; 	pop ds
	
	
.enter_protected_mode:
 
	;;;;;;; UPDATE OFFSETS IN GDT Since we are [DS=CS=?];;;;;;;;
	; mov ax,ds
	; shr ax,4
	; mov [TEMP_GDT_PTR_32.add + 2],ah
	; mov [TEMP_GDT_CODE+4],ah
	;;;;;;;;;


	;0x00_CF_9A_00_0000_FFFF
	;0x0000_FFFF
	;0x00CF_9A00

	call copy_GDT
 

	

	call read_memory_map
	
	; mov cx,1
	; mov si, MEMORY_MAP_BUFFER
	; call print_hex_var
	; call print_nl


	 
	cli  
	
	
	; lgdt [TEMP_GDT_PTR_32]    
	lgdt [GLOBALS]    
	 

	mov eax, cr0 
	or al, 1       
	mov cr0, eax
 

 
	jmp CODE_SEG: First_Jump




copy_GDT:
	mov cx,GDT_SIZE
	mov si, TEMP_GDT
	mov di, GDT_LOCATION
	repnz movsb
	mov WORD [GLOBALS],GDT_SIZE
	mov WORD [GLOBALS+2], GDT_LOCATION
	mov WORD [GLOBALS+4],0
	mov WORD [GLOBALS+6],0
	mov WORD [GLOBALS+8],0
	ret


[bits 32]


First_Jump:


	
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov esp, SECOND_STACK
	

	; jmp CODE_SEG_FLAT:ProtectedModeStart+0x10000
	; jmp $

	; should be 32 () , linked with section.txt = 0x10000 to fix all the offsets
	; can 16 bit jump to "no so far 32 bit address"
	; should be possible , since addresses are just numbers

; ProtectedModeStart:

	

	; mov BYTE [TEMP_GDT_CODE +0x10000 + 4],0 ; rebase to 0

 
	mov edi, 0xB8000
	mov ecx, 80*24                      ; Since we are clearing uint64_t over here, we put the count as Count/4.
	mov eax, 0x331;0x1F201F20       ; Set the value to set the screen to: Blue background, white foreground, blank spaces.
	rep stosw 

; 00000095  F366AB            rep stosd
; 00000098  E96301            jmp 0x1fe


	


	;jmp to_elf_bin after 512
	jmp CODE_SEG:KERNEL_LOADER_ELF_OFFSET 

	



;0xabcd
[bits 16]
hex_obj:
	.str	db "0x"
	.body	db "0000"
	.end	db 0x0A, 0x0D,0
	.ascii db "0123456789ABCDEF"
print_hex:
 
	pusha
	mov di,hex_obj.body + 3
	mov si,hex_obj.ascii
	mov cx,4
.loop:
	mov bx,ax
	and bx,0x0f
	mov bl,[si+bx]
	mov [di],bl
	dec di
	ror ax,4
	loop .loop
	mov si,hex_obj
	call print_zstr
	popa
	ret
;ds:si bytes
print_zstr:
	pusha
	mov ah, 0x0e
.loop:
	lodsb
	cmp al,0
	jz .finish
	int 0x10
	jmp .loop
.finish:
	popa
	ret

;;;;
hex_obj_var:
	.str	db "0x"
	.body	times 16 db 0
	.end	db 0
	
	.ascii db "0123456789ABCDEF"

;;cx number of bytes [1 to 8]
;;si source addr max 8 bytes
print_hex_var:
	
	pusha
	cmp cx,0
	je .error	
	push cx ; temporary swap
	mov di,hex_obj_var.body 
	mov cx, 16
	mov al,0
	rep stosb

	pop cx; back
	
 
	mov di,hex_obj_var.body 
	add si,cx
	dec si
	
	
.loop:
	mov bx,[si]
	and bx,0xf

	;use it as an offset to ascii array
	mov al,[hex_obj_var.ascii + bx]
	;get an octet
	mov bx, [si]
	and bx,0xf0
	shr bx,4
	;use it as an offset to ascii array
	mov ah,[hex_obj_var.ascii + bx]

	mov [di],ah
	mov [di+1],al
	add di,2
	dec si
	loop .loop
	

	mov si,hex_obj_var
	call print_zstr
	popa
	ret

.error:
	mov si, ERR_ZERO_LEN
	call print_zstr
	jmp idle_loop
 

print_space:
	pusha
    mov ah, 0x0e
    mov al, 0x20 ; space
    int 0x10
    popa
    ret

print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret

idle_loop:
    hlt         ; halt CPU until the next interrupt
    jmp idle_loop

	; mov bx,[si]
	; and bx,0xf

	; ;use it as an offset to ascii array
	; mov al,[hex_obj_var.ascii + bx]
	; ;get an octet
	; mov bx, [si]
	; and bx,0xf0
	; shr bx,4
	; ;use it as an offset to ascii array
	; mov ah,[hex_obj_var.ascii + bx]

	; mov [di],ah
	; mov [di+1],al
	; add di,2
	; dec si
	;;; alternative hexe loop
; 	mov al,[si]
; 	mov ah,al
; 	shr ah,4
; 	and ax,0x0f0f
; 	add ax,0x3030

;     cmp al, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
; 	jle .skip_hex_al

; 	add al,7

; .skip_hex_al:
	
;     cmp ah, 0x39 ; if > 9, add extra 8 to represent 'A' to 'F'
; 	jle .skip_hex_ah	
	
; 	add ah,7

; .skip_hex_ah:
; 	mov [di],ah
; 	mov [di+1],al
; 	add di,2
; 	dec si

[bits 16]

;cx num bytes
;di destination
zero_buffer:
	pusha
	mov al, 0x0;
	rep stosb
	popa
	ret

read_lba:
	pusha
	cli
	mov ah,0x42
	mov dl,[BOOT_DRIVE]
	mov si,DAP
	int 0x13
	jc disk_error
	popa
	ret


disk_error:
	mov si, STR_ERROR_DISK
	call print_zstr
	call print_hex ; check out the code at http://stanislavs.org/helppc/int_13-1.html
	call print_nl
	jmp idle_loop



;----------------
MEM_ENTRY_CNT db 0
read_memory_map:
	; push DWORD 0
	; mov word [MEMORY_MAP_BUFFER_NUM_ENTRIES],0
	; mov si,0
	mov di,MEMORY_MAP_BUFFER1
	mov cx, MEMORY_MAP_BUFFER_ENTRY_SIZE * MEMORY_MAP_BUFFER_MAX_ENTRIES
	call zero_buffer
	xor ebx, ebx
.loop:
	mov ax,0x1
	mov [di+20],ax
	mov edx,MEMORY_MAP_MAGIC
	mov eax,0xE820 
	mov ecx,MEMORY_MAP_BUFFER_ENTRY_SIZE
	INT 0x15
	cmp eax,MEMORY_MAP_MAGIC
	jne .magic_error
	jc .end
	cmp ebx,0
	jz .end

	; pop eax
	inc byte [MEM_ENTRY_CNT]
	; add word [MEMORY_MAP_BUFFER_NUM_ENTRIES], 1
	; mov ax,[MEMORY_MAP_BUFFER_SIZE_OFFSET]
	; inc ax
	; mov [MEMORY_MAP_BUFFER_SIZE_OFFSET],ax
	cmp byte [MEM_ENTRY_CNT],MEMORY_MAP_BUFFER_MAX_ENTRIES
	 
	je .end
	add di,MEMORY_MAP_BUFFER_ENTRY_SIZE
	jmp .loop

.magic_error:
	mov si,MEMORY_MAGIC_NUMBER_ERROR
	call print_zstr
	jmp idle_loop


.end:
	mov si,MEMORY_END_REACHED
	call print_zstr
	mov si,MEMORY_ENTRIES_FOUND
	call print_zstr
	
	mov ax,[MEM_ENTRY_CNT]
	call print_hex
	call print_nl

	mov cx, MEMORY_MAP_BUFFER_MAX_ENTRIES
	mov si, MEMORY_MAP_BUFFER1

.loop_all:
	push cx
	mov cx,8
	call print_hex_var
	call print_space
	add si,8
	call print_hex_var
	call print_space
	add si,8
	mov cx,4
	call print_hex_var
	call print_space
	add si,4
	call print_hex_var
	call print_nl
	add si,4
	pop cx
	
	loop .loop_all
	 
	ret



MEMORY_MAGIC_NUMBER_ERROR zstr_nl "Magic Error"
MEMORY_END_REACHED zstr_nl "Memory end reached"
MEMORY_ENTRIES_FOUND zstr "Entries found: "

;accessed bit default (1), in RO page when (0) cause Page fault
SEG_ACCESS_A equ 1 

;for Code Segments[CS]: (1) is allowed to read. Write is never allowed for Code Segments
;for Data Segments[DS]: (1) is write is allowed. Read is always allowed for Data Segments
SEG_ACCESS_RW equ 1 << 1

;Direction / Conforming. 
;For [DS] when (0) grows up,(1) down. [Offset] and [limit] behave differently.
;For [CS] when (0) can only be executed from the ring set in DPL. If (1) can be executed with equal or lover privileges set in DPL; 
SEG_ACCESS_DC equ 1 << 2

;Executable bit
;if (0) = [DS]
;if (1) = [CS]
SEG_ACCESS_E equ 1 << 3

;if (0) = System Segment (eg. Task State Segment)
;if (1) = [CS] or [DS]
SEG_ACCESS_S equ 1 << 4

;Descriptor privilege level
;0 = Highest priv (Kernels)
;3 = lowest; Also see _RW bit
SEG_ACCESS_DPL_0 equ 0
SEG_ACCESS_DPL_1 equ 1 << 5
SEG_ACCESS_DPL_2 equ 2 << 5
SEG_ACCESS_DPL_3 equ 3 << 5

;Present bit.
; if (1) = this is a valid segment
; if (0) = cant access this
SEG_ACCESS_P equ 1 << 7

 
;[Long-mode code flag]
;if (1) 64-bit code segments
;if (0) 32-bit code segments
SEG_FLAGS_L equ 1 << 5

;size Flag
;if (0) defines 16 bit segment
;if (1) defines 32 bit segment
SEG_FLAGS_DB equ 1 << 6

;Granularity scale
;if (0) limit is in 1 Byte blocks
;if (1) limit is in 4 KB blocks (0x1000)
SEG_FLAGS_G equ 1 << 7


;(bits 0-3) of ACCESS BYTE for system Segment Descriptor 32 
SYS_SEG_ACCESS_TYPE32_TSS16_AVAILABLE equ 0x1
SYS_SEG_ACCESS_TYPE32_LDT equ 0x2
SYS_SEG_ACCESS_TYPE32_TSS16_BUSY equ 0x3
SYS_SEG_ACCESS_TYPE32_TSS32_AVAILABLE equ 0x9
SYS_SEG_ACCESS_TYPE32_TSS32_BUSY equ 0xb

;(bits 0-3) of ACCESS BYTE for system Segment Descriptor 64
SYS_SEG_ACCESS_TYPE64_LDT equ 0x2
SYS_SEG_ACCESS_TYPE64_TSS64_AVAILABLE equ 0x9
SYS_SEG_ACCESS_TYPE64_TSS64_BUSY equ 0xb



TEMP_GDT:
		.null 		dd 0x0000_0000
					dd 0x0000_0000

TEMP_GDT_CODE32:
		.lim0 		dw 0xffff
		.base0 		dw 0x0000 
		.base1		db 0x00
		.access		db 0x00 | SEG_ACCESS_RW |SEG_ACCESS_E|SEG_ACCESS_S|SEG_ACCESS_DPL_0|SEG_ACCESS_P
		.lim1_fl	db 0x0f | SEG_FLAGS_DB | SEG_FLAGS_G
		.base2		db 0x00

TEMP_GDT_DATA32:
		.lim0 		dw 0xffff
		.base0 		dw 0x0000 
		.base1		db 0x00
		.access		db 0x00 | SEG_ACCESS_RW |SEG_ACCESS_S|SEG_ACCESS_DPL_0|SEG_ACCESS_P
		.lim1_fl	db 0x0f | SEG_FLAGS_DB | SEG_FLAGS_G
		.base2		db 0x00

TEMP_GDT_CODE64:
		.lim0 		dw 0xffff
		.base0 		dw 0x0000 
		.base1		db 0x00
		.access		db 0x00 | SEG_ACCESS_RW |SEG_ACCESS_E|SEG_ACCESS_S|SEG_ACCESS_DPL_0|SEG_ACCESS_P
		.lim1_fl	db 0x0f | SEG_FLAGS_L | SEG_FLAGS_G 
		.base2		db 0x00

TEMP_GDT_DATA64:
		.lim0 		dw 0xffff
		.base0 		dw 0x0000 
		.base1		db 0x00
		.access		db 0x00 | SEG_ACCESS_RW |SEG_ACCESS_S|SEG_ACCESS_DPL_0|SEG_ACCESS_P
		.lim1_fl	db 0x0f | SEG_FLAGS_DB | SEG_FLAGS_G
		.base2		db 0x00


TEMP_GDT_PTR:
    .len	dw $ - TEMP_GDT - 1
    .add	dq TEMP_GDT

BOOT_DRIVE db 0x00

DAP:
			db 0x10
			db 0x0
	.num	dw 34-1 ; num secs to read
	.off 	dw 0x1000
	.seg 	dw 0x0000
	.ladd	dd 0x01; lower
	.hadd	dd 0x00; higher


 

ERR_ZERO_LEN zstr_nl "ZERO_LEN"
 
STR_SECOND_STAGE zstr_nl "SECOND STAGE"

STR_ERROR_DISK zstr_nl "DISK ERROR"

times 512*2 - ($-$$) db 0
;adjust in the build scropt

