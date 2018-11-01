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

struct list_st{
  list* next;
  char* data;
};

/* append
 * plist    pointer to a list
 * string   the given string to put in the list
 * append takes a pointer to a list and a string to add to that list and then
 * creates a new node with allocated memory and adds a copy of the string to that
 * node. Append then finds the end of plist and adds the new node to it.
 */
void append(p_list* plist, char* string){
  p_list newNode = malloc(sizeof(list));

  newNode->data = strdup(string);
  newNode->next = NULL;

  while(*plist != NULL) plist = &((*plist)->next);
  *plist = newNode;
}

/* print_list
 * thisList   pointer to a list node (not necessarily the first)
 * print_list steps through each node in the linked list, from thisList onward,
 * and prints the data at each node until reaching the end
 */
void print_list(list* thisList){

  while(thisList != NULL){
    printf("%s\n", thisList->data);
    thisList = thisList->next;
  }
}

/* free list
 * thisList    a pointer to a p_list(which is a pointer to a linked list node)
 * This wrapper method takes a pointer to a linked list and calls the recursive
 * function r_free_list to free the list. Once the recursive function returns,
 * free_list then sets the given
 */
void free_list(p_list* plist){
  r_free_list(plist);
  *plist = NULL;
}

/* r_free_list
 * thisList    a pointer to a p_list(which is a pointer to a linked list)
 * r_free_list calls itself on every node in the linked list until it reaches
 * the null terminator at the end of the list. Then it frees all of the data
 * from each node it had just been called on
 */
void r_free_list(p_list* plist){
  if((*plist)->next != NULL) free_list(&(*plist)->next);
  free((*plist)->data);
  free(*plist);
}
