
# --

BUILD_DIR = out
RES_DIR = resources
KERNEL_DIR = kernel



PYTHON = python3

CFLAGS = -g -std=c23 -m32 -fno-zero-initialized-in-bss
LD_FLAGS = -m elf_i386 

OBJECTS_TO_BUILD = $(BUILD_DIR)/boot.bin $(BUILD_DIR)/second_part.bin $(BUILD_DIR)/kernel_loader.bin

 
ALL: CLEAN INIT protected_header ${OBJECTS_TO_BUILD} make_cfg  set_sig

INIT:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${BUILD_DIR}/${KERNEL_DIR}
	mkdir -p ${RES_DIR}
	

CLEAN:
	rm -r ${BUILD_DIR}

elf: $(BUILD_DIR)/kernel_loader.elf
	

# boot
$(BUILD_DIR)/boot.bin: boot64/boot7GPT.asm
	nasm  -f bin -o $@ 	$^
	dd if=$@ of=resources/gptdisk.img bs=1c count=447 conv=notrunc
	

make_cfg: 
	${PYTHON} scripts/write_qemu_config.py 
	${PYTHON} scripts/write_bochs_config.py 

protected_header:
	${PYTHON} scripts/copy_asm_labels.py 

$(BUILD_DIR)/second_part.bin: boot64/second_part4.asm
	nasm -f bin -o $@ $^ 
	dd if=$@ of=resources/gptdisk.img bs=1b count=2048 seek=2048 conv=notrunc
 
$(BUILD_DIR)/entry.o: ${KERNEL_DIR}/protected_entry3.asm
	nasm -f elf32 -o $@ $^ 

$(BUILD_DIR)/$(KERNEL_DIR)/interrupt32.o: ${KERNEL_DIR}/interrupt32.asm
	nasm -f elf32 -o $@ $^ 
	
	

# $(BUILD_DIR)/${KERNEL_DIR}/interrupt.o: ${KERNEL_DIR}/interrupt32.asm
# 	nasm $^ -f elf32 -o $@ 	

$(BUILD_DIR)/${KERNEL_DIR}/32bit-mem.o: ${KERNEL_DIR}/32bit-mem.asm
	nasm $^ -f elf32 -o $@ 	



$(BUILD_DIR)/${KERNEL_DIR}/kernel_loader.o: ${KERNEL_DIR}/kernel_loader.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/display.o: ${KERNEL_DIR}/display.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/loop.o: ${KERNEL_DIR}/loop.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@


$(BUILD_DIR)/${KERNEL_DIR}/ports.o: ${KERNEL_DIR}/ports.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@


$(BUILD_DIR)/${KERNEL_DIR}/timer.o: ${KERNEL_DIR}/timer.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@	

$(BUILD_DIR)/${KERNEL_DIR}/pic.o: ${KERNEL_DIR}/pic.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@ 	

$(BUILD_DIR)/${KERNEL_DIR}/idt.o: ${KERNEL_DIR}/idt.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/cmd.o: ${KERNEL_DIR}/cmd.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@


$(BUILD_DIR)/${KERNEL_DIR}/isr.o: ${KERNEL_DIR}/isr.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/mem.o: ${KERNEL_DIR}/mem.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/memory_map.o: ${KERNEL_DIR}/memory_map.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@


$(BUILD_DIR)/${KERNEL_DIR}/keyboard.o: ${KERNEL_DIR}/keyboard.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/${KERNEL_DIR}/rsdt.o: ${KERNEL_DIR}/rsdt.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@	

$(BUILD_DIR)/${KERNEL_DIR}/pcie.o: ${KERNEL_DIR}/pcie.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@
	


$(BUILD_DIR)/kernel_loader.elf: \
	$(BUILD_DIR)/entry.o \
	$(BUILD_DIR)/${KERNEL_DIR}/kernel_loader.o \
	$(BUILD_DIR)/${KERNEL_DIR}/32bit-mem.o \
	$(BUILD_DIR)/${KERNEL_DIR}/idt.o \
	$(BUILD_DIR)/${KERNEL_DIR}/display.o \
	$(BUILD_DIR)/${KERNEL_DIR}/keyboard.o \
	$(BUILD_DIR)/${KERNEL_DIR}/timer.o \
	$(BUILD_DIR)/${KERNEL_DIR}/isr.o \
	$(BUILD_DIR)/${KERNEL_DIR}/ports.o \
	$(BUILD_DIR)/${KERNEL_DIR}/cmd.o \
	$(BUILD_DIR)/${KERNEL_DIR}/loop.o \
	$(BUILD_DIR)/${KERNEL_DIR}/rsdt.o \
	$(BUILD_DIR)/${KERNEL_DIR}/pcie.o \
	$(BUILD_DIR)/${KERNEL_DIR}/pic.o \
	$(BUILD_DIR)/${KERNEL_DIR}/memory_map.o \
	$(BUILD_DIR)/$(KERNEL_DIR)/interrupt32.o\
	$(BUILD_DIR)/${KERNEL_DIR}/mem.o 

#(-) $(BUILD_DIR)/${KERNEL_DIR}/interrupt.o 

	ld  -Ttext 0x8000  $(LD_FLAGS)  -o $@ $^ 
# 	dd if=$@ of=resources/gptdisk.img bs=1b count=2046 seek=2050 conv=notrunc	 

$(BUILD_DIR)/kernel_loader.bin: \
	$(BUILD_DIR)/entry.o \
	$(BUILD_DIR)/${KERNEL_DIR}/kernel_loader.o \
	$(BUILD_DIR)/${KERNEL_DIR}/32bit-mem.o \
	$(BUILD_DIR)/${KERNEL_DIR}/idt.o \
	$(BUILD_DIR)/${KERNEL_DIR}/display.o \
	$(BUILD_DIR)/${KERNEL_DIR}/keyboard.o \
	$(BUILD_DIR)/${KERNEL_DIR}/timer.o \
	$(BUILD_DIR)/${KERNEL_DIR}/loop.o \
	$(BUILD_DIR)/${KERNEL_DIR}/isr.o \
	$(BUILD_DIR)/${KERNEL_DIR}/ports.o \
	$(BUILD_DIR)/${KERNEL_DIR}/memory_map.o \
	$(BUILD_DIR)/${KERNEL_DIR}/cmd.o \
	$(BUILD_DIR)/${KERNEL_DIR}/rsdt.o \
	$(BUILD_DIR)/${KERNEL_DIR}/pcie.o \
	$(BUILD_DIR)/${KERNEL_DIR}/pic.o \
	$(BUILD_DIR)/$(KERNEL_DIR)/interrupt32.o\
	$(BUILD_DIR)/${KERNEL_DIR}/mem.o 

#(-) $(BUILD_DIR)/${KERNEL_DIR}/interrupt.o 
# 	1049600 + c23c = 16956; 16956 = 423C ? 418C  = B0
# 	ld  -Ttext 0x8000  $(LD_FLAGS) --oformat binary  -o $@ $^ 
	ld  -T scripts/linker.script  $(LD_FLAGS) --oformat binary  -o $@ $^ 

	dd if=$@ of=resources/gptdisk.img bs=1b count=2046 seek=2050 conv=notrunc	
# 	seek 2048+2 ; write 2048- 2 because the first sector is already there

copy_to_partition:
	

 
set_sig:
	dd if=resources/sig.mid.bin of=resources/gptdisk.img bs=1c count=4 seek=1572860 conv=notrunc
	dd if=resources/sig.end.bin of=resources/gptdisk.img bs=1c count=3 seek=2097149 conv=notrunc

 