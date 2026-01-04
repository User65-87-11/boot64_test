THIS equ 0x7c00

%IF 0

	BUFFER = 0x2000 - 0x7000 = 4x 16kb = 16kb = 0x5000
	INITIAL_STACK = 0x2000
	KERNEL_loader = 0x8000 - 0x9_0000
	PARTITION_TABLE = 0x7be0  : 0x7c00 - 0x20
	descriptor?
	
	MEMORY_MAP_TABLE equ BUFFER

	KERNEL_ADD = 0x10_0000

	256b = 0x0_00ff : 0 - 256 - 1
	256b = 0x0_0100 : len 256 
	512b = 0x0_0200
	04kb = 0x0_0fff : 0 - 4096 - 1
	04kb = 0x0_1000 : len 4096
	16kb = 0x0_3fff : 0 - 16384 - 1
	16kb = 0x0_4000 : len 16384
	32kb = 0x0_8000 
	64kb = 0x0_ffff : 0 - 65536 - 1
	64kb = 0x1_0000 : len 65536
	01mb = 0xf_ffff : 0 - 1_048_575 

	32kb in sectors =  0x0_8000  / 0x0200 = 40
%endif


[ORG THIS]



%include "boot64/const.inc"

;0x4000  = 16kb


;;;;;;GLOBALS ;;;;

%macro zstr_nl 1
	db %1, 0x0A, 0x0D,0
%endmacro 

%macro zstr 1
	db %1, 0
%endmacro 



; SECOND_PART_SEGMEN equ 0x1000 ; = [ds:1000]

; GPT_START_CHS_OFFSET equ 446 + THIS
; MAGIC_OFFSET equ 0x7DFE
; MAGIC_OFFSET_1MB equ 0x7E0E

 

; Protective MBR
; 440 bytes
; +6 unused
; offsets:
; +0(1) 0x00 ; bootable or not
; +1(3) CHS START
; +4(1) 0xEE
; +5(3) CHS ENDING
; +8(4) START_LBA
; +12(4) size of disk -1 or 0xFFFFFFFF if large 

;read at 439 + 6 + 1
;little endian 0xAABB = 0xBB 0xAA , LSB first
 

; by default operates with first 64 kib range
;around 0x1000 to 0x7c00 for the second stage
;16 KB 

[BITS 16]


Main:
		 
		xor ax, ax
		mov [BOOT_DRIVE], dl ;
		; Set up segment registers.
		mov ss, ax
		; Set up stack so that it starts below Main.
		mov sp, INITIAL_STACK
		
		mov ds, ax
		mov es, ax
		mov fs, ax
		mov gs, ax
		cld
		mov si, READ_GPT
		call print_zstr
		call print_nl

 
		 
		call read_lba


.check_GPT:

		mov cx,8
		mov di,GPT_HEADER
		mov si,EFI_PART
		REPE cmpsb
		jne error_gpt

		mov si,GPT_HEADER
		call print_zstr
		call print_nl
		
.read_partition_num_entries:
		mov si,STR_NUM_GPT_ENTRIES
		call print_zstr
		
		mov ax, [GPT_HEADER + GPT_HEADER_PARTITION_ENTRY_NUMBER_OFFSET]
		call print_hex
		cmp bx,MAX_ENTRIES
		ja error_many

		
.partition_entry_size:
		mov ax, [GPT_HEADER + GPT_HEADER_PARTITION_ENTRY_SIZE_OFFSET]
		call print_hex
		cmp ax,GPT_PARTITION_ENTRY_SIZE_LIMIT
		ja error_many

.find_my_partition:

		mov cx,0
		mov si,GPT_PARTITION_ENTRY_BUFF
.loop:	
		;save an entry to the buffer
		;check it
		;advance
		;repeat
		push cx
		mov cx,[GPT_HEADER + GPT_HEADER_PARTITION_ENTRY_SIZE_OFFSET]
		mov di,GPT_PARTITION_ENTRY_BUFF 
		rep movsb		
		add si, cx
		pop cx
		inc cx
		cmp cx, [GPT_HEADER + GPT_HEADER_PARTITION_ENTRY_NUMBER_OFFSET];[GPT_HEADER + 80]
		je .not_found
		
		mov cx,8
		mov di,GPT_PARTITION_ENTRY_BUFF + GPT_PARTITION_ENTRY_BUFF_NAME_OFFSET
		mov si,GPT_MAGIC
		REPE cmpsb
		je .found

		
		jmp .loop
.found:		
		mov si, NOT_FOUND + 4 ;string
		call print_zstr
		call print_nl

		; mov ax, KERNEL_LOADER
		; mov [DAP.off],ax
		; mov ax, 0x0000
		; mov [DAP.seg],ax
		; mov ax,0x800		; 2048 sectors
		; mov [DAP.ladd],ax
		; mov ax,KERNEL_LOADER_SEC_NUM
		; mov [DAP.num],ax
		
; 		KERNEL_LOADER_SECOND_SEC_NUM equ 0x2	
; KERNEL_LOADER_SECOND equ 0x1000
	 
		mov word [DAP.off],KERNEL_LOADER_SECOND
		mov word [DAP.seg],0x0000
		mov word [DAP.ladd],0x800
		mov word [DAP.num],KERNEL_LOADER_SECOND_SEC_NUM
		

		call read_lba
		

		mov dl,[BOOT_DRIVE]
		; mov ax,SECOND_PART_SEGMEN
		; mov ds,ax
		jmp 0:KERNEL_LOADER_SECOND

		jmp idle_loop
		
.not_found:
		mov si,NOT_FOUND
		call print_zstr
		call print_nl
		jmp idle_loop

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



[bits 16]
disk_error:
	mov si, DISK_ERROR
	call print_zstr
	;assume ax
	call print_hex ; check out the code at http://stanislavs.org/helppc/int_13-1.html
	call print_nl
	jmp idle_loop

error_many:
	mov si, TOO_MANY
	call print_zstr
	jmp idle_loop

sectors_error:
	mov si, SECTORS_ERROR
	call print_zstr
	jmp idle_loop

error_gpt:
	mov si, EFI_ERROR
	call print_zstr
	jmp idle_loop

idle_loop:
    hlt         ; halt CPU until the next interrupt
    jmp idle_loop


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

print_nl:
    pusha
    
    mov ah, 0x0e
    mov al, 0x0a ; newline char
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10
    
    popa
    ret
;;;;

 
TOO_MANY db "TOO_MANY", 0
NO_EXT db "NO_EXT",0
GPT_MAGIC db "D",0,"A",0,"D",0,"A",0

NOT_FOUND db "NOT_FOUND",0
STR_NUM_GPT_ENTRIES db "ENTRIES: ",0
 
;69,70,73 = 0x30737069
EFI_PART db "EFI PART", 0
EFI_ERROR db "EFI err" ,0
READ_GPT db "READ GPT",0
DISK_ERROR db "Disk read err",0
SECTORS_ERROR db "Incorr sect", 0


BOOT_DRIVE db 0x00

DAP:
			db 0x10
			db 0x0
	.num	dw 34-1 ; num secs to read
	.off 	dw BUFFER
	.seg 	dw 0x0000
	.ladd	dd 0x01; lower
	.hadd	dd 0x00; higher

 
; Pad out file.
times 446 - ($-$$) db 0
bootable db 0x01
; dw 0xAA55