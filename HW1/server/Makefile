output: http_server.o
	gcc http_server.o -o http_server

http_client.o: http_server.c
	gcc -c http_server.c

clean:
	rm http_server.o http_server
