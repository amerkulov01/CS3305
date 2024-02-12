/*
* Andrei Merkulov, 2551145994
* This program takes two command line arguments: the number of threads to use and the maximum number to check for primality.
* It then creates the specified number of threads and each thread finds the sum of all primes from 1 to the maximum number
* and the total number of primes from 1 to the maximum number. The main thread then prints the sum of all the sums and the
* sum of all the counts.
* CS 3305, Assignment 3
* 2023 - 03 - 07
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int max_num, num_threads;   // max_num is the maximum number to check for primality
                            // num_threads is the number of threads to use
int prime_count = 0;    // total number of primes found
long long prime_sum = 0;    // sum of all primes found

void *count_primes(void *arg) { // arg is the thread id
    int id = *(int*)arg;    // id is the thread id
    int low = id * (max_num / num_threads); // low is the lower bound of the range of numbers to check for primality
    int high = (id == num_threads - 1) ? max_num : ((id + 1) * (max_num / num_threads) - 1);    // high is the upper bound of the range of numbers to check for primality

    int count = 0;  
    long long sum = 0;
    for (int i = low; i <= high; i++) { // check for primality of all numbers in the range
        int is_prime = 1;   // is_prime is 1 if the number is prime, 0 otherwise
        for (int j = 2; j < i; j++) {       // check if the number is prime
            if (i % j == 0) {   // if the number is divisible by any number other than 1 and itself, it is not prime
                is_prime = 0;
                break;
            }
        }
        if (is_prime) { // if the number is prime, increment the count and add it to the sum
            count++;
            sum += i;
        }
    }

    printf("Sum of thread %d is %lld, Count is %d\n", id, sum, count);  

    prime_count += count;
    prime_sum += sum;

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {  // argv[1] is the number of threads, argv[2] is the maximum number to check for primality
    if (argc != 3) {
        printf("Usage: %s <num_threads> <max_num>\n", argv[0]); // print usage if the number of arguments is not 3
        return 1;
    }

    num_threads = atoi(argv[1]);    
    max_num = atoi(argv[2]);

    pthread_t threads[num_threads]; // array of threads
    int thread_ids[num_threads];    // array of thread ids

    for (int i = 0; i < num_threads; i++) { // create threads
        thread_ids[i] = i;  
        printf("Thread # %d is finding primes from low = %d to high = %d\n", i, i * (max_num / num_threads), (i == num_threads - 1) ? max_num : ((i + 1) * (max_num / num_threads) - 1));   
        pthread_create(&threads[i], NULL, count_primes, &thread_ids[i]);    
    }

    for (int i = 0; i < num_threads; i++) { // wait for threads to finish
        pthread_join(threads[i], NULL);     // wait for thread i to finish
    }

    printf("\nGRAND SUM IS %lld, COUNT IS %d\n", prime_sum, prime_count);

    return 0;
}