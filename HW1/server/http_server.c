#include "http_server.h"

volatile sig_atomic_t done = 0;

int main(int argc, char* argv[]){
  int servSock, otherSock;
  struct addrinfo hints, *res;
  struct sockaddr storage, other_addr;
  socklen_t addr_size;

  struct sigaction action;
  memset(&action, 0, sizeof(struct sigaction));
  action.sa_handler = term;
  sigaction(SIGTERM, &action, NULL);
  sigaction(SIGINT, &action, NULL);

  if(argc != 2){
    printf("Usage:  %s <Server Port>\n", argv[0]);
    exit(1);
  }

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, argv[1], &hints, &res);

  if((servSock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0)
    DieWithError("socket() failed");

  if(bind(servSock, res->ai_addr, res->ai_addrlen) < 0)
    DieWithError("bind() failed");

  freeaddrinfo(res);

  if(listen(servSock, 5))
    DieWithError("listen() failed");
  printf("Listening on port %s\n", argv[1]);

  while(!done){
    addr_size = sizeof(other_addr);

    otherSock = accept(servSock, (struct sockaddr*)&other_addr, &addr_size);

    if(otherSock < 0 && !done)
      DieWithError("accept() failed");
    else if(otherSock >= 0 && !done)
      HandleTCPClient(otherSock);
  }

  printf("\nServer Shutting Down\n");
  close(servSock);
  return 0;
}

void HandleTCPClient(int clntSocket){
  char* buffer = malloc(RCVBUFSIZE);
  int bytesRcvd;

  if((bytesRcvd = recv(clntSocket, buffer, RCVBUFSIZE-1, 0)) < 0)
    DieWithError("recv() failed");
  buffer[bytesRcvd] = '\0';

  if(bytesRcvd == 0){
    close(clntSocket);
    return;
  }

  // Parse method, file, and version of HTTP request
  char* method = getMethod(buffer, bytesRcvd);

  char* file;
  getFile(buffer, bytesRcvd, &file);

  int version = getVer(buffer, bytesRcvd);

  // Generate the HTTP response.
  char* response;
  int size;
  if(method == NULL || version == -1){
    genBadRequest(&response, &size);
  }else if(file == NULL){
    genNotFound(&response, &size);
  }else if(version == 1){
    genUnsupVersion(&response, &size);
  }else{
    genOK(method, file, &response, &size);
  }
  free(file);

  // Send HTTP response
  size_t left = size;
  size_t len = size;
  size_t sentBytes = 0;
  size_t bytes;
  while(sentBytes < len){
    if((bytes = send(clntSocket, response+sentBytes, left, 0)) < 0)
      DieWithError("send() fail");
    sentBytes += bytes;
    left -= bytes;
  }

  free(response);
  free(buffer);
  close(clntSocket);
}

// Finds the method portion of the HTTP request.
// returns the method string or 400 for bad request.
char* getMethod(char* buffer, size_t size){
  if(strstr(buffer, "GET") ==  buffer){
    return "GET";
  }else if(strstr(buffer, "HEAD") == buffer){
    return "HEAD";
  }else
    return NULL;
}

// Gets the file path portion of the HTTP request.
// Stores result in res. res is the file path string or NULL otherwise.
void getFile(char* buffer, size_t bytesRcvd, char** res){
  char* space1 = strstr(buffer, " ");
  char* space2 = strstr(space1+1, " ");

  // Error check
  if(space1==NULL || space2==NULL){
    *res = NULL;
    return;
  }

  // Get the file path string.
  size_t len = space2 - space1-1;
  if(len > 100){
    *res = NULL;
    return;
  }

  char* path = malloc(len);
  memset(path, 0, len);
  strncpy(path, space1+1, len);
  path[len] = '\0';

  // Valid files
  if(!(strcmp(path, "/") == 0 ||
  strcmp(path, "/index.html") == 0 ||
  strcmp(path, "/TMDG.html") == 0)){
    *res = NULL;
    return;
  }

  *res = path;
}

// Gets the version of the HTTP request.
// Returns 0 if version 1.1, 1 if version not 1.1, and -1 otherwise.
int getVer(char* buffer, size_t bytesRcvd){
  // Locate HTTP version number
  char* http = strstr(buffer, "HTTP");
  if(http == NULL){
    return -1;
  }

  char* slash = strstr(http, "/");
  char* end = strstr(slash, "\r\n");

  if(slash == NULL || end == NULL)
    return -1;

  // Get version number
  size_t len = end-slash;
  char* ver = malloc(len);
  memset(ver, 0, len);
  strncpy(ver, slash+1, len-1);
  ver[len] = '\0';

  // Make sure it is 1.1
  if(strcmp(ver, "1.1") == 0){
    free(ver);
    return 0;
  }
  else{
    free(ver);
    return 1;
  }
}

// Gerate the HTTP response for OK.
void genOK(char* meth, char* file, char** res, int* size){
  char* status = "HTTP/1.1 200 OK\r\n\r\n";

  if(strcmp(meth, "HEAD") == 0){
    *res = malloc(strlen(status));
    strncpy(*res, status, strlen(status));
    *size = strlen(status);
    return;
  }

  // Open file and find size
  FILE* f = fopen("TMDG.html", "r");
  if(f == NULL)
    DieWithError("File Not Found");

  // Find size of file
  fseek(f, 0L, SEEK_END);
  long int fileLen = ftell(f);
  fseek(f, 0L, SEEK_SET);
  *size = strlen(status)+fileLen;

  // Allocate memory for response
  char* response = malloc(*size);
  memset(response, 0, *size);

  // Populate the response
  strncpy(response, status, strlen(status));
  fread(response+strlen(status), sizeof(char), fileLen, f);

  fclose(f);

  *res = response;
}

// Generate the HTTP response for bad request.
void genBadRequest(char** res, int* size){
  char* response = "HTTP/1.1 400 Bad Request\r\n\r\n";
  *res = malloc(strlen(response));
  strncpy(*res, response, strlen(response));
  *size = strlen(response);
}

// Generate the HTTP response for Version not supported.
void genUnsupVersion(char** res, int* size){
  char* response = "HTTP/1.1 505 HTTP Version not supported\r\n\r\n";
  *res = malloc(strlen(response));
  strncpy(*res, response, strlen(response));
  *size = strlen(response);
}

// Generate the HTTP response for file not found
void genNotFound(char** res, int* size){
  char* response = "HTTP/1.1 404 Not Found\r\n\r\n";
  *res = malloc(strlen(response));
  strncpy(*res, response, strlen(response));
  *size = strlen(response);
}

// Handle termination signal
void term(int signum){
  done = 1;
}

// Display error and exit
void DieWithError(char* errorMessage){
  perror(errorMessage);
  exit(1);
}
