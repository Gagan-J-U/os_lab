/*
thread.c

This program demonstrates creating two threads: one to calculate the sum from 1 to n, and another to calculate the factorial of n.

How to run:
1. Compile the program (add -pthread for POSIX threads):
   cc thread.c -o thread -pthread
2. Run the program with an integer argument (e.g., 5):
   ./thread 5

Note: Run on Linux or WSL. The argument should be a positive integer.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Global variables to store sum and product (factorial)
int sum = 0, prod = 1;

// Thread function to calculate the sum from 1 to n
void *sum_thread(void *arg) {
    int n, i;
    n = atoi(arg);  // Convert argument to integer

    printf("Inside sum thread\n");

    for (i = 1; i <= n; i++) {
        sum += i;
    }

    printf("Sum thread completed\n");
    pthread_exit(0);
}

// Thread function to calculate factorial of n
void *mul_thread(void *arg) {
    int n, i;
    n = atoi(arg);  // Convert argument to integer

    printf("Inside mul thread\n");

    for (i = 2; i <= n; i++) {
        prod *= i;
    }

    printf("Mul thread completed (product)\n");
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    pthread_t T1, T2;         // Thread identifiers
    

    // Create thread T1 to calculate sum
    pthread_create(&T1, NULL, sum_thread, argv[1]);

    // Create thread T2 to calculate factorial
    pthread_create(&T2, NULL, mul_thread, argv[1]);

    // Wait for both threads to finish
    pthread_join(T1, NULL);
    pthread_join(T2, NULL);

    // Main thread prints results after both threads finish
    printf("Inside main thread\n");
    printf("Sum = %d\n", sum);
    printf("Factorial = %d\n", prod);

    return 0;
}
