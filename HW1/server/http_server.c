#include "http_server.h"

int main(int argc, char* argv[]){
  int servSock, otherSock;
  struct addrinfo hints, *res;
  struct sockaddr storage, other_addr;
  socklen_t addr_size;

  if(argc > 2 || argc < 1){
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

  while(1){
    addr_size = sizeof(other_addr);

    if((otherSock = accept(servSock, (struct sockaddr*)&other_addr, &addr_size)) < 0)
      DieWithError("accept() failed");
    printf("Accepted Client\n");

    HandleTCPClient(otherSock);
  }

  close(servSock);
  return 0;
}

void HandleTCPClient(int clntSocket){
  char buffer[RCVBUFSIZE];
  int bytesRcvd;

  if((bytesRcvd = recv(clntSocket, buffer, RCVBUFSIZE-1, 0)) < 0)
    DieWithError("recv() failed");
  buffer[bytesRcvd] = '\0';

  printf("Getting Method\n");
  char* method = getMethod(buffer, bytesRcvd);
  printf("Got method %s\n", method);
  char* file;
  memset(file, 0, sizeof(file));
  getFile(buffer, bytesRcvd, &file);
  printf("Got file k%sk\n", file);
  int version = getVer(buffer, bytesRcvd);
  printf("Got version %d\n", version);

  char* response;
  int size;

  printf("Gen res\n");
  if(method == NULL || file == NULL || version == -1){
    response = genBadRequest();
  }else if(version == 1){
    response = genUnsupVersion();
  }else{

    genOK(method, file, &response, &size);
  }

  // Send HTTP response.
  // int sent;
  // printf("size %d\n",size);
  // printf("res %s\n", response);
  // if((sent = send(clntSocket, response, size, 0)) < 0)
  //   DieWithError("send() fail");
  // printf("sent %d\n", sent);

  int test = strlen(response);
  printf("test %d\n", test);
  printf("res size2 %d\n",strlen(response));
  size_t left = size+500;
  size_t len = +500;
  size_t sentBytes = 0;
  size_t bytes;
  printf("left out %d\n",left);
  printf("len out %d\n",len);
  while(sentBytes < len){
    printf("left %d\n", left);
    if((bytes = send(clntSocket, response+sentBytes, left, 0)) < 0)
      DieWithError("send() fail");
    sentBytes += bytes;
    left -= bytes;
    printf("left2 %d\n", left);
    printf("sent %d\n", bytes);
    printf("sentBytes %d\n", sentBytes);
  }

  free(response);

  close(clntSocket);
}

// Finds the method portion of the HTTP request.
// returns the method string or 400 for bad request.
char* getMethod(char* buffer, size_t size){
  printf("%s\n", buffer);
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

  printf("space1 %s\n", space1);
  printf("space2 %s\n", space2);

  // Error check
  if(space1==NULL || space2==NULL){
    *res = NULL;
  }

  // Get the file path string.
  size_t len = space2 - space1-1;
  if(len > 100)
    *res = NULL;
  char path[len];
  strncpy(path, space1+1, len);
  path[len] = '\0';
  printf("path h%sh\n", path);

  // Valid files
  if(!(strcmp(path, "/") == 0 ||
  strcmp(path, "index.html") == 0 ||
  strcmp(path, "/index.html") == 0)){
    *res = NULL;
  }

  *res = path;
  printf("res %s\n", *res );
}

// Gets the version of the HTTP request.
// Returns 0 if version 1.1, 1 if version not 1.1, and -1 otherwise.
int getVer(char* buffer, size_t bytesRcvd){
  char* http = strstr(buffer, "HTTP");
  if(http == NULL){
    return -1;
  }

  char* slash = strstr(http, "/");
  char* end = strstr(slash, "\r\n");

  if(slash == NULL || end == NULL)
    return -1;

  char ver[10];
  size_t len = end-slash-1;
  strncpy(ver, slash+1, len);
  ver[len] = '\0';
  printf("version %sh\n", ver);

  if(strcmp(ver, "1.1") == 0)
    return 0;
  else
    return -1;
}

// Gerate the HTTP response for OK.
void genOK(char* meth, char* file, char** res, int* size){
  // Open file and find size
  FILE* f = fopen("TMDG.html", "r");
  if(f == NULL)
    DieWithError("File Not Found");

  fseek(f, 0L, SEEK_END);
  long int fileLen = ftell(f);
  printf("fileLen %d\n", fileLen);
  fseek(f, 0L, SEEK_SET);

  char* status = "HTTP/1.1 200 OK\r\n\r\n";
  printf("status len %d\n", strlen(status));
  *size = strlen(status)+fileLen;

  char* response = malloc(*size);
  memset(response, 0, *size);
  strncpy(response, status, strlen(status));

  fread(response+strlen(status), sizeof(char), fileLen, f);

  printf("resLen %d\n", strlen(status)+fileLen);

  fclose(f);

  *res = response;
}

// Generate the HTTP response for bad request.
char* genBadRequest(){
  char* response = "HTTP/1.1 400 Bad Request\r\n\r\n";
  return response;
}

// Generate the HTTP response for Version not supported.
char* genUnsupVersion(){
  char* response = "HTTP/1.1 505 HTTP Version not supported\r\n\r\n";
}

void DieWithError(char* errorMessage){
  perror(errorMessage);
  exit(1);
}
