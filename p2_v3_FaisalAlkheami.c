#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Fibonacci function
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// Thread function
void* compute_fib(void* arg) {
    int start = *(int*)arg;
    for (int i = start; i < start + 3; i++) {
        printf("Thread %d: fib(%d) = %lld\n", start / 3 + 1, 40 + i, fib(40 + i));
    }
    return NULL;
}

int main() {
    pthread_t threads[2];
    int ranges[2] = {1, 4};  // Thread 1 computes fib(41-43), Thread 2 computes fib(44-46)

    for (int i = 0; i < 2; i++) {
        if (pthread_create(&threads[i], NULL, compute_fib, &ranges[i]) != 0) {
            perror("Failed to create thread");
            exit(1);
        }
    }

    for (int i = 0; i < 2; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
