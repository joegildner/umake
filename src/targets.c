/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include <stdlib.h>

struct target_st{
  target* nextTarg;
  rule* targRules;
};

struct rule_st{
  rule *nextRule;
  char* rule;
};

rule* target_addrule(target* firstTarg, char* ruleLine){
  rule* lastRule = firstTarg->targRules;

  while(lastRule->nextRule != NULL){
    lastRule = lastRule->nextRule;
  }

  return lastRule;
}

rule* rule_addrule(rule* firstRule, char* ruleLine);

target* add_target(target* firstTarg, char* targLine);
