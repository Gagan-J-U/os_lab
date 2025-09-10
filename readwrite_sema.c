/* 
This program provides a possible solution for the first readers-writers problem 
using mutex and semaphore. There are 3 readers and 3 writers to demonstrate the solution.

Compile using: gcc filename.c -pthread
*/

#include <pthread.h>     // for pthread functions
#include <semaphore.h>   // for semaphores
#include <stdio.h>       // for input/output functions

sem_t wrt;               // Semaphore to control access for writers (and first reader)
pthread_mutex_t mutex;   // Mutex to synchronize access to numreader variable
int cnt = 1;             // Shared data being read/written
int numreader = 0;       // Counter to track how many readers are reading

// Writer thread function
void *writer(void *wno)
{   
    sem_wait(&wrt);  // Writer waits until it's safe to write (exclusive access)
                     // If already locked by a reader or another writer, it will block

    cnt = cnt * 2;   // Critical Section: modifying the shared data
    printf("Writer %d modified cnt to %d\n",(*((int *)wno)), cnt);

    sem_post(&wrt);  // Release exclusive access so others can proceed
                     // Wakes up a blocked reader or writer, if any
}

// Reader thread function
void *reader(void *rno)
{   
    // Lock mutex to safely update numreader counter
    pthread_mutex_lock(&mutex);
    numreader++;                    // Increment the number of active readers

    if(numreader == 1)              // If this is the first reader
    {
        sem_wait(&wrt);             // It blocks writers by locking the wrt semaphore
    }

    pthread_mutex_unlock(&mutex);  // Unlock the mutex so others can update numreader

    // Reading Section (simulated by printing shared data)
    printf("Reader %d: read cnt as %d\n", *((int *)rno), cnt);

    // Lock mutex before updating numreader again
    pthread_mutex_lock(&mutex);
    numreader--;                    // One reader is done

    if(numreader == 0)              // If this was the last reader
    {
        sem_post(&wrt);             // Allow writers to proceed
    }

    pthread_mutex_unlock(&mutex);  // Unlock the mutex
}

int main()
{   
    pthread_t read[3], write[3];             // Arrays for thread IDs for 3 readers and 3 writers

    pthread_mutex_init(&mutex, NULL);        // Initialize the mutex with default attributes

    // Initialize the semaphore 'wrt'
    // 0 → the semaphore is local to this process
    // 1 → initial value is 1 (so one writer or all readers can enter)
    sem_init(&wrt, 0, 1);                    

    int a[3] = {1, 2, 3};    // Helper array to pass different IDs to reader/writer threads

    // Create 3 reader threads
    for(int i = 0; i < 3; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&a[i]);
        pthread_create(&write[i], NULL, (void *)writer, (void *)&a[i]);
        // Arguments: thread_id, attributes (NULL=default), function, argument to function
    }

    

    // Wait for all reader threads to finish
    for(int i = 0; i < 3; i++) {
        pthread_join(read[i], NULL);
        pthread_join(write[i], NULL);
    }


    // Clean up and destroy mutex and semaphore
    pthread_mutex_destroy(&mutex);  // Destroys the mutex; should be done after all threads are done
    sem_destroy(&wrt);              // Destroys the semaphore; no thread should be waiting on it

    return 0;  // Exit program
}