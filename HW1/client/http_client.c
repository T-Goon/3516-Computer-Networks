#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <netdb.h>
#include <arpa/inet.h>   /* for sockaddr_in and inet_addr() */
#include <stdlib.h>        /* for atoi() and exit() */
#include <string.h>       /* for memset() */
#include <unistd.h>
#include <stdbool.h>

#define RCVBUFSIZE 1024

void DieWithError(char* msg);

int main (int argc, char* argv[]){
  struct addrinfo hints, *res;
  int status;
  int sock;
  char* urlIP;
  char* portNum;
  bool pOption = false;
  char* getRequest = "GET / HTTP/1.1\r\n\r\n";
  int bytesRcvd;
  char* fname = "index.html";
  char buffer[RCVBUFSIZE];

  if(argc < 3 || argc > 4){
    fprintf(stderr, "Usage: %s [-p] <Server IP | url> <port number>\n",argv[0]);
    exit(1);
  } else if(argc == 3){ // No options
    urlIP = argv[1];
    portNum = argv[2];

  } else if(argc == 4){ // -p option
    if(strcmp("-p", argv[1]) != 0){
      fprintf(stderr, "Usage: %s [-p] <Server IP | url> <port number>\n",argv[0]);
      exit(1);
    }
    pOption = true;

    urlIP = argv[2];
    portNum = argv[3];
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  printf("urlIP: %s\nport: %s\n",urlIP, portNum);
  if((status = getaddrinfo(urlIP, portNum, &hints, &res)) != 0)
    DieWithError("Can't get address info");

    // Create socket
  if((sock = socket(res->ai_family, res->ai_socktype, 0)) < 0)
    DieWithError("socket() failed");

    // Connect to server
  if(connect(sock, (struct sockaddr *) res->ai_addr, res->ai_addrlen) < 0)
    DieWithError("connect() failed");

  printf("Connection Success\n");

  // Send GET request
  // TODO partial sends, loop
  if(send(sock, getRequest, strlen(getRequest), 0) < 0)
    DieWithError("send() failed");

  FILE* file = fopen(fname, "w");

  //fcntl(sock, F_SETFL, O_NONBLOCK);

  struct pollfd ufds[1];
  ufds[0].fd = sock;
  ufds[0].events = POLLIN;
  int rv = poll(ufds, 1, 3500);

  memset(buffer, 0, RCVBUFSIZE);
  if((bytesRcvd =  recv(sock, buffer, RCVBUFSIZE-1, 0)) < 0)
      DieWithError("recv() failed");
  buffer[bytesRcvd] = '\0';
  printf("bytesRcvd %d\n", bytesRcvd);

  while (bytesRcvd != 0) {

    if(rv == 0)
      break;

    printf("%s\n", buffer);
    fprintf(file, buffer);
    memset(buffer, 0, RCVBUFSIZE);
    rv = poll(ufds, 1, 3500);
    if(rv == -1)
      DieWithError("poll() failed");
    else if(rv == 0)
      break;
    else{
      if(ufds[0].revents & POLLIN){
        if((bytesRcvd =  recv(sock, buffer, RCVBUFSIZE-1, 0)) < 0)
            DieWithError("recv() failed");
      }
    }
    printf("bytesRcvd %d\n", bytesRcvd);
    buffer[bytesRcvd] = '\0';
  }
  printf("while exit\n");

  fclose(file);
  close(sock);
  return 0;
}

void DieWithError(char* msg){
  perror(msg);
  exit(1);
}
