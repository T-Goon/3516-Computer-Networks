# Project 3 Distance Vector Routing Protocol

Implements a distributed asynchronous distance vector routing protocol within a simulated environment with 4 nodes.

![image](https://user-images.githubusercontent.com/32044950/120549774-fe733680-c3c1-11eb-9c24-bb2ec186551d.png)

The program has each of the 4 nodes create and update a distance vector that will allow each node to find the shortest path to any other node in the simulated network graph.

More details can be found in "Project_3_A20.pdf".

## Files

### NodeConfigurationFile

A file that contains the adjacency matrix for the graph in the above image.

### node[0/1/2/3].c

The C files that hold the code for the initialization and updating of each node's distance vector. A diagram of the function relationships in each node is shown below.

![image](https://user-images.githubusercontent.com/32044950/120550526-fc5da780-c3c2-11eb-8ae6-31f150669486.png)


### node_common.c/.h

The files that hold the code common to node[0/1/2/3].c.

## Usage
Put
Makefile,
node_common.c,
node_common.h,
node0.c,
node1.c,
node2.c,
node3.c,
NodeConfigurationFile,
project3.c,
and project3.h

into the same directory.

To compile the code use:
`make`

To run the code use:
`./project3`
