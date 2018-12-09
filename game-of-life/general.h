#include <stdio.h>
#include <stdlib.h>

typedef enum boolean_tag {FALSE, TRUE} Boolean_type;

void Error(char *);

void Error(char *s) {
  fprintf(stderr, "%s\n", s);
  exit(1);
}
