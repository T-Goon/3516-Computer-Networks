#include "http_client.h"

int main (int argc, char* argv[]){
  struct addrinfo hints, *res;
  int status = 0;
  int sock = 0;
  int bytesRcvd = 1;
  int encoding = 0;

  char* urlIP = NULL;
  char* portNum = NULL;
  char* getRequest = "GET / HTTP/1.1\r\nConnection: close\r\n\r\n";
  char* fname = "index.html";
  char buffer[RCVBUFSIZE];

  struct timespec start_time, end_time;

  bool pOption = false;
  bool headerFound = false;
  bool chunkStarted = false;

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

  if((status = getaddrinfo(urlIP, portNum, &hints, &res)) != 0)
    DieWithError("Can't get address info");

    // Create socket
  if((sock = socket(res->ai_family, res->ai_socktype, 0)) < 0)
    DieWithError("socket() failed");

  // Log time before HTTP request
  if(pOption){
    clock_gettime(CLOCK_MONOTONIC, &start_time);
  }

  // Connect to server
  if(connect(sock, (struct sockaddr *) res->ai_addr, res->ai_addrlen) < 0)
    DieWithError("connect() failed");

  // Find the RTT
  if(pOption){
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    printf("\nRTT = %d milliseconds\n\n",
    (end_time.tv_sec - start_time.tv_sec) * 1000 +
    (end_time.tv_nsec - start_time.tv_nsec) / 1000000);
  }

  // Send GET request
  int a;
  if((a = send(sock, getRequest, strlen(getRequest), 0)) < 0)
    DieWithError("send() failed");

  FILE* file = fopen(fname, "w");
  int tot = 0;
  while(bytesRcvd != 0){
    memset(buffer, 0, RCVBUFSIZE);

    // Get data
    if((bytesRcvd = recv(sock, buffer, RCVBUFSIZE-1, 0)) < 0)
        DieWithError("recv() failed");
    buffer[bytesRcvd] = '\0';

    if(!headerFound){
      char* head_end = strstr(buffer, "\r\n\r\n");

      // encoding = findTransferEncoding(buffer, bytesRcvd);

      if(head_end == NULL){
        printf("%s", buffer);
      }else{
        char temp[RCVBUFSIZE+1];

        size_t len = head_end-buffer;
        strncpy(temp, buffer, len);
        temp[len] = '\0';
        printf("%s", temp);

        // processHTML(file, buffer+len+3, bytesRcvd-len-3, chunkStarted);
        fprintf(file, buffer+len+3);
        printf("%s\n", buffer+len+3);
        headerFound = true;
      }
    }else{
      fprintf(file, buffer);
      printf("%s\n", buffer);
      // processHTML(file, buffer, bytesRcvd, chunkStarted);
    }

    printf("bytesRcvd %d\n", bytesRcvd);
    tot += bytesRcvd;

  }
  printf("total bytesRcvd %d\n", tot);

  printf("\n\nPage saved to \"%s\"\n\n", fname);

  fclose(file);
  close(sock);
  exit(0);
}

// Process HTML response.
// Returns 0 to indicate termination and 1 otherwise.
// int processHTML(FILE* file, char* buffer, size_t size, enum transferEncoding te,
//   bool chunkStarted){
//   switch (te) {
//     case CHUNKED:
//     char* end;
//       if((end = strstr(buffer, "0\r\n\r\n")) != NULL){
//         char temp[RCVBUFSIZE];
//         size_t len = end-buffer;
//         strncpy(temp, buffer, len);
//         fprintf(file, temp);
//         return 0;
//       }
//
//       fprintf(file, buffer);
//       break;
//   }
//
//   return 1;
// }

// // Process a chunk of HTML response.
// // returns the length of the chunk.
// int processChunk(FILE* file, char* buffer, size_t size, bool chunkStarted){
//   // Find the length of the chunk if it exists in buffer
//   char chunkLenStr[100];
//   char* sep;
//   size_t len;
//   int chunkLen;
//   if(!chunkStarted){ // if not in middle of chunk look for beginning of next
//     sep = strstr(buffer,"\r\n");
//     len = sep-buffer;
//
//     strncpy(chunkLenStr, buffer, len);
//     chunkLen = atoi(chunkLenStr);
//   }
//
//   // If the chunk is not empty, print to file.
//   if(chunkLen != 0){
//     char content[RCVBUFSIZE];
//     char* endSep = strstr(sep+2, "\r\n");
//     len = endSep - (sep+2);
//     strncpy(content, sep+2, len);
//
//     fprintf(file, content);
//   }
//
//   return chunkLen;
// }

// Find the transfer encoding of the HTTP response
// int findTransferEncoding(char* buffer, size_t bytesRcvd){
//   char* header = strstr(buffer, "Transfer-Encoding:");
//
//   if(header != NULL){
//     char* type = strstr(header, "chunked");
//
//     if(type != NULL){
//       return CHUNKED;
//     }
//   }
//
//   return OTHER;
// }

// Display error message and exit
void DieWithError(char* msg){
  perror(msg);
  exit(1);
}
