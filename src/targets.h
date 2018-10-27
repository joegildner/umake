#ifndef TARGETS
#define TARGETS

typedef struct target_st targetList;
typedef struct rule_st ruleList;

ruleList* new_ruleList(char* ruleLine);
targetList* new_targetList();

void targpush(targetList** firstTarg, char* targLine);
void addtarget(targetList** head, char* ruleLine);

void t_addrule(targetList* target, char* ruleLine);
void addrule(ruleList** target, char* ruleLine);

ruleList* getRuleList(targetList* thisTarget);
targetList* nexttarget(targetList* targ);

void findtarget(char* argument);
void execrules(char* argument);

void printRules(ruleList* rules);
void printTargs(targetList* targs);
#endif
