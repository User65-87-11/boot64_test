
# build_dir = build

# rule nasm_bin
#  command = nasm $in -f bin -o $out

# rule copy_img
#  command = cat $in > $out

# rule ld_bin
#  command = ld -Ttext ? -oformat binary -o $out $in

 

# build ${build_dir}/boot.o:   nasm_bin boot64/boot.asm

# build ${build_dir}/switch.o:  nasm_bin boot64/long_switch.asm

# build ${build_dir}/second_stage.o:  nasm_bin boot64/second_stage.asm

# build ${build_dir}/boot.img:   copy_img ${build_dir}/boot.o ${build_dir}/switch.o ${build_dir}/switch.o ${build_dir}/zfile

# build ${build_dir}/boot100MB.img:   copy_img ${build_dir}/boot.o ${build_dir}/switch.o ${build_dir}/switch.o ${build_dir}/zfile100MB

# build ${build_dir}/boot65kb.img:   copy_img ${build_dir}/boot.o ${build_dir}/switch.o ${build_dir}/switch.o ${build_dir}/zfile65kb


# --

BUILD_DIR = build
RES_DIR = ressources

OBJECTS_TO_BUILD = ${BUILD_DIR}/boot.img ${BUILD_DIR}/boot65kb.img ${BUILD_DIR}/boot100mb.img

ALL: CLEAN INIT ${OBJECTS_TO_BUILD}

INIT:
	mkdir -p ${BUILD_DIR}
	mkdir -p ${RES_DIR}

CLEAN:
	rm -r ${BUILD_DIR}

${RES_DIR}/zfile100mb:
	dd count=100 bs=1M if=/dev/zero of=$@ 

${RES_DIR}/zfile65kb:
	dd count=65 bs=1K if=/dev/zero of=$@ 

${RES_DIR}/zfile:
	dd count=1 bs=16K if=/dev/zero of=$@ 

$(BUILD_DIR)/boot.o: boot64/boot.asm
	nasm $^ -f bin -o $@ 	

$(BUILD_DIR)/second_stage.o: boot64/second_stage.asm
	nasm $^ -f bin -o $@ 	

${BUILD_DIR}/boot.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_stage.o ${RES_DIR}/zfile
	cat $^ > $@

${BUILD_DIR}/boot65kb.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_stage.o ${RES_DIR}/zfile65kb
	cat $^ > $@

${BUILD_DIR}/boot100mb.img: $(BUILD_DIR)/boot.o $(BUILD_DIR)/second_stage.o ${RES_DIR}/zfile100mb
	cat $^ > $@