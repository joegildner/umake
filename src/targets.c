/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include <stdlib.h>
#include <stdio.h>

/* How the data structure works
 *  ______________      ______________
 * | targetList   |    | targetList   |
 * |    nextTarg--|--->|    nextTarg--|--->NULL
 * |    targStr   |    |     targStr  |
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
  char* targStr;
  ruleList* targRules;
};

struct rule_st{
  ruleList* nextRule;
  char* rule;
};

targetList* new_targetList(char* targLine){
  targetList* newTarg = malloc(sizeof(targetList));

  newTarg->nextTarg = NULL;
  newTarg->targStr = targLine;
  newTarg->targRules = NULL;

  return newTarg;
}

ruleList* new_ruleList(char* ruleLine){
  ruleList* newRules = malloc(sizeof(ruleList));

  newRules->nextRule = NULL;
  newRules->rule = ruleLine;

  return newRules;
}

ruleList* t_addrule(targetList* firstTarg, char* ruleLine){
  ruleList* lastRule = firstTarg->targRules;

  while(lastRule->nextRule != NULL){
    lastRule = lastRule->nextRule;
  }

  lastRule->nextRule = new_ruleList(ruleLine);

  return lastRule;
}

ruleList* addrule(ruleList* firstRule, char* ruleLine){
  ruleList* lastRule = firstRule;

  while(lastRule->nextRule != NULL){
    lastRule = lastRule->nextRule;
  }

  lastRule->nextRule = new_ruleList(ruleLine);

  return lastRule;
}

targetList* add_target(targetList* firstTarg, char* targLine){
  return NULL;
}

void printRules(ruleList* rules){
  ruleList* thisNode = rules;
  while(thisNode->nextRule != NULL){
    printf("%s\n", thisNode->rule);
    thisNode = thisNode->nextRule;
  }
  printf("%s\n", thisNode->rule);
}
