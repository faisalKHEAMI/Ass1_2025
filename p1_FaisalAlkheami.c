#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>

// Function for threads to print their IDs
void* print_thread_info(void* arg) {
    printf("Process ID: %d, Thread ID: %lu\n", getpid(), pthread_self());
    return NULL;
}

int main() {
    pthread_t parent_thread;
    
    // Create a thread in the parent process
    if (pthread_create(&parent_thread, NULL, print_thread_info, NULL) != 0) {
        perror("Failed to create parent thread");
        exit(1);
    }
    
    // Wait for the parent thread to finish
    pthread_join(parent_thread, NULL);

    // Fork to create a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child process created. Process ID: %d\n", getpid());
        
        pthread_t child_threads[2];
        for (int i = 0; i < 2; i++) {
            if (pthread_create(&child_threads[i], NULL, print_thread_info, NULL) != 0) {
                perror("Failed to create child thread");
                exit(1);
            }
        }

        // Wait for child threads to finish
        for (int i = 0; i < 2; i++) {
            pthread_join(child_threads[i], NULL);
        }

        printf("Child process finished.\n");
        exit(0);
    } else {
        // Parent process
        printf("Parent process. Process ID: %d\n", getpid());
        
        // Variant 1: Wait for the child process to finish
        // Uncomment the following line for Variant 1
        // wait(NULL);

        printf("Parent process finished.\n");
    }

    return 0;
}
