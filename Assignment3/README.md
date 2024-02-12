# CS 3305 - Assignment 3: Multithreading Prime Number Summation

## Description

This program, created by Andrei Merkulov for the CS 3305 course, demonstrates the use of multithreading to efficiently calculate prime numbers and their summation. Given a maximum number and the number of threads to be utilized, it divides the task of finding prime numbers up to the maximum specified. Each thread calculates both the sum and count of primes within its range, with the main thread aggregating these results to display the total sum of primes and the total count of primes found.

## Features

- Utilizes multithreading to divide the task of finding prime numbers among multiple threads.
- Accepts two command-line arguments: the number of threads to use and the maximum number to check for primality.
- Each thread computes the sum of all primes and the count of primes up to the maximum number.
- Demonstrates synchronization and aggregation of results from multiple threads in the main thread.
## How to Compile and Run

- Ensure you have the GCC compiler and support for POSIX threads installed on your system.
- Compile the program with the following command:
- gcc assignment-3.c -o primeSummation -lpthread
- ./primeSummation <number_of_threads> <maximum_number>
