#include "parsing.h"

int check_language(char *lang) {
  if (strcmp(lang, "c") == 0) return C;
  if (strcmp(lang, "c++") == 0) return C;
  if (strcmp(lang, "java") == 0) return JAVA;
  return -1;
}

void eperror(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}