/* Andrei Merkulov, 251145994
*  CS 3305 Operating Systems
*  Assignment 2
*  This program takes two integers as input and calculates the product of the two integers using 4 processes.
*  The program will partition the two integers into 4 parts, a1, a2, b1, b2, and send each part to a child process.
*  The child process will then multiply the two parts and send the result to the parent process.
*  The parent process will then add the results together and print the final result.
*  2023 - 02 - 14
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);

    // Partition the numbers into a1, a2, b1, b2
    int n = 10000;
    int a1 = a / (n/100);
    int a2 = a % (n/100);
    int b1 = b / (n/100);
    int b2 = b % (n/100);

    printf("Your integers are %d %d\n", a, b);
    printf("Parent (PID %d): created child\n\n", getpid());

    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0 || pipe(pipe3) < 0 || pipe(pipe4) < 0) {
        perror("pipe error");
        return 1;
    }

    // Fork a child process
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork error");
        return 1;
    } else if (pid == 0) {
        // Child process
        int A, B, C, D;

        read(pipe1[0], &a1, sizeof(a1));    // Read a1 from the parent
        read(pipe1[0], &b1, sizeof(b1));    // Read b1 from the parent
        A = a1 * b1;

        printf("\n###\n# Calculating X\n###\n");
        printf("        Child (PID %d): Received %d from parent\n", getpid(), a1);
        printf("        Child (PID %d): Received %d from parent\n", getpid(), b1);
        printf("        Child (PID %d): Sending %d to parent\n", getpid(), A);
        write(pipe2[1], &A, sizeof(A));

        // Multiply a2 and b1 and send B to the parent
        read(pipe1[0], &a2, sizeof(a2));
        read(pipe1[0], &b1, sizeof(b1));
        B = a2 * b1;

        printf("\n###\n# Calculating Y\n###\n");
        printf("        Child (PID %d): Received %d from parent\n", getpid(), a2);
        printf("        Child (PID %d): Received %d from parent\n", getpid(), b1);
        printf("        Child (PID %d): Sending %d to parent\n", getpid(), B);
        write(pipe3[1], &B, sizeof(B));

        // Multiply a1 and b2 and send C to the parent
        read(pipe1[0], &a1, sizeof(a1));
        read(pipe1[0], &b2, sizeof(b2));
        C = B+(a1 * b2);    // Add B to C
        printf("        Child (PID %d): Received %d from parent\n", getpid(), a1);
        printf("        Child (PID %d): Received %d from parent\n", getpid(), b2);
        printf("        Child (PID %d): Sending %d to parent\n", getpid(), C);
        write(pipe3[1], &C, sizeof(C));

        // Multiply a2 and b2 and send D to the parent
                read(pipe1[0], &a2, sizeof(a2));
        read(pipe1[0], &b2, sizeof(b2));
        D = a2 * b2;

        printf("\n###\n# Calculating Z\n###\n");
        printf("        Child (PID %d): Received %d from parent\n", getpid(), a2);
        printf("        Child (PID %d): Received %d from parent\n", getpid(), b2);
        printf("        Child (PID %d): Sending %d to parent\n", getpid(), D);
        write(pipe4[1], &D, sizeof(D));

        exit(0);
    }
    else {
        // parent process
        int X, Y, Z;

        // Send a1 and b1 to the child
        write(pipe1[1], &a1, sizeof(a1));
        write(pipe1[1], &b1, sizeof(b1));
        printf("Parent (PID %d): Sent %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sent %d to child\n", getpid(), b1);
        read(pipe2[0], &X, sizeof(X));
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), X);

        // Send a2 and b1 to the child
        write(pipe1[1], &a2, sizeof(a2));
        write(pipe1[1], &b1, sizeof(b1));
        printf("Parent (PID %d): Sent %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sent %d to child\n", getpid(), b1);
        read(pipe3[0], &Y, sizeof(Y));
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), Y);

        // Send a1 and b2 to the child
        write(pipe1[1], &a1, sizeof(a1));
        write(pipe1[1], &b2, sizeof(b2));
        printf("Parent (PID %d): Sent %d to child\n", getpid(), a1);
        printf("Parent (PID %d): Sent %d to child\n", getpid(), b2);
        read(pipe3[0], &Y, sizeof(Y));
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), Y);

        // Send a2 and b2 to the child
        write(pipe1[1], &a2, sizeof(a2));
        write(pipe1[1], &b2, sizeof(b2));
        printf("Parent (PID %d): Sent %d to child\n", getpid(), a2);
        printf("Parent (PID %d): Sent %d to child\n", getpid(), b2);
        read(pipe4[0], &Z, sizeof(Z));
        printf("Parent (PID %d): Received %d from child\n\n", getpid(), Z);

        int result = (X * 10000) + (Y * 100) + Z;   // Calculate the result
        printf("The result is: %d\n", result);    // Print the result
    }

    return 0;
}
