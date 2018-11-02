/* CSCI 347: targets
* 25 OCT 2018, Joe Gildner
*/
#include "targets.h"
#include "arg_parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* How the data structure works
*  ______________      ______________
* | targetList   |    | targetList   |
* |    nextTarg--|--->|    nextTarg--|--->NULL
* |    targArgs  |    |     targArgs |
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
  p_targets nexttarget;
  p_rules targetrules;
  char* targetstr;
};

struct rule_st{
  p_rules nextrule;
  char* rulestr;
};

p_targets addtarget(p_targets* ptargets, char* targetstr){
  p_targets newnode = malloc(sizeof(targets));

  newnode->targetstr = strdup(targetstr);
  newnode->targetrules = NULL;
  newnode->nexttarget = NULL;

  while(*ptargets != NULL) ptargets = &((*ptargets)->nexttarget);
  *ptargets = newnode;

  return *ptargets;
}

void target_addrule(p_targets ptargets, char* rulestr){
  addrule(&(ptargets->targetrules),rulestr);
}

void print_targets(p_targets targets){
  while(targets != NULL){
    printf("%s:\n",targets->targetstr);
    print_rules(targets->targetrules);
    targets = targets->nexttarget;
  }
}



/* addrule
 * prules    pointer to a list
 * string   the given string to put in the list
 * addrule takes a pointer to a list and a string to add to that list and then
 * creates a new node with allocated memory and adds a copy of the string to that
 * node. addrule then finds the end of prules and adds the new node to it.
 */
void addrule(p_rules* prules, char* rulestr){
  p_rules newNode = malloc(sizeof(rules));

  newNode->rulestr = strdup(rulestr);
  newNode->nextrule = NULL;

  while(*prules != NULL) prules = &((*prules)->nextrule);
  *prules = newNode;
}

/* print_rules
 * thisList   pointer to a list node (not necessarily the first)
 * print_rules steps through each node in the linked list, from thisList onward,
 * and prints the data at each node until reaching the end
 */
void print_rules(p_rules list){
  while(list != NULL){
    printf("%s\n", list->rulestr);
    list = list->nextrule;
  }
}

/* free list
 * thisList    a pointer to a p_rules(which is a pointer to a linked list node)
 * This wrapper method takes a pointer to a linked list and calls the recursive
 * function r_freerules to free the list. Once the recursive function returns,
 * freerules then sets the given prules pointer to NULL to avoid undefined behavior
 * in the caller
 */
void freerules(p_rules* prules){
  if((*prules)!= NULL) r_freerules(prules);
  *prules = NULL;
}

/* r_freerules
 * thisList    a pointer to a p_rules(which is a pointer to a linked list)
 * r_freerules calls itself on every node in the linked list until it reaches
 * the null terminator at the end of the list. Then it frees all of the data
 * from each node it had just been called on
 */
void r_freerules(p_rules* prules){
  if((*prules)->nextrule != NULL) r_freerules(&(*prules)->nextrule);
  free((*prules)->rulestr);
  free(*prules);
}
