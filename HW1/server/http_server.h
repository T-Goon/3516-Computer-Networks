#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), bind(), and connect() */
#include <sys/types.h> // getaddrinfo
#include <netdb.h> // getaddrinfo
#include <arpa/inet.h>   /* for sockaddr_in and inet_ntoa() */
#include <stdlib.h>       /* for atoi() and exit() */
#include <string.h>       /* for memset() */
#include <unistd.h>      /* for close() */

#define RCVBUFSIZE 2048

void DieWithError(char* msg);

void HandleTCPClient(int clntSocket);

char* getMethod(char* buffer, size_t size);
void getFile(char* buffer, size_t bytesRcvd, char** res);
int getVer(char* buffer, size_t bytesRcvd);
void genOK(char* meth, char* file, char** res, int* size);
char* genBadRequest();
char* genUnsupVersion();

#endif
