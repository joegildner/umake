/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include "arg_parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

/* How the data structure works
*  ______________      ______________
* | targetList   |    | targetList   |
* |    nextTarg--|--->|    nextTarg--|--->NULL
* |    targArgs  |    |     targArgs |
* |    targRules |    |     targRules|
* |_______|______|    |______ _|_____|
*         |                    |
*   ______V ____         ______V ____
*  | ruleList   |       | ruleList   |
*  |    rule    |       |     rule   |
*  |    nextRule|       |    nextRule|
*  |______|_____|       |______|_____|
*         |                    |
*        NULL                 NULL
*/

struct target_st{
  p_targets nexttarget;
  p_rules targetrules;
  char** targetdata;
};

struct rule_st{
  p_rules nextrule;
  char* rulestr;
};

/* addtarget
 *
 */
p_targets addtarget(p_targets* ptargets, char* targetstr){
  p_targets newnode = malloc(sizeof(targets));
  int argc;

  newnode->targetdata = arg_parse(strdup(targetstr),&argc);
  newnode->targetrules = NULL;
  newnode->nexttarget = NULL;

  while(*ptargets != NULL) ptargets = &((*ptargets)->nexttarget);
  *ptargets = newnode;

  return *ptargets;
}

void target_addrule(p_targets ptargets, char* rulestr){
  addrule(&(ptargets->targetrules),rulestr);
}


void print_targets(p_targets targets){
  while(targets != NULL){
    printf("%s\n",targets->targetdata[0]);
    print_rules(targets->targetrules);
    targets = targets->nexttarget;
  }
}

void execrules(p_targets targetlist, char* findtarget){
  while(targetlist != NULL){
    if(strcmp(findtarget,targetlist->targetdata[0])==0){
      break;
    }
    targetlist = targetlist->nexttarget;
  }

  if(targetlist != NULL){
    p_rules exrules = targetlist->targetrules;
    while(exrules!=NULL){
      processline(exrules->rulestr);
      exrules = exrules->nextrule;
    }
  }
}

/* addrule
 * prules    pointer to a list
 * string   the given string to put in the list
 * addrule takes a pointer to a list and a string to add to that list and then
 * creates a new node with allocated memory and adds a copy of the string to that
 * node. addrule then finds the end of prules and adds the new node to it.
 */
void addrule(p_rules* prules, char* rulestr){
  p_rules newNode = malloc(sizeof(rules));

  newNode->rulestr = strdup(rulestr);
  newNode->nextrule = NULL;

  while(*prules != NULL) prules = &((*prules)->nextrule);
  *prules = newNode;
}

/* print_rules
 * thisList   pointer to a list node (not necessarily the first)
 * print_rules steps through each node in the linked list, from thisList onward,
 * and prints the data at each node until reaching the end
 */
void print_rules(p_rules list){
  while(list != NULL){
    printf("%s\n", list->rulestr);
    list = list->nextrule;
  }
}

/* free list
 * thisList    a pointer to a p_rules(which is a pointer to a linked list node)
 * This wrapper method takes a pointer to a linked list and calls the recursive
 * function r_freerules to free the list. Once the recursive function returns,
 * freerules then sets the given prules pointer to NULL to avoid undefined behavior
 * in the caller
 */
void freerules(p_rules* prules){
  if((*prules)!= NULL) r_freerules(prules);
  *prules = NULL;
}

/* r_freerules
 * thisList    a pointer to a p_rules(which is a pointer to a linked list)
 * r_freerules calls itself on every node in the linked list until it reaches
 * the null terminator at the end of the list. Then it frees all of the data
 * from each node it had just been called on
 */
void r_freerules(p_rules* prules){
  if((*prules)->nextrule != NULL) r_freerules(&(*prules)->nextrule);
  free((*prules)->rulestr);
  free(*prules);
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
