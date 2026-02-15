#include <string.h>
#include <stdio.h>
#include <sys/ptrace.h>

int auth(char *username, int serial) {
    int username_len;
    int calculated_serial;
    int i;
    
    // Remove newline from username
    username[strcspn(username, "\n")] = '\0';
    
    // Get username length (max 32)
    username_len = strnlen(username, 32);
    
    // Check 1: Username must be longer than 5 characters
    if (username_len <= 5) {
        return 1;  // Authentication failed
    }
    
    // Check 2: Anti-debugging check using ptrace
    if (ptrace(PTRACE_TRACEME, 0, 0, 0) == -1) {
        puts("\x1b[32m.---------------------------.");
        puts("\x1b[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1b[32m'---------------------------'");
        return 1;  // Authentication failed
    }
    
    // Calculate the expected serial based on username
    // Start with character at username[3]
    calculated_serial = (username[3] ^ 0x1337) + 0x5eeded;
    
    // Loop through each character in username
    for (i = 0; i < username_len; i++) {
        // Check: All characters must be > 0x1f (printable ASCII)
        if (username[i] <= 0x1f) {
            return 1;  // Authentication failed
        }
        
        // Complex calculation on each character
        int char_val = (int)username[i];
        
        // XOR with current calculated_serial
        char_val ^= calculated_serial;
        
        // Modulo 1337 operation (optimized division)
        // This is: char_val % 1337
        int temp = char_val;
        temp = (temp * 0x88233b2b) >> 32;  // Magic division
        temp = (temp + char_val) >> 1;
        temp >>= 10;
        temp *= 1337;
        int remainder = char_val - temp;
        
        // Add remainder to calculated_serial
        calculated_serial += remainder;
    }
    
    // Check 3: Compare calculated serial with provided serial
    if (serial == calculated_serial) {
        return 0;  // Authentication successful
    }
    
    return 1;  // Authentication failed
}

int main(int argc, char **argv) {
    char username[32];  // Buffer at esp+0x2c, size 0x20 (32 bytes)
    int serial;         // Variable at esp+0x28
    
    // Print banner
    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");
    
    // Get username
    printf("-> Enter Login: ");
    fgets(username, 32, stdin);
    
    // Print separator
    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");
    
    // Get serial number
    printf("-> Enter Serial: ");
    scanf("%d", &serial);
    
    // Authenticate
    if (auth(username, serial) == 0) {
        puts("Authenticated!");
        system("/bin/sh");
        return 0;
    } else {
        return 1;
    }
}