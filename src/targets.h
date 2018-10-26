#ifndef TARGETS
#define TARGETS

typedef struct target_st target;
typedef struct rule_st rule;

target* addtarget(target* firsttarg, char* targline);

rule* target_addrule(target* target, char* ruleline);
rule* rule_addrule(rule* firstrule, char* ruleline);

target* nexttarget(target* targ);

#endif
