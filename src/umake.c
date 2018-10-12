/* CSCI 347 micro-make
 * 08 OCT 2018, Joe Gildner
 *
 * Forked from starter project written by
 * 09 AUG 2017, Aran Clauson
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <ctype.h>

/* CONSTANTS */


/* PROTOTYPES */

/* Process Line
 * line   The command line to execute.
 * This function interprets line as a command line.  It creates a new child
 * process to execute the line and waits for that process to complete.
 */
void processline(char* line);


/* arg parse
 * line   the command line to extract individual arguments from.
 * This function is used to convert a command line into an array of char pointers,
 * where each pointer is the beginning of an argument or flag.
 */
char** arg_parse(char* line);


/* Count args
 * line   command line to count the arguments in
 * the Count args function takes a command line argument and counts each
 * individual argument, where whitespace is the delimiter
 */
int count_args(char* line);


/* Main entry point.
 * argc    A count of command-line arguments
 * argv    The command-line argument valus
 *
 * Micro-make (umake) reads from the uMakefile in the current working
 * directory.  The file is read one line at a time.  Lines with a leading tab
 * character ('\t') are interpreted as a command and passed to processline minus
 * the leading tab.
 */
int main(int argc, const char* argv[]) {
  FILE* makefile = fopen("./uMakefile", "r");

  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);

  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    if(line[0] == '\t')
      processline(&line[1]);


    linelen = getline(&line, &bufsize, makefile);
  }

  free(line);

  return EXIT_SUCCESS;
}


/*
*/
void processline (char* line) {
  char** commandArgs = arg_parse(line); //Array containing command along with its arguments
  const pid_t cpid = fork();
  switch(cpid) {
    case -1: {
      perror("fork");
      break;
    }

    case 0: {
      execvp(commandArgs[0],commandArgs);
      perror("execvp");
      exit(EXIT_FAILURE);
      break;
    }

    default: {
      int   status;
      const pid_t pid = wait(&status);
      if(-1 == pid) {
        perror("wait");
      }
      else if (pid != cpid) {
        fprintf(stderr, "wait: expected process %d, but waited for process %d",
          cpid, pid);
      }
      free(commandArgs);
      break;
    }
  }
}

/* arg_parse
 * line   a command line, possibly with arguments included, delimited by whitespace
 * the arg parse function takes a command line and constructs an array of pointers to
 * strings, where the first string is the command and all sequential strings are
 * arguments for that command
 */
char** arg_parse(char* line){
  int argCount = count_args(line);
  char* placePointers = line; //pointer for parsing line

  //allocate enough memory to point to each argument, plus one to make the array
  //null terminated as required by execvp
  char** argArray = malloc((argCount+1) * sizeof(char*));

  for(int i=0; i<argCount; i++){
    //ignore whitespace between arguments
    while(isspace(*placePointers)){
      placePointers++;
    }
    argArray[i] = placePointers;
    //each argument in line is terminated by a '\0', si use strlen() to find the
    // distance to the end and skip to just after that point
    placePointers+=strlen(placePointers)+1;
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
  char* countString = line; //char pointer to parse line
  int argCount =0;
  bool firstSpace = false;

  while(*countString != '\0'){
    //case of interest: if countString is pointing to the first space after
    //a series of non space characters, then this must be the end of an argument
    if(isspace(*countString) && firstSpace){
      argCount++;
      *countString = '\0'; // seperate each argument by the null character
      firstSpace = false;
    }
    //Once we reach a non space after a period of spaces, we must be at a new
    //argument, set the firstSpace flag to true to catch the end of the argument
    else if(!isspace(*countString)){
      firstSpace = true;
    }
    countString++;
  }
  //final check: if the null character at the end of line came directly after an
  // argument, be sure to tally that last argument
  if(firstSpace){
    argCount++;
  }

  return argCount;
}
