#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/prctl.h>
#include <unistd.h>
#include <signal.h>

int main() {
    char buffer[128];  // 0x20(%esp) with size 0x80 (128 bytes)
    int status;
    pid_t pid;
    long orig_eax;
    
    // Fork the process
    pid = fork();
    
    if (pid == 0) {
        // Child process
        // Allow parent to trace this process
        prctl(PR_SET_PTRACER, 1, 0, 0, 0);
        
        // Start tracing
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        
        // Prompt for input
        puts("Enter input:");
        gets(buffer);  // Vulnerable function!
        
    } else {
        // Parent process
        // Wait for child to stop
        wait(&status);
        
        // Check if child terminated normally
        if (WIFEXITED(status) || WIFSIGNALED(status)) {
            puts("Child exited");
            return 0;
        }
        
        // Loop to trace syscalls
        while (1) {
            // Get the syscall number (register ORIG_EAX, offset 0x2c = 44)
            orig_eax = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
            
            // Check if syscall is 0xb (11 = execve)
            if (orig_eax == 0xb) {
                puts("Detected execve syscall!");
                // Kill the child process
                kill(pid, SIGKILL);
                break;
            }
            
            // Continue tracing
            wait(&status);
            
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                puts("Child exited");
                break;
            }
        }
    }
    
    return 0;
}