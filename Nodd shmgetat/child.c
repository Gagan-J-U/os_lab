/*
Program: child.c
Description: Attaches to the shared memory created by the parent process,
generates the first n odd numbers, and stores them in the shared memory array.

How to run:
1. Compile both parent.c and child.c:
   gcc parent.c -o parent
   gcc child.c -o child
2. Run the parent program:
   ./parent
   (The parent will fork and run the child automatically)
*/

#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>

// Structure to hold n and an array of 100 integers
typedef struct {
    int n;
    int arr[100];
} pack_t;

int main() {
    key_t key = 1234; // Shared memory key (must match parent)
    // Get shared memory segment
    int shmid = shmget(key, sizeof(pack_t), 0666);
    // Attach shared memory segment
    pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0666);
    printf("[CHILD] Attached to shared memory with key = %d, id = %d. address = %p\n", key, shmid, shm_ptr);
    // Generate first n odd numbers and store in shared memory
    for (int i = 0; i < shm_ptr->n; i++)
        shm_ptr->arr[i] = 2*i + 1;
    printf("[CHILD] Finished generating first %d odd numbers\n", shm_ptr->n);
    // Detach shared memory
    shmdt((void*) shm_ptr);
    return 0;
}