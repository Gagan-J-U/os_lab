/*
prime shmopen mmap - parent.c

This program demonstrates inter-process communication using POSIX shared memory and mmap.
The parent process forks a child, which finds all prime numbers in a given range and writes them to shared memory.
The parent then reads and prints the prime numbers from shared memory.

How to run:
1. Compile both parent.c and child.c (add -lrt for POSIX shared memory):
   cc parent.c -o parent -lrt
   cc child.c -o child -lrt
2. Run the parent program with two arguments (lower and upper bounds, e.g., 10 and 50):
   ./parent 10 50
   (The parent will fork and run the child automatically)

Note: Run on Linux or WSL. Both executables must be in the same directory.

Both programs are correct and will execute as intended, provided you follow the above steps and run on a compatible system.
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i, j;
    int shm_fd;                        // Shared memory file descriptor
    const int SIZE = 4096;             // Size of shared memory
    pid_t pid;                         // Process ID for fork
    void *ptr;                         // Pointer to shared memory

    // Validate command-line arguments
    if (argc > 2) {
        sscanf(argv[1], "%d", &i);     // Lower bound
        sscanf(argv[2], "%d", &j);     // Upper bound

        if (i < 2) {
            printf("Error input: %d\n", i);
            return 0;
        }
    } else {
        // Less than 2 arguments provided
        printf("Error: Minimum of two arguments must be passed as command line arguments!\n");
        exit(0);
    }

    // Fork a child process
    pid = fork();

    if (pid == 0) {
        // ---- CHILD PROCESS ----
        // Execute child process with arguments
        execlp("./child", "child", argv[1], argv[2], NULL);
    } else if (pid > 0) {
        // ---- PARENT PROCESS ----
        wait(NULL);  // Wait for child process to complete

        printf("\nParent: child complete!\n");

        // Open shared memory object created by child
        shm_fd = shm_open("VSS", O_RDONLY, 0666);

        // Map the shared memory into parent’s address space
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

        // Print data from shared memory (prime numbers)
        printf("%s", (char *) ptr);

        // Remove the shared memory object
        shm_unlink("VSS");
    }

    return 0;
}
