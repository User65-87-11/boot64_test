
BUFFER equ 0x2000 ; - 0x7000
INITIAL_STACK equ 0x2000
SECOND_STACK equ 0x80000 ; 0x9_0000
KERNEL_LOADER equ 0x8000 ; 0x9_0000
KERNEL_LOADER_SEC_NUM equ 0x38	
KERNEL_LOADER_SECOND_SEC_NUM equ 0x2	
KERNEL_LOADER_SECOND equ 0x1000
KERNEL_LOADER_ELF_OFFSET equ KERNEL_LOADER ;+ 0x400

CODE_SEG  equ   0x08
DATA_SEG   equ  0x10

KERNEL_ADD equ 0x100000

;  32 bit
; gdtr DW 0 ; For limit storage
;      DD 0 ; For base storage

; 64 bit
; gdtr DW 0 ; For limit storage
;      DQ 0 ; For base storage

INTERRUPT_TABLE_64 equ 0x6000
INTERRUPT_TABLE_64_SIZE equ 0x1000 

INTERRUPT_TABLE_32 equ INTERRUPT_TABLE_64 + INTERRUPT_TABLE_64_SIZE
INTERRUPT_TABLE_32_SIZE equ 0x800




MEMORY_MAP_BUFFER1 equ INTERRUPT_TABLE_32 + INTERRUPT_TABLE_32_SIZE
MEMORY_MAP_BUFFER_ENTRY_SIZE equ 8*3 ;24
MEMORY_MAP_BUFFER_MAX_ENTRIES equ 16
MEMORY_MAP_BUFFER_SIZE equ MEMORY_MAP_BUFFER_ENTRY_SIZE * MEMORY_MAP_BUFFER_MAX_ENTRIES ; 0x180
MEMORY_MAP_MAGIC equ 0x534D4150


	; MEMORY_MAP_BUFFER_SIZE_OFFSET1 equ GLOBALS
	; MEMORY_MAP_BUFFER_ENTRY_SIZE1 equ 8 * 4
	
GDT_LOCATION equ MEMORY_MAP_BUFFER1 + MEMORY_MAP_BUFFER_SIZE
GDT_SIZE equ 5*8 ; 0x28

;0x6000 + 0x1000 + 0x800 + 0x180 + 0x28 = 0x79a8



;put globals below here

GLOBALS equ 0x7a00

; GLOBALS equ 0x7c00 

; GDT_DESCRIPTOR equ 0x7BF0 ; size 16

; MEMORY_MAP_LARGEST_MEM_POINTER equ  0x7BE8 ; size 2 but 8

; MEMORY_MAP_BUFFER_NUM_ENTRIES equ  0x7BE0 ; size 2 but 8

; IDT_DESCRIPTOR equ 0x7BD0

; ;NULL,CS32,DS32,CS64,DS64
; GDT_SIZE equ 5 * 8
; GDT_LOCATION equ 0x7ba0 ; 
; GDT_DESCRIPTOR equ GDT_LOCATION + GDT_SIZE
; ;0x7ba+0x40+ 2+8 = 0x7bea

;#define MAX_COUNT COUNT + 100

 
GPT_HEADER equ BUFFER ; 92 bytes
	GPT_HEADER_SIZE equ 92
	GPT_HEADER_PARTITION_ENTRY_SIZE_OFFSET equ 84
	GPT_HEADER_PARTITION_ENTRY_NUMBER_OFFSET equ 80
	GPT_HEADER_SEGMENT_SIZE equ 512



GPT_PARTITION_ENTRY_BUFF equ BUFFER + GPT_HEADER_SEGMENT_SIZE ; 128 bytes
	GPT_PARTITION_ENTRY_SIZE_LIMIT equ 128
	GPT_PARTITION_ENTRY_BUFF_NAME_OFFSET equ 0x38
	MAX_ENTRIES equ 0x10
	GPT_PARTITION_ENTRY_BUFF_LEN equ GPT_PARTITION_ENTRY_SIZE_LIMIT * MAX_ENTRIES
 


; First uint64_t = Base address
; Second uint64_t = Length of "region" (if this value is 0, ignore the entry)
; Next uint32_t = Region "type"
	; Type 1: Usable (normal) RAM
	; Type 2: Reserved - unusable
	; Type 3: ACPI reclaimable memory
	; Type 4: ACPI NVS memory
	; Type 5: Area containing bad memory
; Next uint32_t = ACPI 3.0 Extended Attributes bitfield (if 24 bytes are returned, instead of 20)
	; Bit 0 of the Extended Attributes indicates if the entire entry should be ignored (if the bit is clear). This is going to be a huge compatibility problem because most current OSs won't read this bit and won't ignore the entry.
	; Bit 1 of the Extended Attributes indicates if the entry is non-volatile (if the bit is set) or not. The standard states that "Memory reported as non-volatile may require characterization to determine its suitability for use as conventional RAM."
	; The remaining 30 bits of the Extended Attributes are currently undefined.




; MEMORY_MAP_BUFFER equ GLOBALS  + 16
; 	MEMORY_MAP_BUFFER_SIZE_OFFSET equ GLOBALS
; 	MEMORY_MAP_BUFFER_ENTRY_SIZE equ 8 * 4
; 	MEMORY_MAP_BUFFER_MAX_ENTRIES equ 10

; MEMORY_MAP_MARGEST_POINTER_32 equ MEMORY_MAP_BUFFER + (MEMORY_MAP_BUFFER_ENTRY_SIZE * MEMORY_MAP_BUFFER_MAX_ENTRIES)


 

;affects  -Ttext 0x8400
;dd if=$@ of=resources/gptdisk.img bs=1b count=2046 seek=2050 conv=notrunc	
;in second part : times 512*2 - ($-$$) db 0





