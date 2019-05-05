#!/usr/bin/env python
import os
import subprocess
import sys

subprocess.call("rm -f ./a.out", shell=True)
retcode = subprocess.call("/usr/bin/g++ uploads/walk.cc", shell=True)

if retcode:
	print("failed to compile walk.cc")
	sys.exit()

subprocess.call("rm -f ./output", shell=True)
retcode = subprocess.call("./test.sh", shell=True)

print ("Score: " + str(retcode) + " out of 2 correct.")
print("*************Original submission*************")
with open('uploads/walk.cc','r') as fs:
	print(fs.read())
