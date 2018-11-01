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

void append(p_list* thisList, char* string){
  p_list newNode = malloc(sizeof(list));

  newNode->data = strdup(string);
  newNode->next = NULL;

  while(*thisList != NULL) thisList = &((*thisList)->next);
  *thisList = newNode;
}

void print_list(list* thisList){

  while(thisList != NULL){
    printf("%s\n", thisList->data);
    thisList = thisList->next;
  }
}
void r_free_list(p_list* thisList){
  if((*thisList)->next != NULL) free_list(&(*thisList)->next);
  free((*thisList)->data);
  free(*thisList);
}

void free_list(p_list* thisList){
  r_free_list(thisList);
  *thisList = NULL;
}

void deleteList(p_list* head_ref) {
   /* deref head_ref to get the real head */
   p_list current = *head_ref;
   p_list next;

   while (current != NULL)
   {
       next = current->next;
       free(current->data);
       free(current);
       current = next;
   }

   /* deref head_ref to affect the real head back
      in the caller. */
   *head_ref = NULL;
}
