#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

// Fibonacci function
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

// Thread function to compute Fibonacci numbers
void* compute_fib(void* arg) {
    int start = *(int*)arg;
    static char result[256]; // Buffer for thread results

    for (int i = start; i < start + 2; i++) {
        char buffer[64];
        sprintf(buffer, "fib(%d) = %lld\n", 40 + i, fib(40 + i));
        strcat(result, buffer);
    }

    return result;  // Return the results to be written to the pipe
}

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        close(pipefd[0]);  // Close reading end of the pipe

        pthread_t threads[2];
        int ranges[2] = {1, 3};  // Thread 1 computes fib(41-42), Thread 2 computes fib(43-44)

        for (int i = 0; i < 2; i++) {
            void* thread_result;
            if (pthread_create(&threads[i], NULL, compute_fib, &ranges[i]) != 0) {
                perror("Failed to create thread");
                exit(1);
            }
            pthread_join(threads[i], &thread_result);

            // Write thread results to the pipe
            write(pipefd[1], thread_result, strlen((char*)thread_result));
        }

        close(pipefd[1]);  // Close writing end of the pipe
        printf("Child process (PID: %d) finished.\n", getpid());
    } else {
        // Parent process
        close(pipefd[1]);  // Close writing end of the pipe

        pthread_t threads[2];
        int ranges[2] = {5, 7};  // Thread 1 computes fib(45-46), Thread 2 computes fib(47-48)

        for (int i = 0; i < 2; i++) {
            void* thread_result;
            if (pthread_create(&threads[i], NULL, compute_fib, &ranges[i]) != 0) {
                perror("Failed to create thread");
                exit(1);
            }
            pthread_join(threads[i], &thread_result);

            // Write thread results to stdout directly
            printf("Parent process (PID: %d) output:\n%s", getpid(), (char*)thread_result);
        }

        char buffer[256];
        int n;
        printf("\n--- Results from Child Process ---\n");
        while ((n = read(pipefd[0], buffer, sizeof(buffer) - 1)) > 0) {
            buffer[n] = '\0';  // Null-terminate the string
            printf("%s", buffer);
        }

        close(pipefd[0]);  // Close reading end of the pipe
        wait(NULL);        // Wait for the child to finish
        printf("Parent process (PID: %d) finished.\n", getpid());
    }

    return 0;
}
