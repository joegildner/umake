#ifndef LIST
#define LIST

typedef struct list_st list;
typedef list* p_list;

void append(p_list* thisList, char* string);
void print_list(list* thisList);

void free_list(p_list* thisList);
void r_free_list(p_list* thisList);
void deleteList(p_list* head_ref);
#endif
