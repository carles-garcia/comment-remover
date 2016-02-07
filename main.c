/*
Copyright (C) 2015,2016  Carles Garcia Cabot  (github.com/carles-garcia)
This file is part of rcom, a tool to remove comments and documentation from
source code files. Released under the GNU GPLv3 (see COPYING.txt)
*/
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <limits.h>

#include "rcom.h"
#include "parsing.h"

#define TMPFILE ".rcom_temp"

const char *argp_program_version = "rcom v1.1.0";
const char *argp_program_bug_address = "https://github.com/carles-garcia/comment-remover/issues";
static char doc[] = 
"rcom -- a utility to remove comments and documentation from source code files";

static char args_doc[] = "c|c++|java [FILE...]";

static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"all", 'a', 0, 0, "Remove all types of comments"},
  {"inline", 'i', 0, 0, "Remove inline comments"},
  {"block", 'b', 0, 0, "Remove block comments"},
  {"javadoc", 'j', 0, 0, "Remove javadoc comments"},
  {"empty-lines", 'e', 0, 0, "Replace comment only lines with empty lines"},
  //{"doxygen", 'd', 0, 0, "Remove doxygen comments"}, NOT IMPLEMENTED
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  
  switch (key) {
    case 'v' :
      arguments->verbose = 1;
      break;
    case 'a' :
      arguments->inlin = arguments->block = 1;
      arguments->jdoc = arguments->doxy = 1; // exclusive...
    case 'i' :
      arguments->inlin = 1;
      break;
    case 'b' :
      arguments->block = 1;
      break;
    case 'j' :
      arguments->jdoc = 1;
      break;
    case 'd' :
      arguments->doxy = 1;
      break;
    case 'e' :
      arguments->empty = 1;
      break;
      
    case ARGP_KEY_ARG :
      /* Here we know that state->arg_num == 0, since we
       force argument parsing to end before any more arguments can
       get here. */
      if ((arguments->language = check_language(arg)) < 0) eperror("Wrong language"); 
      
      /* Now we consume all the rest of the arguments.
       state->next is the index in state->argv of the
       next argument to be parsed, which is the first string
       we’re interested in, so we can just use
       &state->argv[state->next] as the value for
       arguments->strings.
       
       In addition, by setting state->next to the end
       of the arguments, we can force argp to stop parsing here and
       return. */
      arguments->files = &state->argv[state->next];
      state->next = state->argc;
      break;
    case ARGP_KEY_END :
      if (state->arg_num < 1) argp_usage(state);
      arguments->arg_num = state->arg_num;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
      
static struct argp argp = {options, parse_opt, args_doc, doc};

int main(int argc, char *argv[]) {
  struct arguments args;
  memset(&args, 0, sizeof(struct arguments));
  argp_parse(&argp, argc, argv, 0, 0, &args);
  
  if (args.arg_num == 1) rcom(stdin, stdout, &args); // FIFO
  else {
    for (int i = 0; args.files[i]; ++i) {
      char *filename = args.files[i];
      
      FILE *source, *output;
      if ((source = fopen(filename, "r")) == NULL) 
	eperror(filename);
      
      char *resolved_path = realpath(filename, NULL); 
      // now we are sure temp file will be created in the same path as the source file
      if (resolved_path == NULL) eperror("Failed to obtain path");
      char tmpname[strlen(resolved_path) + strlen(TMPFILE) + 1];
      strcpy(tmpname, resolved_path);
      strcat(tmpname, TMPFILE);
      free(resolved_path);
      if ((output = fopen(tmpname, "w")) == NULL) //if exists it will be truncated
	eperror("Can't create temporal file");
    
      rcom(source, output, &args);
      
      if (fclose(source) != 0) eperror(filename);
      if (fclose(output) != 0) eperror(tmpname);
      
      char newname[strlen(filename)+2]; // '~' and '\0' 
      strcpy(newname, filename);
      strcat(newname,"~"); // strcat modifies first parameter !!!
      // backup the original file. Rename overwrites the newname file if it exists
      if (rename(filename, newname) < 0) eperror("Can't rename original file"); 
      if (rename(tmpname, filename) < 0) eperror("Can't rename temporary file");
      
      if (args.verbose) 
	fprintf(stderr, "Comments successfully removed from %s. Backup file %s created\n", filename, newname);
    }
    fprintf(stderr, "Done.\n");
  }
  
  return 0;
}
