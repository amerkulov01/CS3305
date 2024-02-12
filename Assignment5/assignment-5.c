/*
* Andrei Merkulov, 251145994
* this program simulates a train system with 2 trains and 5 stations
* trains can pick up passengers at station 0 and drop them off at any other station 
* trains can only pick up passengers if they have room for them
* CS 3305 Assignment 5
* 2023-04-04
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_TRAINS 2
#define NUM_STATIONS 5

typedef struct {    
    int id; 
    int capacity;
} Train;

typedef struct {    
    int id;
    int passengers;
} Station;

Station stations[NUM_STATIONS] = {  
    {0, 500},
    {1, 50},
    {2, 100},
    {3, 250},
    {4, 100}
};

Train trains[NUM_TRAINS] = {
    {0, 100},
    {1, 50}
};

pthread_mutex_t station_mutexes[NUM_STATIONS];
pthread_mutex_t stdout_mutex;

void* train_simulation(void* arg) {     // trains pick up passengers at station 0 and drop them off at any other station
    Train* train = (Train*)arg;
    int passengers_to_drop_off = 0;
 
    while (stations[0].passengers > 0 || passengers_to_drop_off > 0) {  // trains can only pick up passengers if they have room for them
        for (int i = 0; i < NUM_STATIONS; i++) {    // trains can pick up passengers at station 0 and drop them off at any other station
            pthread_mutex_lock(&station_mutexes[i]);

            pthread_mutex_lock(&stdout_mutex);  
            printf("Train %d ENTERS Station %d\n", train->id, i);
            fflush(stdout);
            pthread_mutex_unlock(&stdout_mutex);

            if (i == 0 && stations[i].passengers > 0) {  
                int passengers_to_pick_up = train->capacity - passengers_to_drop_off;       
                if (passengers_to_pick_up > stations[i].passengers) { // check if there are enough passengers to pick up  
                    passengers_to_pick_up = stations[i].passengers;
                }
                passengers_to_drop_off += passengers_to_pick_up;
                stations[i].passengers -= passengers_to_pick_up;

                sleep(passengers_to_pick_up / 10);  // this is to simulate the time it takes to pick up/drop off passengers

                pthread_mutex_lock(&stdout_mutex);  
                printf("        Train %d is at Station %d and has %d/%d passengers\n", train->id, i, passengers_to_drop_off, train->capacity);
                printf("        Station %d has %d passengers to pick up\n", i, stations[i].passengers);
                fflush(stdout);
                pthread_mutex_unlock(&stdout_mutex);
            } else if (i > 0 && passengers_to_drop_off > 0) {  // check if there are enough passengers to drop off
                int passengers_to_unload = stations[i].passengers;
                if (passengers_to_unload > passengers_to_drop_off) {    // check if there are enough passengers to drop off
                    passengers_to_unload = passengers_to_drop_off;
                }
                passengers_to_drop_off -= passengers_to_unload;
                stations[i].passengers -= passengers_to_unload;

                sleep(passengers_to_unload / 10);   

                pthread_mutex_lock(&stdout_mutex);
                printf("        Train %d is at Station %d and has %d/%d passengers\n", train->id, i, passengers_to_drop_off, train->capacity);
                printf("        Station %d has %d passengers to drop off\n", i, stations[i].passengers);
                fflush(stdout);
                pthread_mutex_unlock(&stdout_mutex);
            }

            pthread_mutex_lock(&stdout_mutex);  // print out the train and station status
            printf("Train %d LEAVES Station %d\n", train->id, i);
            fflush(stdout);
            pthread_mutex_unlock(&stdout_mutex);

            pthread_mutex_unlock(&station_mutexes[i]);
        }
    }
    return NULL;
}

int main() {
    pthread_t train_threads[NUM_TRAINS];

    for (int i = 0; i < NUM_STATIONS; i++) {    // initialize the mutexes
        pthread_mutex_init(&station_mutexes[i], NULL);  // 
    }
    pthread_mutex_init(&stdout_mutex, NULL);    // initialize the mutex for stdout

    for (int i = 0; i < NUM_TRAINS; i++) {  // create the trains
        pthread_create(&train_threads[i], NULL, train_simulation, &trains[i]);  // create the trains
    }
    for (int i = 0; i < NUM_TRAINS; i++) {  // join the trains
    pthread_join(train_threads[i], NULL);
    }

    for (int i = 0; i < NUM_STATIONS; i++) {    // destroy the mutexes
        pthread_mutex_destroy(&station_mutexes[i]);
    }
    pthread_mutex_destroy(&stdout_mutex);   // destroy the mutex for stdout

    return 0;
}

