#include "rcom.h"
#define BUFSIZE 1024

//it might not work with lines longer than 1023 characters 
// todo: error handling
void rcom(FILE *source, FILE *output, int lang, struct options *opts) {
  char buffer[BUFSIZE], copy[BUFSIZE];
  char quote = 0;
  int comment = 0, i, finish;
  
  while (fgets(buffer, sizeof buffer, source)) {
    for (i = 0, finish = 0; !finish; ++i) {
      switch (buffer[i]) {
	// case '\0' , si no va lultima linia append un \n al final de cada fitxer
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
	  else copy[i] = buffer[i];
	  break;
	
	case '\"' :
	case '\'' :
	  if (comment) break;
	  if (!quote) quote = buffer[i];
	  else if (quote == buffer[i]) quote = 0;
	  copy[i] = buffer[i];
	  break;
	  
	// This case is for characters like \" 
	case '\\' :
	  if (!comment) {
	    copy[i] = buffer[i];
	    ++i;
	    copy[i] = buffer[i];
	  }
	  break;
	  
	default:
	  if (!comment) copy[i] = buffer[i];
      }
    }
    copy[i-1] = '\n';
    copy[i] = '\0';
    fputs(copy, output); 
  }
}