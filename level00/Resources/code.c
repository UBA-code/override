#include <stdio.h>
#include <stdlib.h>

int main() {
    int user_input;
    
    puts("===================");
    puts("Enter the password:");
    puts("===================");
    
    printf("> ");
    
    scanf("%d", &user_input);
    
    if (user_input == 5276) {
        puts("Access granted!");
        system("/bin/sh");
        return 0;
    } else {
        puts("Access denied!");
        return 1;
    }
}