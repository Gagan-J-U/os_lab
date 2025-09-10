/*
Program: parent.c
Description: Demonstrates inter-process communication using shared memory.
The parent process creates a shared memory segment, takes input n, forks a child process,
then waits for the child to fill the first n odd numbers in the shared memory array.
After the child finishes, the parent prints the odd numbers and deletes the shared memory.

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
#include <sys/wait.h>
#include <unistd.h>

// Structure to hold n and an array of 100 integers
typedef struct {
    int n;
    int arr[100];
} pack_t;

int main() {
    key_t key = 1234; // Shared memory key
    // Create shared memory segment
    int shmid = shmget(key, sizeof(pack_t), 0666 | IPC_CREAT);
    // Attach shared memory segment
    pack_t* shm_ptr = (pack_t*) shmat(shmid, NULL, 0666);
    printf("[PARENT] Created shared memory with key = %d, id = %d, address = %p\n", key, shmid, shm_ptr);
    printf("[PARENT] Enter n: ");
    scanf("%d", &shm_ptr->n); // Store n in shared memory

    pid_t child_pid;
    // Fork a child process
    if ((child_pid = fork()) == 0) {
        // In child process: execute child program
        execlp("./child", "child", NULL); // Child fills arr with odd numbers
    } else {
        // In parent process: wait for child to finish
        wait(NULL);
        printf("[PARENT] Child finished executing.\n");
        printf("[PARENT] The first %d odd numbers are: \n", shm_ptr->n);
        // Print the odd numbers from shared memory
        for (int i = 0; i < shm_ptr->n; i++)
            printf("%d ", shm_ptr->arr[i]);
        putchar('\n');
    }
    // Detach shared memory
    shmdt((void*) shm_ptr);
    // Delete shared memory segment
    shmctl(shmid, IPC_RMID, NULL);
    printf("[PARENT] Shared Memory Deleted\n");
    return 0;
}