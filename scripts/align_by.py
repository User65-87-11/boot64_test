# import sys

import sys
import os

# print(sys.argv)

import subprocess


file_path = sys.argv[1]
size = os.path.getsize(file_path)
print(f"\tInput file size: {size} bytes")

 

in_size = int(size)
unaligned = in_size % 512
align_by = 512 - unaligned
new_size = in_size + align_by
 
# sys.stdin.write(str(align_by))





print("\tAligning by " +str(align_by)+" bytes, to:"+str(new_size)+" bytes")

subprocess.run([
    "dd",
    "if=/dev/zero",
    "of="+sys.argv[1],
    "bs=1c",
    "oflag=append",
    "conv=notrunc",
    "status=none",
    "count="+str(align_by)
])
