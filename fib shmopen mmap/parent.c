// Using shm_open and mmap
// parent.c
//
// This program demonstrates inter-process communication using POSIX shared memory and mmap.
// The parent process forks a child, which generates the first N Fibonacci numbers and writes them to shared memory.
// The parent then reads and prints the Fibonacci sequence from shared memory.
//
// How to run:
// 1. Compile both parent.c and child.c (add -lrt for POSIX shared memory):
//    cc parent.c -o parent -lrt
//    cc child.c -o fib -lrt
// 2. Run the parent program with N as argument (e.g., N=10):
//    ./parent 10
//    (The parent will fork and run the child automatically)
//
// Note: Run on Linux or WSL. Both executables must be in the same directory.

#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>     // for wait()
#include <stdio.h>
#include <string.h>
#include <unistd.h>       // for fork(), exec, etc.
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>        // for shm_open
#include <sys/mman.h>     // for mmap

int main(int argc, char *argv[]) {
    int i;
    pid_t pid;

    const int SIZE = 4096;  // Shared memory size
    int shm_fd;             // Shared memory file descriptor
    void *ptr;              // Pointer to shared memory

    // Check if user passed a command-line argument
    if (argc > 1) {
        sscanf(argv[1], "%d", &i);  // Read input as integer
        if (i < 1) {
            printf("Error input: %d\n", i);
            return 0;
        }
    } else {
        // No argument provided
        printf("Error: not passing N in command line\n");
        exit(0);
    }

    // Create a child process
    pid = fork();

    if (pid == 0) {
        // ---- CHILD PROCESS ----
        // Replace this process with the 'fib' executable
        // Pass the argument N as argv[1]
        execlp("./child", "child", argv[1], NULL);
    } else if (pid > 0) {
        // ---- PARENT PROCESS ----

        // Wait for child to complete execution
        wait(NULL);
        printf("\nPARENT: child completed\n");

        // Open the shared memory object created by child (in fib.c)
        shm_fd = shm_open("VSS", O_RDONLY, 0666);

        // Map shared memory object into parent's address space
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

        // Read and print data from shared memory
        printf("Parent printing:\n");
        printf("%s ", (char *)ptr);  // Cast and print string

        // Unlink shared memory so it's removed
        shm_unlink("VSS");
    }

    return 0;
}
