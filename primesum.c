/*  Pat Eizenga
 *  COSC3355.001
 *  4/11/2025
 *  Purpose: To calculate a sum/series/sequence of prime numbers up to a specific integer input 
 *      using console commands in linux. This program must utualize pthreads and a consumer
 *       function in order to accomplish this. Initializers, synchronize API calls, semaphores and
 *        making sure not to double count the sentinel values were all needed to debug
 */
// Sources: man pages for pthreads and semaphores (used to learn pthreads/synchronization API calls)
// Three Easy Pieces chapters

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>

#define MAX_THREADS 5  // Number of producer threads
#define SLOT_EMPTY 0   // Value representing an empty slot in shared array

// Shared variables
int count = 3;                       // Start at 3 and increment by 2 to get successive odd numbers
int upper_limit;                    // Upper limit of prime check (from user input)
int prime_array[MAX_THREADS] = {0}; // Shared array where each producer thread stores discovered prime
int done = 0;                        // Flag to signal all producers are done
int sum = 2;                         // Final sum of primes (initialized with 2 since we skip even numbers)

// Synchronization tools
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;      // For protecting access to `count`
pthread_mutex_t array_mutex = PTHREAD_MUTEX_INITIALIZER;      // For protecting access to `prime_array`
pthread_cond_t array_not_empty = PTHREAD_COND_INITIALIZER;    // Consumer waits on this if no primes
pthread_cond_t array_not_full[MAX_THREADS];                   // One condition per producer (used when their slot is full)

// Utility: Check if a number is prime
int is_prime(int n) {
    if (n <= 1) return 0;
    if (n == 2) return 1;
    if (n % 2 == 0) return 0;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return 0;
    }
    return 1;
}

// Producer thread function
void* producer(void* arg) {
    int id = *(int*)arg;
    free(arg);  // Free the dynamically allocated thread ID

    while (1) {
        int candidate;

        // Get the next odd number to check for primality
        pthread_mutex_lock(&count_mutex);
        candidate = count;
        count += 2;
        pthread_mutex_unlock(&count_mutex);

        // Exit condition: if the candidate exceeds the upper limit
        if (candidate > upper_limit) {
            break;
        }

        // Test if the candidate is prime
        if (is_prime(candidate)) {
            pthread_mutex_lock(&array_mutex);

            // If slot is not empty, wait until the consumer clears it
            while (prime_array[id] != SLOT_EMPTY) {
                pthread_cond_wait(&array_not_full[id], &array_mutex);
            }

            // Store the prime number in the producer's assigned slot
            prime_array[id] = candidate;

            // Signal the consumer that a new prime is available
            pthread_cond_signal(&array_not_empty);
            pthread_mutex_unlock(&array_mutex);
        }
    }

    // Mark this producer as done by placing -1 in their slot
    pthread_mutex_lock(&array_mutex);
    prime_array[id] = -1;  // Sentinel value indicating producer is done
    pthread_cond_signal(&array_not_empty);  // Wake consumer to check for done signal
    pthread_mutex_unlock(&array_mutex);

    return NULL;
}

// Consumer thread function
void* consumer(void* arg) {
    int producers_finished = 0;
    int finished[MAX_THREADS] = {0};  // Track if a producer is already counted as done

    while (1) {
        pthread_mutex_lock(&array_mutex);

        // Wait until there's at least one non-zero slot (or a done signal)
        int has_data = 0;
        for (int i = 0; i < MAX_THREADS; i++) {
            if (prime_array[i] != SLOT_EMPTY) {
                has_data = 1;
                break;
            }
        }

        if (!has_data) {
            pthread_cond_wait(&array_not_empty, &array_mutex);
        }

        // Process primes from all slots
        for (int i = 0; i < MAX_THREADS; i++) {
            if (prime_array[i] > 0) {
                sum += prime_array[i];
                prime_array[i] = SLOT_EMPTY;
                pthread_cond_signal(&array_not_full[i]);  // Let the producer know the slot is free
            } else if (prime_array[i] == -1 && !finished[i]) {
                finished[i] = 1;
                prime_array[i] = SLOT_EMPTY;
                producers_finished++;
            }
        }

        pthread_mutex_unlock(&array_mutex);

        // If all producers are done, exit the loop
        if (producers_finished == MAX_THREADS) {
            break;
        }
    }

    return NULL;
}

// Main function
int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <positive_integer>\n", argv[0]);
        return EXIT_FAILURE;
    }

    upper_limit = atoi(argv[1]);
    if (upper_limit < 2) {
        printf("Sum of primes: 0\n");
        return EXIT_SUCCESS;
    }

    // Initialize condition variables for each producer
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_cond_init(&array_not_full[i], NULL);
    }

    pthread_t producers[MAX_THREADS];
    pthread_t consumer_thread;

    // Create consumer thread
    if (pthread_create(&consumer_thread, NULL, consumer, NULL) != 0) {
        perror("Failed to create consumer thread");
        exit(EXIT_FAILURE);
    }

    // Create producer threads
    for (int i = 0; i < MAX_THREADS; i++) {
        int* id = malloc(sizeof(int));
        if (id == NULL) {
            perror("Failed to allocate thread ID");
            exit(EXIT_FAILURE);
        }
        *id = i;
        if (pthread_create(&producers[i], NULL, producer, id) != 0) {
            perror("Failed to create producer thread");
            exit(EXIT_FAILURE);
        }
    }

    // Wait for consumer to finish
    if (pthread_join(consumer_thread, NULL) != 0) {
        perror("Failed to join consumer thread");
        exit(EXIT_FAILURE);
    }

    // Print the final result
    printf("Sum of primes: %d\n", sum);

    // Clean up
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_cond_destroy(&array_not_full[i]);
    }
    pthread_mutex_destroy(&count_mutex);
    pthread_mutex_destroy(&array_mutex);
    pthread_cond_destroy(&array_not_empty);

    return EXIT_SUCCESS;
}
