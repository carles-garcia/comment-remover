#include <string.h>

#define C 1
#define JAVA 3

struct arguments;
struct options;

int check_language(char *lang);

void getOptions(struct arguments *args, struct options *opts);

