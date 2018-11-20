/* targets.h
 * 9 NOV 2018, Joe Gildner
 */
#ifndef LIST 
#define LIST

// target list specifications 
typedef struct target_st targets;
typedef targets* p_targets;
typedef struct rule_st rules;
typedef rules* p_rules;

// rule list prototypes
p_targets addtarget(p_targets*, char*);
void target_addrule(p_targets ptargets, char* rulestr);
void addrule(p_rules*, char*);

// memory freeing prototypes
void freetargets(p_targets*);
void r_freetargets(p_targets*);
void freerules(p_rules* );
void r_freerules(p_rules*);

// execution prototypes
void processline(char* line);
void exectarget(p_targets, char*);
void execrules(p_rules);
p_targets findtarget(p_targets targetlist, char* findtarget);
int expand(char* orig, char* new, int newsize);
int modified(char* target, char* dependency);

// print prototypes
void print_targets(p_targets);
void print_rules(p_rules);

#endif
