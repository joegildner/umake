#ifndef LIST
#define LIST
/* target list specifications */
//TODO: add target list specifications...


/* rule list specifications */
typedef struct rule_st rules;
typedef rules* p_rules;

void addrule(p_rules* rulelist, char* string);
void print_rules(p_rules rulelist);

void freerules(p_rules* rulelist);
void r_freerules(p_rules* rulelist);

#endif
