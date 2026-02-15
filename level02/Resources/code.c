#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    char username[100];
    char password_from_file[41];
    char user_password[100];
    FILE *fp;
    int bytes_read;
    
    memset(username, 0, 100);
    memset(password_from_file, 0, 41);
    memset(user_password, 0, 100);
    
    fp = fopen("r", ".pass");
    
    if (fp == NULL) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }
    
    bytes_read = fread(password_from_file, 1, 0x29, fp);
    
    password_from_file[strcspn(password_from_file, "\n")] = '\0';
    
    if (bytes_read != 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
    }
    
    fclose(fp);
    
    puts("***********************************");
    puts("*\t\tlevel03\t\t**");
    puts("***********************************");
    puts("Password:");
    
    printf("> ");
    fgets(username, 100, stdin);
    username[strcspn(username, "\n")] = '\0';
    
    printf("> ");
    fgets(user_password, 100, stdin);
    user_password[strcspn(user_password, "\n")] = '\0';
    
    puts("Verifying credentials...");
    
    if (strncmp(password_from_file, user_password, 0x29) == 0) {
        printf("Welcome, %s!\n", username);
        system("/bin/sh");
        return 0;
    } else {
        printf("%s\n", username);
        puts("Access denied!");
        exit(1);
    }
}