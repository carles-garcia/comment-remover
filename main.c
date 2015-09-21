#include <stdio.h>
#include <stdlib.h>

void rcom(FILE *source, FILE *output);

void usage() {
  const char* msg =
  "usage: rcom sourcefile outputfile";
  fprintf(stderr, msg);
  exit(EXIT_FAILURE);
}

void eperror(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  if (argc != 3) usage();
  FILE* source;
  FILE* output;
  if ((source = fopen(argv[1], "r")) == NULL) 
    eperror(argv[1]);
  // If the file exists, it's truncated. Otherwise it's created
  if ((output = fopen(argv[2], "w")) == NULL) 
    eperror(argv[2]);
  
  rcom(source, output);
  if (fclose(source) != 0) eperror(argv[1]);
  if (fclose(output) != 0) eperror(argv[2]);
  
  return 0;
  
}
  
  