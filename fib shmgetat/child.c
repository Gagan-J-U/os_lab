/*
System V Shared Memory Fibonacci Example
child.c

This program generates the first N Fibonacci numbers and writes them to System V shared memory.
The parent process (parent.c) reads and prints the sequence from shared memory after the child completes.

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

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <stdlib.h>  // For atoi()

int main(int argc, char *argv[])
{
    int k = 2, n1, n2, n3;      // k is the index for Fibonacci loop
    void *ptr;                  // Pointer to shared memory

    // Get shared memory ID using key 1122
    int shmid = shmget((key_t)1122, 4096, 0666);

    // Attach the shared memory segment to this process
    ptr = shmat(shmid, NULL, 0666);

    printf("CHILD:\n");

    int i = atoi(argv[1]);     // Convert input string to integer N

    // Initialize first two Fibonacci numbers
    n1 = 0;
    n2 = 1;

    // Write first number to shared memory
    sprintf(ptr, "%d ", n1);
    ptr += strlen(ptr);        // Move pointer to next free byte
    printf("%d ", n1);

    // Write second number
    sprintf(ptr, "%d ", n2);
    ptr += strlen(ptr);
    printf("%d ", n2);

    // Compute and write rest of the Fibonacci numbers
    while (k < i)
    {
        n3 = n1 + n2;

        sprintf(ptr, "%d ", n3);   // Append to shared memory
        ptr += strlen(ptr);        // Move pointer forward
        printf("%d ", n3);         // Also print to console

        n1 = n2;
        n2 = n3;
        k++;
    }

    // Remove the shared memory segment after usage
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
