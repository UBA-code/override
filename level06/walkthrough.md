# Serial Number Algorithm

The program prompts login and serial number for user to authenticate. After debugging, the program generates a hashed serial number from the given login and compare it with a the serial inserted by user, if they match, it runs a shell environment.

Since the program uses `ptrace()` to check debugging mode, a break point should be set at the address of ptrace() function, and jump directly skipping the ptrace() check.

```
0x080487b5 <+109>:  call   0x80485f0 <ptrace@plt> ; ptrace check
0x080487ed <+165>:	mov    0x8(%ebp),%eax ; the address after ptrace check
```

Another break point should be set to break into the address of where the comparaison happens between the calculated_serial and the user's serial.
```
0x08048866 <+286>:    cmp    -0x10(%ebp),%eax ; address of serial comparaison
```

Running the program after setting up two break points at the right spots:
```
(gdb) b *0x080487b5 ;	just before ptrace
(gdb) b *0x08048866 ;	at serials comparaison
(gdb) r ; run the program
```

Jumping over the ptrace() check like:
```
(gdb) jump *0x080487ed ; jump over the check
```

Then reading the value of the calculated serial, after casting it from hexa to int:
```
(gdb)  p *(int*)($ebp-0x10) ; read the serial value
$1 = 6233767
```

The value of serial is: $1 = **6233767**.

It can be used to access authenticate and get the flag:
```
./level06
-> Enter Login: bonjour
-> Enter Serial: 0
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
```