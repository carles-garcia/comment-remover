#include "rcom.h"

void rcom(FILE *source, FILE *output, struct arguments *opts) {
  char *buffer = NULL;  
  size_t n = 0;
  int comment = 0; // Are we inside a block comment?
  
  while (getline(&buffer, &n, source) > 0) {
    char *copy = malloc(n);
    if (copy == NULL) eperror("Failed to malloc copy buffer");
    // Here n should be >= 2 (at least '\n' '\0')
    int finished = 0; // Has this line parsing finished?
    int white = 1; // Does this line have whitespace only?
    char quote = 0; // Are we inside a quote?
    int has_com = comment; // Does this line have a comment?
    int i;
    for (i = 0; !finished; ++i) { 
      /* Warning: this doesn't work if last line doesn't end wih \n,
       * however according to POSIX all text files you end with it.
       * Tools like cat & grep don't work without \n in the last line neither.
       * A solution would be adding a case '\0' and append a \n to the EOF.
       * 
       * The C language standard says A source file that is not empty shall end 
       * in a new-line character, which shall not be immediately preceded by a
       * backslash character.
       * 
       * 
       */
      switch (buffer[i]) {
	case '\n' :
	  finished = 1;
	  break;
	
	case '/' :
	  if (quote) copy[i] = buffer[i];
	  else if (!comment) {
	    if (opts->inlin && buffer[i+1] == '/') {
	      finished = 1;
	      has_com = 1;
	    }
	    else if (buffer[i+1] == '*') {
	      if (opts->jdoc && buffer[i+2] == '*') {
		comment = 1;
		has_com = 1;
		i += 2;
	      }
	      else if (opts->block) {
		if (buffer[i+2] == '*') {
		  white = 0;
		  copy[i] = buffer[i];
		  ++i;
		  copy[i] = buffer[i];
		  ++i;
		  copy[i] = buffer[i];
		}
		else {
		  comment = 1;
		  has_com = 1;
		  ++i;
		}
	      }
	      else copy[i] = buffer[i];
	    }
	    else copy[i] = buffer[i];
	  } 
	  break;
	
	case '*' :
	  if (comment) {
	    if (buffer[i+1] == '/') {
	      comment = 0;
	      ++i;
	    }
	  }
	  else {
	    white = 0;
	    copy[i] = buffer[i];
	  }
	  break;
	
	case '\"' :
	case '\'' :
	  if (comment) break;
	  if (!quote) {
	    white = 0;
	    quote = buffer[i];
	  }
	  else if (quote == buffer[i]) quote = 0;
	  copy[i] = buffer[i];
	  break;
	// backslash newline may fail. The solution would be analyze char by char
	default:
	  if (!comment) {
	    if (!isspace(buffer[i])) white = 0;
	    copy[i] = buffer[i];
	  }
      }
    }
    //if (white && !has_com && !comment) white = 0;
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
    free(copy);
    buffer = NULL;
    n = 0;
  }
  free(buffer);
}