/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include "arg_parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
  targetList* nextTarg;
  char* targArgs;
  ruleList* targRules;
};

struct rule_st{
  ruleList* nextRule;
  char* rule;
};

void targpush(targetList** firstTarg, char* targLine){
  targetList* newTarg = malloc(sizeof(targetList));
  int argc;

  newTarg->nextTarg = *firstTarg;
  char** targArgs = arg_parse(targLine,&argc);
  newTarg->targArgs = malloc((argc+1) * sizeof(char*));
  memcpy(newTarg->targArgs, targArgs, (argc+1) * sizeof(char*));
  strcpy(newTarg->targArgs,targLine);
  newTarg->targRules = NULL;

  *firstTarg = newTarg;
}

void t_addrule(targetList* target, char* ruleLine){
  addrule(&target->targRules,ruleLine);
}

void addrule(ruleList** head, char* ruleLine){
  ruleList* next = malloc(sizeof(ruleList));
  next->nextRule = NULL;
  next->rule = malloc(strlen(ruleLine)*sizeof(char*));
  strcpy(next->rule,ruleLine);

  if(*head == NULL){
    *head = next;
  }else{
    ruleList* thisNode = *head;
    while(thisNode->nextRule != NULL){
      thisNode = thisNode->nextRule;
    }
    thisNode->nextRule = next;
  }
}

ruleList* getRuleList(targetList* target){
  return target->targRules;
}


targetList* nexttarget(targetList* original){
  return original->nextTarg;
}


void findtargetrules(char* argument, targetList* targets, ruleList** rules){
  targetList thisTarget = NULL;
  while(targets != NULL){
    if(strcmp(argument,targets->targArgs)==0){
      thisTarget = targets;
    }
  }
  *rules = thisTarget->targRules;
}

void printRules(ruleList* rules){
  ruleList* thisRule = rules;
  while(thisRule->nextRule != NULL){
    printf("%s\n", thisRule->rule);
    thisRule = thisRule->nextRule;
  }
  printf("%s\n", thisRule->rule);
}

void printTargs(targetList* targs){
  while(targs!= NULL){
    printf("%s\n", targs->targArgs);
    printRules(targs->targRules);
    targs = targs->nextTarg;
  }
}
