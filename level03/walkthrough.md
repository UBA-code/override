# level03

after login to level03 user, there's an `level03` executable in the home directory.

after dissassembling the executable using `ghidra` and `binary ninja`, you can check the source code at [source.c](source.c).

the main function prompts for a password (integer) and passes it to the `test` function along with a constant integer `322424845`.

the `test` function first subtracts the integer we entered as password from the constant integer `322424845`, and then it check for the result inside a switch-case statement with multiple cases.

the switch case has the following cases (1,2,3,4,5,6,7,8,9,16,17,18,19,20,21).

and each case calls a function named `decrypt` with same case value.

the `decrypt` function has a hash <code>Q}|u`sfg~sf{}|a3</code>, and it iterate over each character of the hash and XOR it with a value based on the case value passed to it, then it compares the result with the string "Congratulations!", and if it matches, it spawns a shell.

so we now need to find the correct password that when subtracted from `322424845` gives us one of the case values in the switch-case statement that will produce the correct hash when passed to the `decrypt` function.

we will take the first character of the hash `Q` as example to find the correct case value.

binary value of `Q` is `01010001` and binary value of `C` (first character of "Congratulations!") is `01000011`.

we will use XOR operation to find the case value:

```
  01010001  (Q)
^ 01000011  (C)
-----------
  00010010
```

the decimal value of `00010010` is `18`.

so the case value that will produce the correct hash is `18`.

we need to enter `322424827` (322424845 - 18).

after entering the password `322424827`, we get a shell as level03 user.

```sh
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ whoami
level04
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
$
```
