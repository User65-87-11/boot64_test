
# --

BUILD_DIR = build
RES_DIR = ressources

PYTHON = python3

CFLAGS = -g -std=c23 -m32
LD_FLAGS = -m elf_i386 

OBJECTS_TO_BUILD = ${BUILD_DIR}/boot.img 

ALL: CLEAN INIT ${OBJECTS_TO_BUILD}

INIT:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${RES_DIR}
	mkdir -p ${BUILD_DIR}/protected

CLEAN:
	rm -r ${BUILD_DIR}

elf: $(BUILD_DIR)/kernel.elf
	


${RES_DIR}/zfile100mb:
	dd count=100 bs=1M if=/dev/zero of=$@ 

${RES_DIR}/zfile65kb:
	dd count=65 bs=1K if=/dev/zero of=$@ 

${RES_DIR}/zfile:
	dd count=1 bs=16K if=/dev/zero of=$@ 

# boot
$(BUILD_DIR)/boot.o: boot64/boot2.asm
	nasm $^ -f bin -o $@ 	



# second stage
$(BUILD_DIR)/second_part.o: boot64/second_part.asm
	nasm $^  -f bin -o $@ 	

$(BUILD_DIR)/kernel_entry.o: boot64/kernel_entry.asm
	nasm $^  -f elf -o $@ 	


$(BUILD_DIR)/protected/entry.o: protected/entry.c
	gcc  $(CFLAGS) -ffreestanding -fno-pie -g -c $^ -o $@


$(BUILD_DIR)/kernel.bin: $(BUILD_DIR)/protected/entry.o $(BUILD_DIR)/kernel_entry.o
	ld  -Ttext 0x1000  $(LD_FLAGS) --oformat binary  -o $@ $^ 

$(BUILD_DIR)/kernel.elf: $(BUILD_DIR)/protected/entry.o $(BUILD_DIR)/kernel_entry.o
	ld  -Ttext 0x1000  $(LD_FLAGS)  -o $@ $^ 



# copy
${BUILD_DIR}/boot.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_part.o $(BUILD_DIR)/kernel.bin
	cat $^ > $@
	${PYTHON} scripts/align_by.py $@
	${PYTHON} scripts/write_qemu_config.py $@
	${PYTHON} scripts/write_bochs_config.py $@
# ${BUILD_DIR}/boot.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_stage.o
# 	cat $^ > $@

 