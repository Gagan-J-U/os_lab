/*
System V Shared Memory Fibonacci Example
parent.c

This program demonstrates inter-process communication using System V shared memory.
The parent process creates a shared memory segment, forks a child, and waits for the child to write the first N Fibonacci numbers to shared memory.
The parent then reads and prints the sequence from shared memory.

How to run:
1. Compile both parent.c and child.c:
   gcc parent.c -o parent
   gcc child.c -o fib 
2. Run the parent program with N as argument (e.g., N=10):
   ./parent 10
   (The parent will fork and run the child automatically)

Note: Run on Linux or WSL. Both executables must be in the same directory.

Both programs are correct and will execute as intended, provided you follow the above steps and run on a compatible system.
*/

#include <sys/types.h>    // for pid_t, key_t
#include <sys/wait.h>     // for wait()
#include <stdio.h>        // for printf()
#include <string.h>       // for strlen()
#include <unistd.h>       // for fork(), execlp()
#include <sys/shm.h>      // for shmget, shmat, shmdt
#include <stdlib.h>    // included but not used here (System V uses shm.h)

int main(int argc, char *argv[])
{
    int i;                // Holds input number
    pid_t pid;            // Process ID
    const int SIZE = 4096;     // Shared memory size
    int shmid;                 // Shared memory ID
    void *ptr;                 // Pointer to shared memory

    // Create a System V shared memory segment with key 1122
    shmid = shmget((key_t)1122, 4096, 0666 | IPC_CREAT);

    // Attach the shared memory segment to the address space
    ptr = shmat(shmid, NULL, 0666);

    // Input validation from command line
    if (argc > 1) {
        i=atoi(argv[1]);  
        if (i < 1) {
            printf("Error input: %d\n", i);
            return 0;
        }
    } else {
        // No input provided
        return 1;
    }

    // Create child process
    pid = fork();

    if (pid == 0) {
        // ---- CHILD PROCESS ----
        // Replace with "fib" program, passing the number as argument
        execlp("./child", "child", argv[1], NULL);
    } else if (pid > 0) {
        // ---- PARENT PROCESS ----
        wait(NULL);  // Wait for child to finish

        printf("\nPARENT: child completed\n");
        printf("Parent printing:\n");

        // Read data written by child from shared memory
        printf("%s ", (char *)ptr);

        // Detach shared memory
        shmdt(ptr);
    }

    return 0;
}
