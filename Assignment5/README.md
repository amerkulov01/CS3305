# CS 3305 - Assignment 5: Train System Simulation
## Description
This program, created by Andrei Merkulov for CS 3305, simulates a train system with 2 trains operating across 5 stations. It aims to demonstrate concurrent programming concepts using pthreads in C. The simulation includes picking up passengers at station 0 and dropping them off at any of the other stations, provided there's enough room on the train.

## Features
- Concurrent Train Operation: Utilizes pthreads to simulate two trains operating in parallel.
- Station Management: Manages 5 stations with varying numbers of passengers waiting to be picked up.
- Passenger Pickup and Dropoff: Trains pick up passengers from station 0 and drop them off at subsequent stations, adhering to train capacity constraints.
- Synchronization: Employs mutexes to manage access to shared resources like station data and standard output, ensuring thread safety.
