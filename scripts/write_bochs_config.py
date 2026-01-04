
import sys
import os

# print(sys.argv)

import subprocess

print(f"Write [bochs] config")

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
#solve it when it goes above 63
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


path2 = "scripts/bochs.config"

# print(os.path.abspath(path2))

with open(path2, "w+") as f:
	f.write(f"ata0-master: type=disk, path="+path1+f", cylinders={CYL}, heads={HEAD}, spt={SEC}\n")
	f.write("boot: disk\n")
	f.write("megs: 1024\n")
	f.write("pci: enabled=1, chipset=i440fx\n")
 
	# f.write("update_freq: 10\n")
	# 6MHZ(instructions per sec) 
	f.write("cpu: ips=6000000\n")
     
	f.write("clock: sync=slowdown, time0=local\n")
    # type=at
	f.write("keyboard: type=mf, serial_delay=150, paste_delay=100000\n")

	