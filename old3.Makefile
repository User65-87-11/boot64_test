
# --

BUILD_DIR = out
RES_DIR = resources

PYTHON = python3

CFLAGS = -g -std=c23 -m32
LD_FLAGS = -m elf_i386 

OBJECTS_TO_BUILD = $(BUILD_DIR)/boot.bin $(BUILD_DIR)/second_part.bin $(BUILD_DIR)/kernel.bin

 
ALL: CLEAN INIT ${OBJECTS_TO_BUILD} make_cfg  set_sig

INIT:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${RES_DIR}
	mkdir -p ${BUILD_DIR}/protected

CLEAN:
	rm -r ${BUILD_DIR}

elf: $(BUILD_DIR)/kernel.elf
	

# boot
$(BUILD_DIR)/boot.bin: boot64/boot6GPT.asm
	nasm  -f bin -o $@ 	$^
	dd if=$@ of=resources/gptdisk.img bs=1c count=447 conv=notrunc
	

make_cfg: 
	${PYTHON} scripts/write_qemu_config.py 
	${PYTHON} scripts/write_bochs_config.py 



$(BUILD_DIR)/second_part.bin: boot64/second_part3.asm
	nasm -f bin -o $@ $^ 
	dd if=$@ of=resources/gptdisk.img bs=1b count=2048 seek=2048 conv=notrunc
 
$(BUILD_DIR)/entry.o: boot64/protected_entry3.asm
	nasm -f elf32 -o $@ $^ 

$(BUILD_DIR)/protected/kernel.o: protected/kernel.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

$(BUILD_DIR)/kernel.bin:$(BUILD_DIR)/entry.o $(BUILD_DIR)/protected/kernel.o 
	ld  -Ttext 0x10200  $(LD_FLAGS) --oformat binary  -o $@ $^ 
	dd if=$@ of=resources/gptdisk.img bs=1b count=2047 seek=2049 conv=notrunc	
# 	seek 2048+1 ; write 2048- 1 because the first sector is already there

copy_to_partition:
	

load_second_part:
	dd if=resources/sig.start.bin of=resources/gptdisk.img bs=1c count=7 seek=1048576 conv=notrunc

set_sig:
	dd if=resources/sig.mid.bin of=resources/gptdisk.img bs=1c count=4 seek=1572860 conv=notrunc
	dd if=resources/sig.end.bin of=resources/gptdisk.img bs=1c count=3 seek=2097149 conv=notrunc

	
# 	2097152 - 3 = 2097149
# 	1572864 - 4 = 1572860 haslfway at  0.5MB

# # PROTECTED 
# $(BUILD_DIR)/protected_entry3.o: boot64/protected_entry3.asm
# 	nasm $^  -f elf -o $@ 	

# $(BUILD_DIR)/protected/kernel.o: protected/kernel.c
# 	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@

# $(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/protected/kernel.o $(BUILD_DIR)/protected_entry3.o
# 	ld  -Ttext 0x1000  $(LD_FLAGS) --oformat binary  -o $@ $^ 


# # copy
# ${BUILD_DIR}/boot.img: $(BUILD_DIR)/boot.o  $(BUILD_DIR)/kernel.bin
# 	cat $^ > $@
# 	${PYTHON} scripts/align_by.py $@
# 	${PYTHON} scripts/write_qemu_config.py $@
# 	${PYTHON} scripts/write_bochs_config.py $@


# ${BUILD_DIR}/boot.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_stage.o
# 	cat $^ > $@

 