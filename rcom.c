/*
Copyright (C) 2015,2016  Carles Garcia Cabot  (github.com/carles-garcia)
This file is part of rcom, a tool to remove comments and documentation from
source code files. Released under the GNU GPLv3 (see COPYING.txt)
*/
/*
 * The C language standard says a source file that is not empty shall end 
 * in a new-line character, which shall not be immediately preceded by a
 * backslash character.
 * 
 * Note: If there's a backslash-newline parsing might fail in rare cases.
 * The solution would be analyze char by char, or check if last character -2 == '\\'
 * 
 */

#include "rcom.h"

void rcom(FILE *source, FILE *output, struct arguments *opts) {
  char *buffer = NULL;  
  size_t n = 0;
  int comment = 0; // Are we inside a block comment?
  
  while (getline(&buffer, &n, source) > 0) {
    char copy[n+1];

    // Here n should be >= 2 (at least '\n' '\0')
    int finished = 0; // Has this line parsing finished?
    int white = 1; // Does this line have whitespace only?
    char quote = 0; // Are we inside a quote?
    int has_com = comment; // Does this line have a comment?
    int i = 0;
    for (int j = 0; !finished; ++j) { 
      switch (buffer[j]) {
	case '\n' :
	  copy[i++] = buffer[j];
	  finished = 1;
	  break;
	
	case '/' :
	  if (quote) copy[i++] = buffer[j];
	  else if (!comment) {
	    if (opts->inlin && buffer[j+1] == '/') {
	      ++i;
	      finished = 1;
	      has_com = 1;
	    }
	    else if (buffer[j+1] == '*') {
	      if (opts->jdoc && buffer[j+2] == '*') {
		comment = 1;
		has_com = 1;
		++j;
	      }
	      else if (opts->block) {
		if (buffer[j+2] == '*') {
		  white = 0;
		  copy[i++] = buffer[j];
		  ++j;
		  copy[i++] = buffer[j];
		  ++j;
		  copy[i++] = buffer[j];
		}
		else {
		  comment = 1;
		  has_com = 1;
		  ++j;
		}
	      }
	      else copy[i++] = buffer[j];
	    }
	    else copy[i++] = buffer[j];
	  } 
	  break;
	
	case '*' :
	  if (comment) {
	    if (buffer[j+1] == '/') {
	      comment = 0;
	      ++j;
	    }
	  }
	  else {
	    white = 0;
	    copy[i++] = buffer[j];
	  }
	  break;
	
	case '\"' :
	case '\'' :
	  if (comment) break;
	  if (!quote) {
	    white = 0;
	    quote = buffer[j];
	  }
	  else if (quote == buffer[j]) quote = 0;
	  copy[i++] = buffer[j];
	  break;
	
	default:
	  if (!comment) {
	    if (!isspace(buffer[j])) white = 0;
	    copy[i++] = buffer[j];
	  }
      }
    }
    
    // If this line has only white space but doesn't have a removed comment keep it
    if (white && !has_com) white = 0;
    if (!white) {
      copy[i-1] = '\n';
      copy[i] = '\0';
      fputs(copy, output);
    }
    else if (opts->empty) {
      copy[0] = '\n';
      copy[1] = '\0';
      fputs(copy, output);
    }
    free(buffer);
    buffer = NULL;
    n = 0;
  }
  free(buffer);
}
