#include <stdio.h>
#include <stdlib.h>

#define TMPFILE "rcom_temp"

void rcom(FILE *source, FILE *output);

void usage() {
  const char* msg =
  "usage: rcom [-options] sourcefile [outputfile]\n"
  + "If outputfile is not specified, sourcefile is overwritten\n";
  fprintf(stderr, msg);
  exit(EXIT_FAILURE);
}

void eperror(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[]) {
  int overwrite = 0;
  char *in = argv[1]; //input path
  char *out = argv[2];
  
  // FIFO
  if (argc == 1) {
    rcom(stdin, stdout);
    return 0;
  }
  
  for (int i = 1; i < argc; ++i) {
  //if (argc != 3) usage();
  FILE* source;
  FILE* output;
  if ((source = fopen(in, "r")) == NULL) 
    eperror(in);
  
  if (overwrite) {
    if ((output = fopen(TMPFILE, "w")) == NULL) 
      eperror("Can't create temporal file");
  }
  else {
    // If the file exists, it's truncated. Otherwise it's created
    if ((output = fopen(out, "w")) == NULL) 
      eperror(out);
  }
  
  rcom(source, output);
  if (fclose(source) != 0) eperror(in);
  if (fclose(output) != 0) eperror(out)
  if (overwrite) {
    if (remove(in) < 0) eperror(in);
    if (rename(TMPFILE, in) < 0) eperror(in);
  }
  
  return 0;
  
}
  
  