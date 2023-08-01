

cc -o multi_cpu_example multi_cpu_example.c -lpthread
./multi_cpu_example

  

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4
#define ARRAY_SIZE 1000000

int array[ARRAY_SIZE];
long sum = 0;

void *compute_sum(void *thread_id) {
    long tid = (long)thread_id;
    long start = tid * (ARRAY_SIZE / NUM_THREADS);
    long end = start + (ARRAY_SIZE / NUM_THREADS);

    // Each thread computes a partial sum
    long partial_sum = 0;
    for (long i = start; i < end; i++) {
        partial_sum += array[i];
    }

    // Update the global sum using a lock to avoid race condition
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    sum += partial_sum;
    pthread_mutex_unlock(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;

    // Initialize the array with values from 1 to ARRAY_SIZE
    for (long i = 0; i < ARRAY_SIZE; i++) {
        array[i] = i + 1;
    }

    // Create threads and compute the sum in parallel
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, compute_sum, (void *)t);
        if (rc) {
            printf("ERROR: return code from pthread_create() is %d\n", rc);
            return 1;
        }
    }

    // Wait for all threads to finish
    for (t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("The sum of the array elements is: %ld\n", sum);
    pthread_exit(NULL);
}
