# 48MB : segments 93750

DISK=resources/gptdisk.img

dd if=/dev/zero of=$DISK bs=512 count=93750

# QEMU can automatically create a virtual FAT disk image from a specified directory tree
