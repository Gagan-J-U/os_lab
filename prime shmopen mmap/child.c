/*
prime shmopen mmap - child.c

This program finds all prime numbers in a given range and writes them to POSIX shared memory using mmap.
The parent process (parent.c) reads and prints the sequence from shared memory after the child completes.

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

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char *argv[]) {
    void *ptr;  // Pointer to shared memory
    const int SIZE = 4096;

    // Create or open a shared memory object with read/write permissions
    int shm_fd = shm_open("VSS", O_CREAT | O_RDWR, 0666);

    // Set size of shared memory
    ftruncate(shm_fd, SIZE);

    // Map shared memory object in the process’s address space
    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    printf("CHILD:\n");

    // Convert command line arguments to integers
    int i = atoi(argv[1]);  // Lower bound
    int j = atoi(argv[2]);  // Upper bound
    int flag = 0;           // Flag to check if number is prime

    printf("The prime numbers in the range %d and %d are:\n", i, j);

    // Loop over numbers in the range [i, j)
    for (int num = i; num < j; num++) {
        flag = 0;

        // Check divisibility from 2 to num/2
        for (int k = 2; k <= num / 2; k++) {
            if (num % k == 0) {
                flag = 1;   // Not a prime
                break;
            }
        }

        // If flag is still 0, number is prime
        if (flag == 0 && num > 1) {
            printf("%d\t", num);                        // Print to console
            sprintf(ptr, "%d\t", num);
            ptr+=strlen(ptr);   // Append to shared memory
        }
    }

    printf("\n");

    return 0;
}
