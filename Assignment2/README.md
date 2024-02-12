# CS 3305 - Assignment 2: Multiprocessing Integer Product

## Description

This program, authored by Andrei Merkulov for the CS 3305 Operating Systems course, demonstrates an innovative approach to calculating the product of two integers. By partitioning each integer into two parts (a1, a2 for the first integer and b1, b2 for the second), the program distributes the multiplication workload across four processes. Each child process is responsible for calculating a part of the product, which are then aggregated by the parent process to produce the final result. This approach exemplifies multiprocessing and inter-process communication in C.

## Features

- Takes two integers as input from the user.
- Partitions the integers into parts and calculates their products in parallel using child processes.
- Demonstrates the use of fork() for process creation and IPC (Inter-Process Communication) for result aggregation.
- Outputs the final product of the two integers after combining the results from each child process.
## How to Compile and Run

- Ensure you have the GCC compiler installed.
- Compile the program with the following command:
- gcc assignment-2.c -o integerProduct
- ./integerProduct
