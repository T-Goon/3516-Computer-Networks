#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void test(char** a){
  FILE* f = fopen("TMDG.html", "r");
  char* hello = "Hello monkey\n\n";

  fseek(f, 0L, SEEK_END);
  long int fileLen = ftell(f);
  printf("fileLen %d\n", fileLen);
  fseek(f, 0L, SEEK_SET);

  char* string = malloc(fileLen+strlen(hello));
  strncpy(string, hello, strlen(hello));

  fread(string+strlen(hello), sizeof(char), fileLen, f);

  *a = string;
  fclose(f);
}

int main(void){

  char* string;

  test(&string);


  printf("%s\n", string);
  free(string);
}
