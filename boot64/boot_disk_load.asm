; 	AH = 02
; 	AL = number of sectors to read	(1-128 dec.)
; 	CH = track/cylinder number  (0-1023 dec., see below)
; 	CL = sector number  (1-17 dec.)
; 	DH = head number  (0-15 dec.)
; 	DL = drive number (0=A:, 1=2nd floppy, 80h=drive 0, 81h=drive 1)
; 	ES:BX = pointer to buffer
disk_geom:

; AH = 08h
; DL = Drive number


; Output:
; CH = Maximum value for cylinder (10-bit value; upper 2 bits in CL)
; CL = Maximum value for sector
; DH = Maximum value for heads

; For fixed disks:
; AH = Status of operation
; DL = Number of fixed disks
; CF = Set if error; otherwise cleared
	pusha
	
	
	

; rbx            0x1                 1
; rcx            0x5                 5
; rdx            0x21930180          563282304
	mov ah,0x8
	mov dl, [BOOT_DRIVE]

	int 0x13
	jc disk_error ; if error (stored in the carry bit)
 

 
	;read max cylinders with 2 bits from CL 
	; C = 0
	mov bh, cl
	rol bh, 2
	mov bl, ch
	and bx, 0x02ff
	call print_hex

	; read max secotrs per track
	; SPT = 5
	movzx  bx, cl
	and bx, 0x00df
	call print_hex


	;read max heads
	; HPC? = 0x14 = 20
	movzx bx,dh
	call print_hex
	call print_nl
	; C:0 S:1F H:0f
	; LBA = (C × HPC + H) × SPT + (S − 1)
	; LSize = (1*1f*0f)
	; C = LBA ÷ (HPC × SPT)
	; H = (LBA ÷ SPT) mod HPC
	; S = (LBA mod SPT) + 1
	; LBA = (512*20)   
	;	C = (512*20) / (20*5) = 102.4 = 102
	;	H = ((512*20) / 5) mod 20 =  2048 mod 20 = 8
	;	S = ((512*20) mod 5) + 1 = 10240 mod 5  + 1 = 1
	popa
	ret
disk_load:
		; load 'dh' sectors from drive 'dl' into ES:BX
		
		pusha
		; reading from disk requires setting specific values in all registers
		; so we will overwrite our input parameters from 'dx'. Let's save it
		; to the stack for later use.
		push dx

		mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = 'read'
		mov al, dh   ;(2) al <- number of sectors to read (0x01 .. 0x80)
		mov cl, 0x02 ;(2->) cl <- sector (0x01 .. 0x11)
					; 0x01 is our boot sector, 0x02 is the first 'available' sector
		mov ch, 0x00 ; ch <- cylinder (0x0 .. 0x3FF, upper 2 bits in 'cl')

		; (0x80)dl <- drive number. Our caller sets it as a parameter and gets it from BIOS
		; (0 = floppy, 1 = floppy2, 0x80 = hdd, 0x81 = hdd2)
	; call print_hex
	; 	jmp $	
		
		mov dh, 0x00 ; dh <- head number (0x0 .. 0xF)
		push SECOND_PART_DATA_SEG
		pop es
		mov bx, 0x0000
		; [es:bx] <- pointer to buffer where the data will be stored
		; caller sets it up for us, and it is actually the standard location for int 13h
		int 0x13      ; BIOS interrupt
		
		jc disk_error ; if error (stored in the carry bit)

		pop dx
		cmp al, dh    ; BIOS also sets 'al' to the # of sectors read. Compare it.
		jne sectors_error
		popa
		ret


disk_error:
		mov bx, DISK_ERROR
		call print
		; call print_nl
		mov dh, ah ; ah = error code, dl = disk drive that dropped the error
		movzx bx,DH
		call print_hex ; check out the code at http://stanislavs.org/helppc/int_13-1.html
		call print_nl
		jmp disk_loop

sectors_error:
		mov bx, SECTORS_ERROR
		call print

disk_loop:
	.idle_loop:
    hlt         ; halt CPU until the next interrupt
    jmp .idle_loop

DISK_ERROR: db "Disk read error", 0x0A, 0x0D,0
SECTORS_ERROR: db "Incorrect number of sectors read",0x0A, 0x0D, 0

DISK_PARAMS db "CYL, SEC, HEAD", 0x0A, 0x0D,0
