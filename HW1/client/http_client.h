#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include <stdio.h>          /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>   /* for sockaddr_in and inet_addr() */
#include <stdlib.h>        /* for atoi() and exit() */
#include <string.h>       /* for memset() */
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define RCVBUFSIZE 2048

// typedef enum transferEncoding{
//   CHUNKED,
//   OTHER
// };

void DieWithError(char* msg);

#endif
