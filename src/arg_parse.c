/* arg_parse
 * 23 OCT 2018, Joe Gildner
 */

#include "arg_parse.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* arg_parse
 * line   a command line, possibly with arguments included, delimited by whitespace
 * the arg parse function takes a command line and constructs an array of pointers to
 * strings, where the first string is the command and all sequential strings are
 * arguments for that command
 */
char** arg_parse(char* line, int *argcp){
  *argcp = count_args(line);
  char* placePointers = line;
  char** argArray = malloc((*argcp+1) * sizeof(char*));

  for(int i=0; i<*argcp; i++){
    while(isspace(*placePointers)){
      placePointers++;
    }
    argArray[i] = placePointers;
    argArray[i+1] = NULL;
    placePointers+=strlen(placePointers)+1;
  }

  printf("%s\n", argArray[0]);

  return argArray;
}

/* count_args
 * line:    a command line with multiple arguments possible
 * the count_args function has two main goals: first to count the number
 * of arguments seperated by whitespace in the string line, and second to
 * place null characters '\0' at the end of each argument, such that the
 * arg_parse function can easily distinguish the seperate arguments
 */
int count_args(char* line){
  char* countString = line;
  int argCount =0;
  bool firstSpace = false;

  while(*countString != '\0'){
  if(isspace(*countString) && firstSpace){
      argCount++;
      *countString = '\0';
      firstSpace = false;
    }
    else if(!isspace(*countString)){
      firstSpace = true;
    }
    countString++;
  }

  if(firstSpace){
    argCount++;
  }

  return argCount;
}
