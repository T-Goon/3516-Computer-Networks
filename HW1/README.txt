Client:
Put http_server.c, http_server.h, TMDG.html, TMDG_files folder, and Makefile into the same directory.

To compile the code use "make".

To run the client:
./http_server <port number>

Server:
Put http_client.c, http_client.h, and Makefile into the same directory.

To compile the code use "make".

To run the client:
./http_client [-p] <server URL/ip> <port number>

Options:
-p - Makes the client print out the RTT of connecting the specified server.
