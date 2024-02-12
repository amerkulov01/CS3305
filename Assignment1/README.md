# CS 3305 - Assignment 1: Password Decoder

## Description

This program, written by Andrei Merkulov for the CS 3305 course, is designed to decode passwords through a systematic approach. It iterates over possible ASCII character combinations to find the correct password segments. The program showcases techniques in string manipulation, process creation with fork(), and inter-process communication to efficiently decode the password.

## Features

- Iterates through ASCII character combinations from ASCII_MIN (33) to ASCII_MAX (126).
- Uses fork() to create child processes for parallel password checking.
- Includes a custom function checkPassword to verify password segments.
- Demonstrates process synchronization and data sharing among processes.

## How to Compile and Run

- Ensure you have the GCC compiler and any required libraries installed. Compile the program with the following command:
- gcc assignment-1.c -o passwordDecoder
- ./passwordDecoder

