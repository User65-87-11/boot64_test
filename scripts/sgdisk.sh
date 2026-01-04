#

#sgdisk


# Command (? for help): n
# Partition number (1-128, default 1): 1
# First sector (34-93716, default = 2048) or {+-}size{KMGTP}: 2048
# Last sector (2048-93716, default = 93716) or {+-}size{KMGTP}: 93716
# Current type is 'Linux filesystem'
# Hex code or GUID (L to show codes, Enter = 8300): ef00
# Changed type of partition to 'EFI System'

LOOPP=/dev/loop0
FILEP=resources/gptdisk.img

#maybe dont need to loop this
# losetup  $LOOPP $FILEP

# sgdisk -Z $LOOPP

# sgdisk -n 1:2048:93716 -G $LOOPP

# sgdisk -c 1:"Hello World!"  $LOOPP

# sgdisk -t 1:ef01 $LOOPP

# losetup -d $LOOPP

#------
sgdisk -Z $FILEP
sgdisk -n 1:2048:93716 -G $FILEP
sgdisk -c 1:"Hello World!"  $FILEP
sgdisk -t 1:ef01 $FILEP

dd if=build/boot.img of=$FILEP


