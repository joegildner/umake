/* arg_parse 
 * 23 OCT 2018, Joe Gildner
 */

#include <stdbool.h>
#include <stdlib.h> 
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "arg_parse.h"

/* arg_parse
 * line   a command line, possibly with arguments included, delimited by whitespace
 * the arg parse function takes a command line and constructs an array of pointers to
 * strings, where the first string is the command and all sequential strings are
 * arguments for that command
 */
char** arg_parse(char* line, int *argcp){
  *argcp = count_args(line);
  char** argArray = malloc((*argcp+1) * sizeof(char*));

  for(int i=0; i<*argcp; i++){
    while(isspace(*line)){
      line++;
    }
    argArray[i] = line;
    argArray[i+1] = NULL;
    line+=strlen(line)+1;
  }
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
  int argCount =0;
  bool firstSpace = false;

  while(*line != '\0'){
    if(isspace(*line) && firstSpace){
      argCount++;
      *line = '\0';
      firstSpace = false;
    }else if(*line == ':'){
      *line = ' ';
      line--;
    }
    else if(!isspace(*line)){
      firstSpace = true;
    }
    line++;
  }

  if(firstSpace){
    argCount++;
  }

  return argCount;
}
