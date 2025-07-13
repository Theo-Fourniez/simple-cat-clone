#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "errno.h"
#include <sys/stat.h>

#define READ_BUF_SIZE 512

FILE* getFilePointer(char path[]);
void printFile(FILE* file, char* buffer);

int main(int argc, char const *argv[]) {

  FILE* fp;
  char buffer[READ_BUF_SIZE + 1];

  for (int i=1; i < argc; i++) {  
    fp = argc > 1 ? getFilePointer((char*) argv[i]) : stdin;
    printFile(fp, buffer);
    if (fp != stdin) fclose (fp);
  }

  return EXIT_SUCCESS;
}

FILE* getFilePointer(char path[]){
  FILE* fp = fopen(path, "r");

  if(!fp){
    fprintf(stderr, "Could not open %s : %s", path, strerror(errno));
    exit(EXIT_FAILURE);
  }

  struct stat statbuf[1];
  fstat(fp->_fileno, statbuf);

  if(S_ISDIR(statbuf->st_mode)){
    fprintf(stderr, "Could not concatenate the file %s : File is a directory", path);
    exit(EXIT_FAILURE);
  }
  return fp;
}

void printFile(FILE* file, char* buffer){
  size_t bytesRead;
  // Buffer the file reading and print it to stdout
  while ((bytesRead = fread(buffer, sizeof(char), READ_BUF_SIZE, file)) > 0) {
    buffer[bytesRead] = '\0';  // Null-terminate the buffer
    printf("%s", buffer);
  }
}
