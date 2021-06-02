# Project 2 Implementing Reliable Data Transfer Protocol

Implements the Alternating Bit Protocol for reliable data transfer. Also simulates an unreliable network environment for testing purposes. A diagram showing part of the project
is shown below.

![image](https://user-images.githubusercontent.com/32044950/120547567-4cd30600-c3bf-11eb-81de-ed6efdc4118a.png)

The part being implemented in this project is Layer 4 in the diagram.

More details can be found in "Project_2_A20.pdf".

## Files

### project2.c/.h

The environment used to simulate an unreliable network channel. Is represented by both Layer 3 and 5 in the diagram above.

![image](https://user-images.githubusercontent.com/32044950/120548077-eef2ee00-c3bf-11eb-97bf-3238b96d5894.png)

### student2A.c

The agent that is the sender of information. Transmits and retransmits packets to "student2B.c" as necessary.

### student2B.c

The agent that is the reciever of information. Verifies that recieved packets are non-corrupted and in-order and asks for retransmissions of packets as necessary.

### student_common.c/.h

A file that contains code commonly used by both "student2A.c" and "student2B.c".

### Makefile

The file used to compile the project.

## Usage
Put
Makefile,
project2.c,
project2.h,
student_common.c,
student_common.h,
student2A.c, and
student2B.c
into the same directory.

To compile the code do:
`make`

To run the code do:
`./p2`
