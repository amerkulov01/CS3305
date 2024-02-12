/*
* Andrei Merkulov, 251145994
* This program simulates the execution of a set of processes using three different scheduling algorithms:
* First Come First Served, Shortest Job First, and Round Robin.
* The program reads the processes from a file and displays the wait and turnaround times for each process
* as well as the average wait and turnaround times for all processes.
* CS 3305 Assignment 4
* 2023 - 03 - 21
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {    // Struct to hold process information
    char id[10];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int wait_time;
    int turnaround_time;
} Process;

void read_input_file(const char *filename, Process **processes, int *process_count) {   // Read processes from the input file
    FILE *file = fopen(filename, "r");
    if (file == NULL) { // Check if the file was opened successfully
        printf("Error opening file.\n");
        exit(1);
    }

    // Count lines in the file
    int ch;
    int lines = 0;
    while ((ch = fgetc(file)) != EOF) { // Count the number of lines in the file
        if (ch == '\n') {
            lines++;
        }
    }

    // Allocate memory for processes
    *processes = (Process *) malloc(lines * sizeof(Process));
    *process_count = lines;

    // Read processes from the file
    rewind(file);
    for (int i = 0; i < lines; i++) {   
        fscanf(file, "%9[^,],%d,%d", (*processes)[i].id, &(*processes)[i].arrival_time, &(*processes)[i].burst_time);   
        (*processes)[i].remaining_time = (*processes)[i].burst_time;        
        (*processes)[i].wait_time = 0;  
        (*processes)[i].turnaround_time = 0;    
    }

    fclose(file);
}

void display_output(Process *processes, int process_count, int algorithm, int quantum) {    // Display the output
    // Calculate total wait time and turnaround time
    int total_wait_time = 0;
    int total_turnaround_time = 0;  
    for (int i = 0; i < process_count; i++) {   
        total_wait_time += processes[i].wait_time;  
        total_turnaround_time += processes[i].turnaround_time;  
    }

    // Calculate average wait time and turnaround time
    float average_wait_time = (float) total_wait_time / process_count;
    float average_turnaround_time = (float) total_turnaround_time / process_count;

    // Display scheduling algorithm
    printf("\n");
    switch (algorithm) {
        case 0:
            printf("First Come First Served");
            break;
        case 1:
            printf("Shortest Job First");
            break;
        case 2:
            printf("Round Robin with Quantum %d", quantum);
            break;
        default:
            printf("Unknown");
    }
    printf("\n");

    // Display individual process wait and turnaround times
    for (int i = 0; i < process_count; i++) {
        printf("\nP%d", i);
        printf("\n\tWaiting time:          %2d", processes[i].wait_time);
        printf("\n\tTurnaround time:      %3d", processes[i].turnaround_time);
    }

    // Display average wait and turnaround times
    printf("\n\nTotal average waiting time:     %5.1f", average_wait_time);
    printf("\nTotal average turnaround time:  %5.1f\n", average_turnaround_time);
}

void first_come_first_served(Process *processes, int process_count) {
    int current_time = 0;

    for (int i = 0; i < process_count; i++) {
        // Calculate the wait time for the current process
        processes[i].wait_time = current_time - processes[i].arrival_time;

        // Update the current time
        current_time += processes[i].burst_time;

        // Calculate the turnaround time for the current process
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
    }
}


int compare_burst_time(const void *a, const void *b) {  // Compare function for qsort
    const Process *process_a = (const Process *)a;  // Cast the void pointers to Process pointers
    const Process *process_b = (const Process *)b;  

    if (process_a->burst_time < process_b->burst_time) {    // Compare the burst times
        return -1;  // Return -1 if process_a has a shorter burst time
    } else if (process_a->burst_time > process_b->burst_time) {  // Return 1 if process_b has a shorter burst time
        return 1;   
    } else {
        return 0;
    }
}

void shortest_job_first(Process *processes, int process_count) {    // Sort the processes by burst time
    int current_time = 0;

    // Sort the processes by burst time
    qsort(processes, process_count, sizeof(Process), compare_burst_time);

    for (int i = 0; i < process_count; i++) {
        // Calculate the wait time for the current process
        processes[i].wait_time = current_time - processes[i].arrival_time;

        // Update the current time
        current_time += processes[i].burst_time;

        // Calculate the turnaround time for the current process
        processes[i].turnaround_time = current_time - processes[i].arrival_time;
    }
}

void round_robin(Process *processes, int process_count, int time_quantum) {   
    int current_time = 0;
    int completed_processes = 0;    // Number of processes that have completed execution
    int all_processes_completed = 0;

    int *remaining_burst_times = (int *)malloc(sizeof(int) * process_count);    // Array of remaining burst times for each process
    for (int i = 0; i < process_count; i++) {   // Initialize the array
        remaining_burst_times[i] = processes[i].burst_time;
    }

    while (!all_processes_completed) {  // While all processes have not completed execution
        all_processes_completed = 1;

        for (int i = 0; i < process_count; i++) {   // Loop through all processes
            if (remaining_burst_times[i] > 0) { // If the process has not completed execution
                all_processes_completed = 0;

                if (remaining_burst_times[i] > time_quantum) {  
                    // Process needs to be preempted
                    current_time += time_quantum;
                    remaining_burst_times[i] -= time_quantum;
                } else {
                    // Process finishes executing
                    current_time += remaining_burst_times[i];   // Update the current time
                    processes[i].wait_time = current_time - processes[i].burst_time - processes[i].wait_time;   // Calculate the wait time
                    processes[i].turnaround_time = current_time - processes[i].wait_time;   // Calculate the turnaround time
                    remaining_burst_times[i] = 0;   // Set the remaining burst time to 0
                    completed_processes++;  // Increment the number of completed processes
                }
            }
        }
        // If all processes have been checked and none were completed,
        // increment the current time to avoid an infinite loop.
        if (all_processes_completed && completed_processes == 0) {
            current_time++;
        }
    }
    free(remaining_burst_times);
}

int main(int argc, char *argv[]) {  // Main function
    if (argc != 3 && argc != 4) {   // Check for valid number of arguments
        printf("Invalid number of arguments.\n");   
        return 1;
    }

    // Read input file
    Process *processes = NULL;  // Array of processes
    int process_count = 0;
    read_input_file(argv[2], &processes, &process_count);   // Read the input file

    // Run the scheduling algorithm
    int algorithm;
    if (strcmp(argv[1], "-f") == 0) {   // Check for the scheduling algorithm
        first_come_first_served(processes, process_count);  // Run the algorithm
        algorithm = 0;
    } else if (strcmp(argv[1], "-s") == 0) {    // Check for the scheduling algorithm
        shortest_job_first(processes, process_count);   // Run the algorithm
        algorithm = 1;
    } else if (strcmp(argv[1], "-r") == 0) {    // Check for the scheduling algorithm
        if (argc != 4) {    // Check for valid number of arguments
            printf("Invalid number of arguments for Round Robin.\n");   
            return 1;
        }
        int time_quantum = atoi(argv[3]);   // Get the time quantum
        if (time_quantum <= 0) {
            printf("Invalid time quantum.\n");  
            return 1;
        }
        round_robin(processes, process_count, time_quantum);    // Run the algorithm
        algorithm = 2;
    } else {    // Invalid scheduling algorithm
        printf("Invalid scheduling algorithm.\n");  
        return 1;
    }

    // Display the output
    display_output(processes, process_count, algorithm, argc == 4 ? atoi(argv[3]) : 0);

    // Clean up memory
    free(processes);

    return 0;
}


