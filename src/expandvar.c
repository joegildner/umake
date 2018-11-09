#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "arg_parse.h"
#include <string.h>


#include "targets.h"
#include "linetype.h"

int expand(char* orig, char* new, int newsize);

int main(int argc, char* argv[]) {
  char* line = strdup("Hello I am ${");
  char expanded[1024];

  int err = expand(line, expanded, 1024);

  printf("%s\n",expanded);
  printf("%d", err);

}

/* Expand
 * orig    The input string that may contain variables to be expanded
 * new     An output buffer that will contain a copy of orig with all
 *         variables expanded
 * newsize The size of the buffer pointed to by new.
 * returns 1 upon success or 0 upon failure.
 */
int expand(char* orig, char* new, int newsize){
  char* varstart = NULL;

  while(*orig!='\0'){
    if(varstart != NULL){
      if(*orig == '}'){
        *orig = '\0';
        sprintf(new, "%s", getenv(varstart));
        new+=strlen(new);
        varstart = NULL;
      }
      else if(*orig == '{') {
        fprintf(stderr, "Unable to expand, mismatched \'{\'\n");
        break;
      }
    }
    else{
      if(*orig == '$' && orig[1] == '{' && varstart == NULL){
        varstart = orig+2;
        orig += 2;
      }else{
        *new = *orig;
        new++;
      }
    }
    orig++;
  }

  return (varstart == NULL);

}
