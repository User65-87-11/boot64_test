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

NoLongMode db "N", 0x0A, 0x0D, 0