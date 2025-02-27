#include <stdio.h>
#include <pthread.h>

#define NUM_TASKS 10

int results[NUM_TASKS];

// Fibonacci function
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// Thread function
void* compute_fib(void* arg) {
    int task_id = *(int*)arg;
    results[task_id] = fib(10 + task_id);
    return NULL;
}

int main() {
    pthread_t threads[NUM_TASKS];
    int task_ids[NUM_TASKS];

    // Create threads for each task
    for (int i = 0; i < NUM_TASKS; i++) {
        task_ids[i] = i;
        if (pthread_create(&threads[i], NULL, compute_fib, &task_ids[i]) != 0) {
            perror("Failed to create thread");
            return 1;
        }
    }

    // Wait for all threads to complete
    for (int i = 0; i < NUM_TASKS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print results in order
    for (int i = 0; i < NUM_TASKS; i++) {
        printf("Task %d: fib(%d) = %d\n", i + 1, 10 + i, results[i]);
    }

    return 0;
}
