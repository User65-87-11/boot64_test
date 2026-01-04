# import sys

import sys
import os

# print(sys.argv)

import subprocess



print(f"Write [qemu] config")

path0 = "resources/gptdisk.img"

# file_path = sys.argv[1]
size = os.path.getsize(path0)

print("\tFile: "+path0)
print(f"\tFile size: {size} bytes")


# LBA is a sector address.
# CHS is also a sector address

size_in_sec = int(size / 512)

SEC = 0
HEAD = 1
CYL = 1

if size_in_sec >63:
   SEC  = 32
   CYL = int(size_in_sec / SEC) 
else:
   SEC  = int(size_in_sec)

print(f"\tSize in SEC:{size_in_sec}")
# 508


print(f"\tGeometry C:{CYL} H:{HEAD} S:{SEC}")
# path1 ="\""+ r"\\wsl.localhost\\Debian\\home\\user\\devel\\os64\\build\\boot.img" +"\""

path1 ="\""+ r"\\wsl.localhost\Debian\home\user\devel\os64\resources\gptdisk.img" +"\""



path2 = "scripts/qemu.config"

# print(os.path.abspath(path2))

with open(path2, "w+") as f:
    # f.write(f"ata0-master: type=disk, path="+path1+f", cylinders={CYL}, heads={HEAD}, spt={SEC}\n")
    # f.write("boot: disk")
	f.write("[device \"sata-disk\"]\n")
	f.write("driver = \"ide-hd\"\n")
	f.write("bus = \"ide.0\"\n")
	f.write("drive = \"disk0\"\n")
	f.write(f"cyls = \"{CYL}\"\n")
	f.write(f"heads = \"{HEAD}\"\n")
	f.write(f"secs = \"{SEC}\"\n")
	f.write("bios-chs-trans = \"none\"\n")
     
	f.write("[drive \"disk0\"]\n")
	f.write("if = \"none\"\n")
	f.write("format = \"raw\"\n")
	f.write("file = "+path1+"\n")
   

	f.write("[machine]\n")
	f.write("type = \"q35\"\n")


# 	 [machine]
#   type = "virt"
#   gic-version = "host"
          
 

# [device "sata-disk"]
# driver = "ide-hd"
# bus = "ide.0"
# drive = "disk0"
# cyls = "1"
# heads = "1"
# secs = "26"
# bios-chs-trans = "none"

# [drive "disk0"]
# if = "none"
# format = "raw"
# file = "\\wsl.localhost\Debian\home\user\devel\os64\build\boot.img"