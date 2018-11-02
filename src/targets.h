#ifndef LIST
#define LIST
/* target list specifications */
typedef struct target_st targets;
typedef targets* p_targets;
typedef struct rule_st rules;
typedef rules* p_rules;

p_targets addtarget(p_targets*, char*);
void target_addrule(p_targets ptargets, char* rulestr);
void print_targets(p_targets);
void execrules(p_targets, char*);
void freetargets(p_targets*);
void r_freetargets(p_targets*);

/* rule list specifications */

void addrule(p_rules*, char*);
void print_rules(p_rules);
void freerules(p_rules* );
void r_freerules(p_rules*);

/* Process Line
* line   The command line to execute.
* This function interprets line as a command line.  It creates a new child
* process to execute the line and waits for that process to complete.
*/
void processline(char* line);

#endif
