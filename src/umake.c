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

  //print_targets(targets);

  for(int i=1; i<argc; i++){
    execrules(targets, argv[i]);
  }

  free(line);

  return EXIT_SUCCESS;
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
