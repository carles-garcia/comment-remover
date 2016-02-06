#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define C 1
#define JAVA 3

struct arguments {
  int verbose, inlin, block, jdoc, empty;
  int doxy; //not used yet
  int arg_num;
  int language;
  char **files;
};

int check_language(char *lang);

void eperror(char *msg);