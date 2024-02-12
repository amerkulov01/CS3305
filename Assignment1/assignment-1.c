/*
Andrei Merkulov - 251145994
CS 3305
password decoder - Assignment 1
2023/01/31
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "checkPassword.h"

#define LEN 12
#define PART_LEN 3
#define ASCII_MIN 33
#define ASCII_MAX 126
#define CHECK_PART(part) checkPassword(part, i * PART_LEN)
#define PRINT_PART(part) for (int j = 0; j < PART_LEN; j++) printf("%c", part[j]);

void find_password_seq(void);
void find_password_fork(int is_run_hackme);

int main(int argc, char *argv[]) { // main function to run the program and check for arguments -f and -p
  int is_fork = 0, is_run_hackme = 0;
  char opt;

  while ((opt = getopt(argc, argv, "fp")) != -1) {
    switch (opt) {
      case 'f':
        is_fork = 1;
        break;
      case 'p':
        is_run_hackme = 1;
        break;
      default:
        fprintf(stderr, "Usage: %s [-f] [-p]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  if (is_fork)
    find_password_fork(is_run_hackme);
  else
    find_password_seq();

  return 0;
}

void find_password_seq(void) {  // function to find the password sequentially 
  char part[PART_LEN + 1];

  for (int i = 0; i < LEN / PART_LEN; i++) {    // for loop to iterate through the password
    for (int c1 = ASCII_MIN; c1 <= ASCII_MAX; c1++) {   // for loop to iterate through the ASCII characters
      for (int c2 = ASCII_MIN; c2 <= ASCII_MAX; c2++) { 
        for (int c3 = ASCII_MIN; c3 <= ASCII_MAX; c3++) {
          part[0] = c1;  // set the first character of the password
          part[1] = c2; // set the second character of the password
          part[2] = c3; // set the third character of the password
          part[3] = '\0';   // set the end of the password

          if (CHECK_PART(part) == 0) {  // check if the password is correct
            printf("Part %d: ", i + 1);
            PRINT_PART(part);
            printf("\n");
            break;
          }
        }
      }
    }
  }
}

void find_password_fork(int is_run_hackme) {    // function to find the password using fork
  char part[PART_LEN + 1];
  pid_t pid;
  int status;

  for (int i = 0; i < LEN / PART_LEN; i++) {    // for loop to iterate through the password
    for (int c1 = ASCII_MIN; c1 <= ASCII_MAX; c1++) {   // for loop to iterate through the ASCII characters
      for (int c2 = ASCII_MIN; c2 <= ASCII_MAX; c2++) { 
        for (int c3 = ASCII_MIN; c3 <= ASCII_MAX; c3++) {
          part[0] = c1;
          part[1] = c2;
          part[2] = c3;
          part[3] = '\0';

          pid = fork();  // fork the process
          if (pid == 0) {          
            // Child process
            if (CHECK_PART(part) == 0) {
              if (is_run_hackme) {  // run the hackme program if the password is correct
                execlp("./hackme", "", NULL);
                perror("execlp");
                exit(EXIT_FAILURE); 
              } else {  // print the password if the password is correct
                printf("Part %d: ", i + 1);
                PRINT_PART(part);
                printf("\n");
                exit(EXIT_SUCCESS);
              }
            } else {
              exit(EXIT_FAILURE);
            }
          } else if (pid > 0) { 
            // Parent process
            wait(&status);  // wait for the child process to finish
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0)  // check if the child process exited successfully
              break;
          } else {
            // Fork error
            perror("fork");
            exit(EXIT_FAILURE);
          }
        }
      }
    }
  }
}

        
