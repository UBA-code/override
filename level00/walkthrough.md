# Password Crack

The program checks for a password, if the user sets '5276’ as input, it does call a shell environment using `system("/bin/sh”)`.

The command used to get the password in the shell:

```

level00@OverRide:~$ ./level00 
***********************************
* 	     -Level00 -		  *
***********************************

Password:5276
Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL

```