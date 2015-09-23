#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include "rcom.h"
#include "parsing.h"

#define TMPFILE "rcom_temp"

const char *argp_program_version = "rcom version 1.0.0";
const char *argp_program_bug_address = "https://github.com/carles-garcia/comment-remover/issues";
static char doc[] = 
"rcom -- a utility to remove comments and documentation from source code files";

static char args_doc[] = "rcom LANGUAGE [OPTION...] [FILE...]";

static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output"},
  {"inline", 'i', 0, 0, "Remove inline comments"},
  {"block", 'b', 0, 0, "Remove block comments"},
  {"javadoc", 'j', 0, 0, "Remove javadoc comments"},
  {"doxygen", 'd', 0, 0, "Remove doxygen comments"},
  {0}
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
  struct arguments *arguments = state->input;
  
  switch (key) {
    case 'v' :
      arguments->verb = 1;
      break;
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
    case ARGP_KEY_ARG :
      /* Here we know that state->arg_num == 0, since we
       f orce argument parsing *to end before any more arguments can
       get here. */
      arguments->lang = arg;
      
      /* Now we consume all the rest of the arguments.
       s tate->next is the inde*x in state->argv of the
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
      if (state->arg_num < 2) argp_usage(state);
      arguments->arg_num = state->arg_num;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
      
static struct argp argp = {options, parse_opt, args_doc, doc};

void eperror(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}


int main(int argc, char *argv[]) {
  int lang;
    
  struct arguments args;
  args.verb = args.inlin = args.block = 0; 
  args.jdoc = args.doxy = 0;
  
  argp_parse(&argp, argc, argv, 0, 0, &args);
  
  if ((lang = check_language(args.lang) < 0)) eperror("Wrong language"); //TODO
  
  struct options opts;
  // get just the necessary options to avoid passing unnecessary strings to the stack
  getOptions(&args, &opts); //TODO
  
  if (args.arg_num == 2) rcom(stdin, stdout, lang, opts); // FIFO
  else {
    for (int i = 0; args.files[i]; ++i) {
      FILE* source, output;
      if ((source = fopen(args.files[i], "r")) == NULL) 
	eperror(args.files[i]);
      
      if ((output = fopen(TMPFILE, "w")) == NULL) 
	eperror("Can't create temporal file");
    
      rcom(source, output, lang, opts);
      
      if (fclose(source) != 0) eperror(args.files[i]);
      if (fclose(output) != 0) eperror("Can't close temporal file")
      
      if (rename(args.files[i], strcat(args.files[i],"~") < 0) eperror("Can't rename original file");
      if (rename(TMPFILE, args.files[i]) < 0) eperror("Can't rename temporal file");
    }
  }
  
  return 0;
  
}
  
  