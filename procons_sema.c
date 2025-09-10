#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_ITEMS 3     // Max items a producer produces / consumer consumes
#define BUFFER_SIZE 5   // Size of buffer

int buffer[BUFFER_SIZE];
int in = 0;             // Index for next produced item
int out = 0;            // Index for next consumed item

sem_t empty;            // Counts empty slots in buffer
sem_t full;             // Counts full slots in buffer
pthread_mutex_t mutex;  // Mutex for critical section

void *producer(void *pno) {
    int id = *((int *)pno);
    for (int i = 0; i < MAX_ITEMS; i++) {
        int item = rand() % 100; // Produce a random item

        sem_wait(&empty);       // Wait if buffer is full
        pthread_mutex_lock(&mutex); // Enter critical section

        buffer[in] = item;
        printf("Producer %d produced %d at index %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&full);              // Signal that buffer has a new full slot
    }
    return NULL;
}

void *consumer(void *cno) {
    int id = *((int *)cno);
    for (int i = 0; i < MAX_ITEMS; i++) {

        sem_wait(&full);           // Wait if buffer is empty
        pthread_mutex_lock(&mutex);   // Enter critical section

        int item = buffer[out];
        printf("Consumer %d consumed %d at index %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;

        pthread_mutex_unlock(&mutex); // Exit critical section
        sem_post(&empty);             // Signal that buffer has an empty slot
    }
    return NULL;
}

int main() {
    pthread_t prod[3], cons[3];
    int p_id[3], c_id[3];

    // Initialize semaphores and mutex
    sem_init(&empty, 0, BUFFER_SIZE); // Initially all slots empty
    sem_init(&full, 0, 0);            // Initially no full slots
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    for (int i = 0; i < 3; i++) {
        p_id[i] = i+1;
        c_id[i] = i+1;
        pthread_create(&prod[i], NULL, producer, &p_id[i]);
        pthread_create(&cons[i], NULL, consumer, &c_id[i]);
    }

    // Join threads
    for (int i = 0; i < 3; i++) {
        pthread_join(prod[i], NULL);
        pthread_join(cons[i], NULL);
    }

    // Cleanup
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
