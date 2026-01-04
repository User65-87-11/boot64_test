import os

import re
print(f"Write constant header")

path0 = "boot64/const.inc"

# file_path = sys.argv[1]

with open(path0, "r") as f:
	lines = f.readlines()

i = 0
line_len = len(lines)
print(f"\tnum lines {line_len}")
cc =[]
while i < line_len:
	l = lines[i].strip().split()
	n = l
	if len(l) == 0:
		cc.append("")
		i+=1
		continue
	if l[0].startswith(";"):
		i+=1
		l = lines[i].strip().split()
	elif l[0] == r"%macro":
		i+=1
		l = lines[i].strip().split()

		while len(l)>0 and l[0] != r"%endmacro":
			i+=1
			l = lines[i].strip().split()
		i+=1
	else:  #normal
		
		c=[]
		c.append("#define")
		
		for a in l:
			if a.startswith(";"):
				break
			if a == "equ":
				continue
			
			if a.endswith(";"):
				a = a[0:-1]
				c.append(a)
				break
			
			c.append(a)
		
		c.insert(2," (")
		c.append(" )")
		t = " ".join(c)
		
		# print(t)
		cc.append(t)
	i+= 1
			
path2 = "kernel/boot_const.h"

if len(cc) == 0:
	print(f"FAILED to write: {path2}")
	exit(0)

with open(path2, "w+") as f:
	for c in cc:
		f.write(f"{c}\n")

