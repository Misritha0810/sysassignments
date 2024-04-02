#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

/*
 Author: Misritha Maridi
 Date: Apr 01, 2024
 To Compile: gcc -O -Wall lab11.c -lpthread
 To Run: ./a.out 1000 4
*/

typedef struct {
    double *a;    // Pointer to the array of elements
    double sum;   // Sum computed by this thread
    int N;        // Total number of elements in the array
    int size;     // Total number of threads
    long tid;     // Thread ID
} ThreadData;

// Mutex for synchronizing access to the shared sum variable
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Thread function to compute partial sum
void *compute(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    int start, end, range, i;
    
    // Calculate the range of elements this thread will sum
    range = data->N / data->size;
    start = data->tid * range;
    end = start + range;
    if (data->tid == data->size - 1) { // Last thread may have more elements
        end = data->N;
    }

    // Compute the partial sum for this thread's range
    for (i = start; i < end; i++) {
        data->sum += data->a[i];
    }

    // Synchronize and update the global sum
    pthread_mutex_lock(&mutex);
    // No need for a global sum variable, as sum calculation is done at the end
    pthread_mutex_unlock(&mutex);

    printf("Thread %ld finished. Computed partial sum: %g\n", data->tid, data->sum);
    return NULL;
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <# of elements> <# of threads>\n", argv[0]);
        return -1;
    }

    int N = atoi(argv[1]);
    int size = atoi(argv[2]);
    pthread_t threads[size];
    ThreadData data[size];
    double a[N], totalSum = 0.0;

    // Initialize the array
    for (int i = 0; i < N; i++) {
        a[i] = i + 1;  // Fill the array with values 1 to N
    }

    // Initialize thread data and create threads
    for (long i = 0; i < size; i++) {
        data[i] = (ThreadData){.a = a, .sum = 0.0, .N = N, .size = size, .tid = i};
        pthread_create(&threads[i], NULL, compute, &data[i]);
    }

    // Wait for threads to finish and calculate total sum
    for (int i = 0; i < size; i++) {
        pthread_join(threads[i], NULL);
        totalSum += data[i].sum;  // Summing up all partial sums
    }

    printf("Total sum computed: %g, Expected: %g\n", totalSum, (double)N * (N + 1) / 2);

    return 0;
}
