#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* Effectively my own strcpy, but one that drops all leading and
   trailing whitespace.  There's almost certainly a better/safer
   library function to do the same.

   Strings will need to be freed for any long-lived process.
*/

char *chomp(char *buf) {
  int tailidx = strlen(buf) - 1;
  int frontidx = 0;
  for( ; tailidx > -1 && isspace(buf[tailidx]); tailidx--) {
    /* do nothing, we're just looking for the tail of the effective string */
  }
  for( ; frontidx < tailidx && isspace(buf[frontidx]); frontidx++) {
    /* do nothing, we're just looking for the front of the effective string */
  }
  int len = (tailidx - frontidx) + 1;
  char *newbuf = calloc(len, sizeof(char));
  int newidx = 0;
  for( ; frontidx <= tailidx; frontidx++, newidx++) {
    newbuf[newidx] = buf[frontidx];
  }
  return newbuf;
}

void main(void) {
  char *input = NULL;
  size_t linecap = 0;
  while(getline(&input, &linecap, stdin) > 0) {
    char *chomped = chomp(input);
    printf("Chomped: '%s'\n", chomped);
  }
}
