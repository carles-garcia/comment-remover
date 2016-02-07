/*
Copyright (C) 2015,2016  Carles Garcia Cabot  (github.com/carles-garcia)
This file is part of rcom, a tool to remove comments and documentation from
source code files. Released under the GNU GPLv3 (see COPYING.txt)
*/
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
