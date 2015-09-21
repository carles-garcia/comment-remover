#include <stdio.h>

#define BUFSIZE 1024
//it might not work with lines longer than 1023 characters 
// todo: error handling
void remove(FILE *source, FILE *output) {
  char buffer[BUFSIZE], copy[BUFSIZE]
  char quote = 0;
  int comment = 0;
  
  while (fgets(buffer, sizeof buffer, source) {
    for (int i = 0; buffer[i]; ++i) {
      switch (buffer[i]) {
	case '/' :
	  if (!comment && !quote) {
	    if (buffer[i+1] == '/') {
	      copy[i] = '\n';
	      // why fill with zeros ??
	    }
	    else if (buffer[i+1] == '*') {
	      comment = 1;
	      ++i;
	    }
	  }
	  else copy[i] = buffer[i];
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
	  break;
	  
	// This case is for characters like \" 
	case '\\' :
	  if (!comment) {
	    copy[i] = buffer[i]
	    ++i;
	    copy[i] = buffer[i];
	  }
	  break;
	  
	default:
	  if (!comment) copy[i] = buffer[i];
      }
    }
    fputs(copy, output); // si escriu mes del comptes es posa un \0 al final i ja esta
  }
}
