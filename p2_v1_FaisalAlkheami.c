#include <stdio.h>

// Fibonacci function
long long fib(int n) {
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}

int main() {
    for (int i = 0; i < 6; i++) {
        printf("fib(42) = %lld\n", fib(42));
    }
    return 0;
}
