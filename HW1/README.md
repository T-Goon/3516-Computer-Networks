# Project 1 Socket Programming

A simple HTTP client and server made from scratch using C socket programming.

More details can be found in "Project_1_A20/Project_1_A20.pdf".

## Client:

A C program that prompts the user for an ip address or URL and then sends a GET request to download the HTML of the webpage hosted there.

### Usage
Put http_server.c, http_server.h, TMDG.html, TMDG_files folder, and Makefile into the same directory.

To compile the code use `make`.

To run the client:
`./http_server <port number>`

## Server:

A C program that hosts a simple website and appropriately handles a variety of HTTP requests and responses.

### Usage
Put http_client.c, http_client.h, and Makefile into the same directory.

To compile the code use `make`.

To run the client:
`./http_client [-p] <server URL/ip> <port number>`

Options:
- -p - Makes the client print out the RTT (Round trip time) of connecting the specified server.
