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
#include <string.h>

#include "arg_parse.h"
#include "targets.h"

/* CONSTANTS */

/* PROTOTYPES */

//void processrules(ruleList* rules);

int getlinetype(char* line);

int addenvvar(char* line);

int linetype(char* thischar);
int linetype_q1(char* thischar);
int linetype_rule(char* thischar);
int linetype_targ(char* thischar);
int linetype_var(char* thischar);

enum{
  LINE_INVALID = -1,
  LINE_TARGET,
  LINE_RULE,
  LINE_VAR
};

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
  if(makefile == NULL){
    perror("Unable to find uMakefile");
    exit(1);
  }

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

    switch(linetype(line)){
      case LINE_TARGET:
        currtarget = addtarget(&targets,line);
        break;
      case LINE_RULE:
        target_addrule(currtarget,line);
        break;
      case LINE_VAR:
        if(addenvvar(line)) perror("setenv");
        break;

    }

    linelen = getline(&line, &bufsize, makefile);
  }

  for(int i=1; i<argc; i++){
    exectarget(targets, argv[i]);
  }

  free(line);
  freetargets(&targets);
  fclose(makefile);


  return EXIT_SUCCESS;
}

/* addenvvvar
 * line   new variable assignment to add to the environment
 * addenvvar takes a string variable assignment of the form x = y and adds it to
 * the current environment
 */
int addenvvar(char* line){
  char name[32];
  int iname = 0;
  char val[32];
  int ival = 0;

  while(*line != '='){
    if(*line != ' '){
      name[iname] = *line;
      name[iname+1] = '\0';
      iname++;
    }
    line++;
  }
  line++;

  bool leadspace = (*line == ' ');
  while(*line != '\0'){
    if(*line == ' ' && leadspace)
      line++;
    else if(*line != ' ' && leadspace)
      leadspace = false;
    else{
      val[ival] = *line;
      val[ival+1] = '\0';
      ival++;
      line++;
    }
  }

  return setenv(name, val, 0);
}

/* linetype
 * thischar   current character in line
 * the following give functions represent states on the above statemachine, except invalid.
 * the functions call eachother based on given input and follow the edges of the state machine
 */
int linetype(char* thischar){
  if(*thischar == ' ')
    return linetype(++thischar);
  else if(*thischar == '\t')
    return linetype_rule(++thischar);
  else if(isalnum(*thischar))
    return linetype_q1(++thischar);
  else return LINE_INVALID;
}

int linetype_q1(char* thischar){
  if(*thischar == ':')
    return linetype_targ(++thischar);
  else if(*thischar == '=')
    return linetype_var(++thischar);
  else 
    return linetype_q1(++thischar);
}

int linetype_targ(char* thischar){
  if(*thischar == '#'){
    *thischar = '\0';
    return LINE_TARGET;
  }
  else if(*thischar == '\0')
    return LINE_TARGET;
  else
    return linetype_targ(++thischar);
}

int linetype_rule(char* thischar){
  if(*thischar == '#'){
    *thischar = '\0';
    return LINE_RULE;
  }
  else if(*thischar == '\0')
    return LINE_RULE;
  else
    return linetype_rule(++thischar);
}


int linetype_var(char* thischar){
  if(*thischar == '#'){
    *thischar = '\0';
    return LINE_VAR;
  }
  else if(*thischar == '\0')
    return LINE_VAR;
  else
    return linetype_var(++thischar);
}