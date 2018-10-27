/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include "arg_parse.h"
#include <stdlib.h>
#include <stdio.h>

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
  char** targArgs;
  ruleList* targRules;
};

struct rule_st{
  ruleList* nextRule;
  char* rule;
};


void targpush(targetList* firstTarg, char* targLine){
  targetList* newTarg = malloc(sizeof(targetList));
  int argc;

  newTarg->nextTarg = firstTarg;
  newTarg->targArgs = arg_parse(targLine, &argc);
  newTarg->targRules = NULL;

  firstTarg = newTarg;
}

ruleList* new_ruleList(char* ruleLine){
  ruleList* newRules = malloc(sizeof(ruleList));

  newRules->nextRule = NULL;
  newRules->rule = ruleLine;

  return newRules;
}

ruleList* addrule(ruleList* firstRule, char* ruleLine){
  ruleList* lastRule = firstRule;

  while(lastRule->nextRule != NULL){
    lastRule = lastRule->nextRule;
  }

  lastRule->nextRule = new_ruleList(ruleLine);

  return lastRule->nextRule;
}

ruleList* getRuleList(targetList* target){
  return target->targRules;
}


targetList* nexttarget(targetList* original){
  return original->nextTarg;
}

void printRules(ruleList* rules){
  ruleList* thisNode = rules;
  while(thisNode->nextRule != NULL){
    printf("%s\n", thisNode->rule);
    thisNode = thisNode->nextRule;
  }
  printf("%s\n", thisNode->rule);
}

void printTargs(targetList* targs){
  targetList* thisTarg = targs;
  while(thisTarg->nextTarg != NULL){
    printf("%s\n", thisTarg->targArgs[0]);
    thisTarg = thisTarg->nextTarg;
  }
}
