#include "rcom.h"
#define BUFSIZE 1024

//it might not work with lines longer than 1023 characters 
void rcom(FILE *source, FILE *output, int lang, struct options *opts) {
  char buffer[BUFSIZE], copy[BUFSIZE];
  char quote = 0;
  int comment = 0, i, finish, white;
  
  // TODO doxy
  while (fgets(buffer, sizeof buffer, source)) {
    for (i = 0, finish = 0, white = 1; !finish; ++i) { 
      /* Warning: this doesn't work if last line doesn't end wih \n,
       * however according to POSIX all text files you end with it.
       * Tools like cat & grep don't work without \n in the last line neither.
       * A solution would be adding a case '\0' and append a \n to the EOF.
       */
      switch (buffer[i]) {
	
	case '\n' :
	  finish = 1;
	  break;
	
	case '/' :
	  if (quote) copy[i] = buffer[i];
	  else if (!comment) {
	    if (opts->inlin && buffer[i+1] == '/') finish = 1;
	    else if (buffer[i+1] == '*') {
	      if (opts->jdoc && buffer[i+2] == '*') {
		comment = 1;
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
		  ++i;
		}
	      }
	    }
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
	  
	// This case is for characters like \" 
	case '\\' : // review escape sequences... is it necessary
	  if (!comment) {
	    white = 0;
	    copy[i] = buffer[i];
	    ++i;
	    copy[i] = buffer[i];
	  }
	  break;
	  
	default:
	  if (!comment) {
	    if (!isspace(buffer[i])) white = 0;
	    copy[i] = buffer[i];
	  }
      }
    }
    if (white) {
      copy[0] = '\n';
      copy[1] = '\0';
    }
    else {
      copy[i-1] = '\n';
      copy[i] = '\0';
    }
    fputs(copy, output); 
  }
}