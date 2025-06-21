# NOTE!
This branch has not completely integrated the scheduler implementation to the OS Emulator
## In order to test the scheduler algorithm, run:
```bash
  ./scheduler
```
## Sample Run
```bash
OS Scheduler started.
Scheduler set to: FCFS
Process 0 assigned to Core 0 with 15 ticks.
Process 1 assigned to Core 1 with 15 ticks.
Process 2 assigned to Core 2 with 15 ticks.
Process 3 assigned to Core 3 with 15 ticks.
Executing instruction 1 of process Process1Executing instruction 1 of process Process3

Executing instruction 1 of process Process0
Executing instruction 1 of process Process2
Executing instruction 2 of process Process1
Executing instruction 2 of process Process3
Executing instruction 2 of process Process0
Executing instruction 2 of process Process2
Executing instruction 3 of process Process1
Executing instruction 3 of process Process0
Executing instruction 3 of process Process3
Executing instruction 3 of process Process2
Executing instruction 4 of process Process1
Executing instruction 4 of process Process3
Executing instruction 4 of process Process0
Executing instruction 4 of process Process2
Executing instruction 5 of process Process0
Executing instruction 5 of process Process3
Executing instruction 5 of process Process1
Executing instruction 5 of process Process2
Process 0 finished.
Process 1 finished.
Process 3 finished.
Process 2 finished.
Process 4 assigned to Core 0 with 15 ticks.
Process 5 assigned to Core 1 with 15 ticks.
Process 6 assigned to Core 2 with 15 ticks.
Process 7 assigned to Core 3 with 15 ticks.
Executing instruction 1 of process Process6
Executing instruction 1 of process Process5Executing instruction 1 of process Process4
Executing instruction 1 of process Process7

Executing instruction 2 of process Process6
Executing instruction 2 of process Process4
Executing instruction 2 of process Process5
Executing instruction 2 of process Process7
Executing instruction 3 of process Process6
Executing instruction 3 of process Process7
Executing instruction 3 of process Process5
Executing instruction 3 of process Process4
Executing instruction 4 of process Process7
Executing instruction 4 of process Process5
Executing instruction 4 of process Process6
Executing instruction 4 of process Process4
Executing instruction 5 of process Process6
Executing instruction 5 of process Process5
Executing instruction 5 of process Process4
Executing instruction 5 of process Process7
Process 6 finished.
Process 4 finished.
Process Process 85 finished. assigned to Core 
Process 7 finished.
0 with 15 ticks.
Process 9 assigned to Core 1 with 15 ticks.
Executing instruction 1 of process Process8
Executing instruction 1 of process Process9
Executing instruction 2 of process Process9
Executing instruction 2 of process Process8
Executing instruction 3 of process Process9Executing instruction 3 of process Process8

Executing instruction 4 of process Process9
Executing instruction 4 of process Process8
Executing instruction 5 of process Process9Executing instruction 5 of process Process8

Process 8 finished.
Process 9 finished.
Finished Processes:
Process ID: 0, Name: Process 0
Process ID: 1, Name: Process 1
Process ID: 3, Name: Process 3
Process ID: 2, Name: Process 2
Process ID: 6, Name: Process 6
Process ID: 4, Name: Process 4
Process ID: 5, Name: Process 5
Process ID: 7, Name: Process 7
Process ID: 8, Name: Process 8
Process ID: 9, Name: Process 9
Core 0 stopped.
Core 1 stopped.
Core 2 stopped.
Core 3 stopped.
Scheduler stopped.
OS Scheduler finished.

```

## Locate Main.cpp and open Terminal Directory
```bash
C:\user\OS_EMULATOR
```

## Compile and Run (Linux)
Enter the following commands: 
```bash
g++ o- Main -I headers source/*.cpp
./Main
```

## Compile and Run (Mac)
Enter the following commands:
```bash
g++ -std=c++20 -o Main -I headers source/*.cpp
./Main
```

## Compile and Run (Windows)
Enter the following commands: 
```bash
g++ -o Main -I headers source/*.cpp 
Main
```
