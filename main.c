#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "rcom.h"

#define TMPFILE "rcom_temp"

const char *argp_program_version = "rcom version 1.0.0";
const char *argp_program_bug_address = "https://github.com/carles-garcia/comment-remover/issues";
static char doc[] = "rcom -- a utility to remove comments from source code files";

struct arguments {
  FILE *in, *out;
  int inlin, block;
}



void usage() {
  const char *msg =
  "usage: rcom c|c++|java|any [-options] [sourcefile] [outputfile]\n"
  + "If outputfile is not specified, sourcefile is overwritten\n"
  + "If neither sourcefile nor outpufile are specified, stdin and stdout are used\n";
  fprintf(stderr, msg);
  exit(EXIT_FAILURE);
}

void eperror(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
  int overwrite = 0;
  char *in = argv[1]; //input path
  char *out = argv[2];

  struct arguments arguments;
  arguments.in = arguments.out = "-";
  arguments.inlin = arguments.block = 0;
  
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
  
  rcom(arguments);
  if (fclose(source) != 0) eperror(in);
  if (fclose(output) != 0) eperror(out)
  if (overwrite) {
    if (remove(in) < 0) eperror(in);
    if (rename(TMPFILE, in) < 0) eperror(in);
  }
  
  return 0;
  
}
  
  