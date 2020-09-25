#include "http_client.h"

int main (int argc, char* argv[]){
  struct addrinfo hints, *res;
  int status = 0;
  int sock = 0;
  int bytesRcvd = 1;
  int encoding = 0;
  int rv;

  char* urlIP = NULL;
  char* portNum = NULL;
  char* getRequest;
  char* fname = "index.html";
  char buffer[RCVBUFSIZE];

  struct timespec start_time, end_time;
  struct pollfd ufds[1];

  bool pOption = false;
  bool headerFound = false;

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

  // Separate the file path and URL
  bool urlChanged = false;
  char* path = strstr(urlIP, "/");
  if(path == NULL)
    path = "/";
  else{
    char* temp = urlIP;
    urlIP = malloc(strlen(temp)-strlen(path));
    memset(urlIP, 0, strlen(temp)-strlen(path));
    strncpy(urlIP, temp, strlen(temp)-strlen(path));
    urlChanged = true;
  }

  getRequest = malloc(strlen(path)+38);
  memset(getRequest, 0, strlen(path)+38);
  sprintf(getRequest, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, urlIP);
  printf("%s\n", getRequest);
  printf("%s\n", portNum);


  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if((status = getaddrinfo(urlIP, portNum, &hints, &res)) != 0)
    DieWithError("Can't get address info");

  if(urlChanged)
    free(urlIP);

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

  free(getRequest);

  ufds[0].fd = sock;
  ufds[0].events = POLLIN;

  FILE* file = fopen(fname, "w");
  while(bytesRcvd != 0){
    memset(buffer, 0, RCVBUFSIZE);

    rv = poll(ufds, 1, 3500);
    if (rv == -1) {
      DieWithError("poll() failed"); // error occurred in poll()
    } else if (rv == 0) {
      break;
    }

    // Get data
    if((bytesRcvd = recv(sock, buffer, RCVBUFSIZE-1, 0)) < 0)
        DieWithError("recv() failed");
    buffer[bytesRcvd] = '\0';

    // Separate header from html body
    if(!headerFound){
      char* head_end = strstr(buffer, "\r\n\r\n");

      if(head_end == NULL){
        printf("%s", buffer);
      }else{
        char* temp = malloc(RCVBUFSIZE+1);
        memset(temp, 0, RCVBUFSIZE+1);

        size_t len = head_end-buffer;
        strncpy(temp, buffer, len);
        temp[len] = '\0';
        printf("%s", temp);

        fprintf(file, buffer+len+3);
        headerFound = true;

        free(temp);
      }
    }else{
      fprintf(file, buffer);
    }

    // Exit recv loop at end of body
    if((strstr(buffer, "</html>") != NULL) ||
    (strstr(buffer, "</HTML>") != NULL))
      break;
  }

  printf("\n\nPage saved to \"%s\"\n\n", fname);

  fclose(file);
  close(sock);
  exit(0);
}

// Display error message and exit
void DieWithError(char* msg){
  perror(msg);
  exit(1);
}
