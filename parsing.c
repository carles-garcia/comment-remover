#include "parsing.h"

int check_language(char *lang) {
  if (strcmp(lang, "c") == 0) return C;
  if (strcmp(lang, "c++") == 0) return C;
  if (strcmp(lang, "java") == 0) return JAVA;
  return -1;
}

void getOptions(struct arguments *args, struct options *opts) {
  opts->verb = args->verb;
  opts->inlin = args->inlin;
  opts->block = args->block;
  opts->jdoc = args->jdoc;
  opts->doxy = args->doxy;
}
  