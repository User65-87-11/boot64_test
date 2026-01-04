; shouldn't use them
; callee-saved regs vs caller saved regs danger

global mem_set32

global mem_copy32

global mem_rev_copy32

;global str_eq32
; global cstr_len32
global mem_eq32
 

global strz_len32
global str_utf16_len32

global mem_search32

global mem_search32_end

global mem_salloc32

; in 32BIT arguments (A,B,C) are passed
;	push(C),push(B),push(A)
; and accessed
; 	[esp+4],[esp+8],[esp+12]




; in 64BIT, the order of a fun(edi,esi,edx,ecx,r8,r9, stack...)
; edi, esi, edx, ecx, r8, r9  = gcc
; but this is 32, so restore via the stack

mem_salloc32:
	mov edi, [esp]
	mov esi, [esp + 4]

	mov eax,esp
	sub eax, 0x14
	sub esp, esi
	mov [esp],edi
	
	ret



mem_search32:
	push ebp
	mov	ebp, esp
	;;prologue
	 
	sub	esp, 4*2
 
	;ebp  - 8 ; mem.len - pat.len; save ecx?
	;ebp - 4 ; save edi
	 
	
	mov edx, DWORD   [ebp + 16 + 4] ; pattern.len
	mov esi, DWORD   [ebp + 12 + 4] ; char * pattern
	mov ecx, DWORD   [ebp + 8 + 4] ; mem.len
	mov edi, DWORD   [ebp + 4 + 4] ; char * mem.start
	sub ecx,edx
	inc ecx
	mov [ebp - 8],ecx
	mov [ebp - 4],edi

.loop:
	;; find al
	mov edi,[ebp - 4]
	mov ecx,[ebp - 8]
	mov esi,[ebp + 12 + 4]
	mov al,[esi]

	repne scasb; find al and stop
	
	
	jnz .not_found ;

 
		
		mov [ebp - 4],edi
		mov [ebp - 8],ecx

		mov ecx,edx
		dec edi

		repe cmpsb
		jnz .loop; 
		jz .found;

 

.not_found:
	mov eax,0xffffffff
	jmp .exit
.found:
	mov eax,[ebp - 4]
 	dec eax

	 
	
	;;should return 0xff is not found
	;;or 0 to 0xfe
	;;epilogue
.exit:	
 
	mov	esp, ebp
	pop	ebp
	ret

mem_eq32:
	; (a[esp+4],b[esp+8],c[esp+12]) without saving EBP
	; (a[esp+4+4],b[esp+8+4],c[esp+12+4]) WITH saving EBP
		mov eax, 1
		
		mov edi,[esp+4]
		mov esi,[esp+8]
		mov ecx,[esp+12]
		cld
		repe cmpsb 
	jz mem_eq32_exit
		mov eax,0
	mem_eq32_exit:
		ret 
 
;all strings as a blob
 
;--------
mem_copy32:

	mov ecx, DWORD   [esp + 12]
	mov esi, DWORD   [esp + 8]
	mov edi, DWORD   [esp + 4] 
	cld
	rep movsb
	ret


 
strz_len32:
	mov edi,[esp + 4]
	mov ecx,0
	mov al,0
.loop:
	inc ecx
	scasb
	jnz .loop
	dec ecx


	mov eax,ecx

 
	ret

str_utf16_len32:
	mov edi,[esp + 4]
	mov ecx,0
	mov ax,0
.loop:	
	inc ecx
	scasw
	jnz .loop
	dec ecx
	mov eax,ecx
	ret

mem_rev_copy32:
	mov ecx, DWORD   [esp + 12]
	cmp ecx,0
	je .exit

	mov esi, DWORD   [esp + 8]
	mov edi, DWORD   [esp + 4]
	add esi , ecx
	dec esi 
		
.loop:

	mov dl, [esi]
	mov [edi], dl
	dec esi
	inc edi
	dec ecx
	cmp ecx, 0
	jg .loop
.exit:
    ret	

 
mem_set32:

	mov ecx, DWORD   [esp + 12]
	mov al, BYTE   [esp + 8]
	mov edi, DWORD   [esp + 4]
	cld
	rep stosb

	ret

;;-------------

mem_search32_end:
	push ebp
	mov	ebp, esp
	;;prologue
	 
	sub	esp, 4*2
 
	;ebp  - 8 ; mem.len - pat.len; save ecx?
	;ebp - 4 ; save edi
	 
	
	mov edx, DWORD   [ebp + 16 + 4] ; pattern.len
	mov esi, DWORD   [ebp + 12 + 4] ; char * pattern
	mov ecx, DWORD   [ebp + 8 + 4] ; mem.len
	mov edi, DWORD   [ebp + 4 + 4] ; char * mem.start
	sub ecx,edi
	sub ecx,edx
	inc ecx
	mov [ebp - 8],ecx
	mov [ebp - 4],edi

.loop:
	;; find al
	mov edi,[ebp - 4]
	mov ecx,[ebp - 8]
	mov esi,[ebp + 12 + 4]
	mov al,[esi]

	repne scasb; find al and stop
	 
	; jz .found ; zf = 1 ; terminated
	jnz .not_found ;
	
	
 
		
		mov [ebp - 4],edi
		mov [ebp - 8],ecx


		mov ecx,edx
		; dec ecx 
		dec edi

		repe cmpsb
		jnz .loop; after all
		jz .found;


 

.not_found:
	mov eax,0xffffffff
	jmp .exit
.found:
	mov eax,[ebp - 4]
 	dec eax
	
	;;should return 0xff is not found
	;;or 0 to 0xfe
	;;epilogue
.exit:	
 
	mov	esp, ebp
	pop	ebp
	ret