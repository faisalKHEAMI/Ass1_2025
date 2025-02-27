#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Fibonacci function
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process (PID: %d)\n", getpid());
        for (int i = 1; i <= 3; i++) {
            printf("Child: fib(%d) = %lld\n", 40 + i, fib(40 + i));
        }
    } else {
        // Parent process
        printf("Parent process (PID: %d)\n", getpid());
        for (int i = 4; i <= 6; i++) {
            printf("Parent: fib(%d) = %lld\n", 40 + i, fib(40 + i));
        }
        wait(NULL);  // Wait for the child to finish
    }

    return 0;
}
