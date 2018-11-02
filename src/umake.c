/* CSCI 347 micro-make
* 08 OCT 2018, Joe Gildner
*
* Forked from starter project written by
* 09 AUG 2017, Aran Clauson
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "arg_parse.h"
#include "targets.h"

/* CONSTANTS */


/* PROTOTYPES */

//void processrules(ruleList* rules);

int getlinetype(char* line);

/* Process Line
* line   The command line to execute.
* This function interprets line as a command line.  It creates a new child
* process to execute the line and waits for that process to complete.
*/
void processline(char* line);

/* Main entry point.
* argc    A count of command-line arguments
* argv    The command-line argument valus
*
* Micro-make (umake) reads from the uMakefile in the current working
* directory.  The file is read one line at a time.  Lines with a leading tab
* character ('\t') are interpreted as a command and passed to processline minus
* the leading tab.
*/
int main(int argc, char* argv[]) {
  FILE* makefile = fopen("./uMakefile", "r");

  size_t  bufsize = 0;
  char*   line    = NULL;
  ssize_t linelen = getline(&line, &bufsize, makefile);

  p_targets targets = NULL;
  p_targets currtarget = NULL;

  while(-1 != linelen) {

    if(line[linelen-1]=='\n') {
      linelen -= 1;
      line[linelen] = '\0';
    }

    int lineType = getlinetype(line);
    if(lineType == 0){
      currtarget = addtarget(&targets,line);
    }
    else if(lineType == 1){
      target_addrule(currtarget,line);
    }

    linelen = getline(&line, &bufsize, makefile);
  }

  print_targets(targets);

  free(line);

  return EXIT_SUCCESS;
}


/* Process line
 * line    command line to extract command and arguments from
 * This function takes a full command line, with multiple possible arguments
 * and flags, then executes the command in a child process. If line is empty
 * then process line does nothing with it
 */
void processline (char* line) {
  int argc;
  char** commandArgs = arg_parse(line, &argc);

  if(argc>0){
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
        break;
      }
    }
  }
  free(commandArgs);
}

/* get line type
 * line   makefile line
 * get line type determines whether we are looking at a target, a rule,
 * or just a blank line. Skips all leading whitespace
 * 0 for target
 * 1 for rule
 * 2 for other
 */
int getlinetype(char* line){
  int lineType = 2;

  while(line[0] == ' '){
    line++;
  }

  if(line[0] == '\t') lineType = 1;
  else if(isalpha(line[0])) lineType = 0;
  else lineType =2;

  return lineType;

  }
