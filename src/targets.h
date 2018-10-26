#ifndef TARGETS
#define TARGETS

typedef struct target_st targetList;
typedef struct rule_st ruleList;

ruleList* new_ruleList(char* ruleLine);
targetList* new_targetList(char* ruleLine);

targetList* addtarget(targetList* firstTarg, char* targLine);

ruleList* t_addrule(targetList* firstTarg, char* ruleLine);
ruleList* addrule(ruleList* firstrule, char* ruleLine);

targetList* nexttarget(targetList* targ);

void printRules(ruleList* rules);

#endif
