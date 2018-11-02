#ifndef LIST
#define LIST
/* target list specifications */
//TODO: add target list specifications...
typedef struct target_st targets;
typedef targets* p_targets;

p_targets addtarget(p_targets*, char*);
void target_addrule(p_targets ptargets, char* rulestr);
void print_targets(p_targets);

/* rule list specifications */
typedef struct rule_st rules;
typedef rules* p_rules;

void addrule(p_rules*, char*);
void print_rules(p_rules);

void freerules(p_rules* );
void r_freerules(p_rules*);

#endif
