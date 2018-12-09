/* Simulation of Conway's Game of Life. Core code from Kruse, Leung
 * and Tondo: Data Structures and Program Design in C ©1991
 */

/*
 * Various tweaks applied by John R. Daily:
 *
 * - Input handling changed, can now be redirected from a file and
 *   instead of using -1 -1 as termination, end of file or a blank
 *   line will do the trick. Also don't wait for confirmation: proceed
 *   with the next generation after a pause
 *
 * - Some slightly cleaner code in places
 *
 * - Cleaner output (don't clutter up screen with dead cells)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#include "general.h"
#include "lifedef.h"
#include "calls.h"

int main(void) {
  int row, col;
  int generation = 0;
  Grid_type map;
  Grid_type newmap;

  Initialize(map);

  /* Will continue until the user exits with ^C */
  do {
    WriteMap(generation++, map);
    for (row = 0; row < MAXROW; row++) {
      for (col = 0; col < MAXCOL; col++) {
        switch (NeighborCount(row, col, map)) {
        case 0:
        case 1:
          newmap[row][col] = DEAD;
          break;
        case 2:
          newmap[row][col] = map[row][col];
          break;
        case 3:
          newmap[row][col] = ALIVE;
          break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
          newmap[row][col] = DEAD;
          break;
        }
      }
    }
    CopyMap(map, newmap);

    /* Possible improvements:
     * + allow sleep to be specified as CLI parameter in microseconds
     *   for usleep()
     * + allow number of generations to be specified instead
     */
    sleep(1);
  } while(1);
  return 0;
}

int valid(int row, int col) {
  return row >= 0 && row < MAXROW && col >= 0 && col < MAXCOL;
}

void Initialize(Grid_type map) {
  int row, col;
  printf("GoL simulation\n");
  for (row = 0; row < MAXROW; row++) {
    for (col = 0; col < MAXCOL; col++) {
      map[row][col] = DEAD;
    }
  }
  printf("On each line give a pair of live coordinates\n");
  printf("Terminate the list with an empty line\n");

  do {
    char *input = chomp(getrawline(stdin));
    if (strlen(input) == 0) {
      break;
    }

    sscanf(input, "%d %d", &row, &col);

    if (valid(row, col)) {
      map[row][col] = ALIVE;
    } else {
      printf("Invalid values\n");
    }
    free(input);
  } while(1);
}

void WriteMap(int gen, Grid_type map) {
  int row, col;
  printf("### Generation %d\n", gen);
  for (row = 0; row < MAXROW; row++) {
    for (col = 0; col < MAXCOL; col++) {
      if (map[row][col] == ALIVE) {
        printf("%s", LIVE_CELL);
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

/*
 * Make certain the row or cell index never drops below zero.
 */
int bottom(int index) {
  if (index <= 0) {
    return 0;
  }
  return index - 1;
}

/*
 * Make certain the row or cell index never exceeds the maximum.
 */
int top(int index, int max) {
  if (index >= max - 1) {
    return max - 1;
  }
  return index + 1;
}

int NeighborCount(int row, int col, Grid_type map) {
  int i, j;
  int rlow, rhigh;
  int clow, chigh;
  int count = 0;

  rlow = bottom(row);
  rhigh = top(row, MAXROW);
  clow = bottom(col);
  chigh = top(col, MAXCOL);

  for (i = rlow; i <= rhigh; i++) {
    for (j = clow; j <= chigh; j++) {
      if (map[i][j] == ALIVE) {
        count++;
      }
    }
  }
  if (map[row][col] == ALIVE) {
    count--;
  }
  return count;
}

void CopyMap(Grid_type dst, Grid_type src) {
  int row, col;
  for (row = 0; row < MAXROW; row++) {
    for (col = 0; col < MAXCOL; col++) {
      dst[row][col] = src[row][col];
    }
  }
}

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

/*
 * Either a blank line or EOF will result in an empty string
 */
char *getrawline(FILE *fp) {
  char *input = NULL;
  size_t linecap = 0;
  getline(&input, &linecap, fp);
  return input;
}
