#pragma once

#include <string.h>

#define C 1
#define JAVA 3

struct arguments {
  char *lang;
  int verb, inlin, block, jdoc, doxy, empty, arg_num;
  char **files;
};

struct options {
  int verb, inlin, block, jdoc, doxy, empty;
};

int check_language(char *lang);

void getOptions(struct arguments *args, struct options *opts);

