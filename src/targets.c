/* CSCI 347: targets 
* 25 OCT 2018, Joe Gildner 
*/
#include "targets.h"
#include "arg_parse.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>

/* How the data structure works
*  ______________      ______________
* | target_st    |    | target_st    |
* |    nexttarget|--->|    nexttarget|--->NULL
* |    targetdata|    |    targetdata|
* |    targetrules    |    targetrules
* |_______|______|    |______ _|_____|
*         |                    |
*   ______V ____         ______V ____
*  | rule_st    |       | ruleList   |
*  |    rulestr |       |    rule    |
*  |    nextrule|       |    nextRule|
*  |______|_____|       |______|_____|
*         |                    |
*        NULL                 NULL
*/

struct target_st{
  p_targets nexttarget;
  p_rules targetrules;
  char** targetdata;
};

struct rule_st{
  p_rules nextrule;
  char* rulestr;
};

/*------------------------------------------------------------- 
  --------------- Target Builder functions ------------------*/

/* addtarget
 * ptargets   pointer to a list of targets, not necessarily the head
 * targetstr  string data to add to the new target node
 * addtarget constructs a new target node a
 */
p_targets addtarget(p_targets* ptargets, char* targetstr){
  p_targets newnode = malloc(sizeof(targets));
  int argc;

  newnode->targetdata = arg_parse(strdup(targetstr),&argc);
  newnode->targetrules = NULL;
  newnode->nexttarget = NULL;

  while(*ptargets != NULL) ptargets = &((*ptargets)->nexttarget);
  *ptargets = newnode;

  return *ptargets;
}

/*target_addrule
 *ptargets  pointer to a list of targets
 *target_addrule adds a rule to the end of the rulelist for the
 *given target.
 */
void target_addrule(p_targets ptargets, char* rulestr){
  addrule(&(ptargets->targetrules),rulestr);
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


/*------------------------------------------------------------- 
  --------------- Memory cleanup functions---------------------*/


/* free rules
 * rules    a pointer to a p_rules(which is a pointer to a linked list node)
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

/* free targets
 * ptargets   pointer to a targets data structure whose data is to be freed
 * freetargets is a wrapper method which calls the recursive method r_freetargets to 
 * free the memory in the structure. This method then clears the pointer
 */
void freetargets(p_targets* ptargets){
  if((*ptargets)!=NULL) r_freetargets(ptargets);
  *ptargets = NULL;
}

/* r_freetargets
 * ptargets   target structure to free
 * r_freetargets recursively calls itself on every target contained in the target structure and then
 * uses the recursive freerules function to free each rule list of each target befgore freeing the
 * data of each target
 */
void r_freetargets(p_targets* ptargets){
  if((*ptargets)->nexttarget!=NULL) r_freetargets(&(*ptargets)->nexttarget);
  freerules(&(*ptargets)->targetrules);
  free((*ptargets)->targetdata[0]);
  free((*ptargets)->targetdata);
  free(*ptargets);
}



/*------------------------------------------------------------- 
  --------------- Target Execution functions ------------------*/

/* execrules
 * targetlist   a pointer to a list of targets
 * findtarget   user-specified target we are looking for
 * execrules iterates through the targetlist until it finds a matching
 * target and then sequentially executes the rules associated with that
 * target
 */
void exectarget(p_targets targetlist, char* targstr){
  p_targets thistarget = findtarget(targetlist,targstr);

  if(thistarget != NULL){
    int index = 1;
    char** depends = thistarget->targetdata;
    while(depends[index]!=NULL){
      exectarget(targetlist, depends[index]);
      index++;
    }

    execrules(thistarget->targetrules);
  }
  else if(access(targstr, F_OK)){
    fprintf(stderr,"target not found: %s\n", targstr);
  }
}

void execrules(p_rules rulelist){
  if(rulelist != NULL){
    processline(rulelist->rulestr);
    execrules(rulelist->nextrule);
  }
}

/* findtarget
 * targetlist   head of the target structure in question
 * targstr      name of the target which we are looking for in the targetlist
 * findtarget iterates through each target in the given target list until it finds
 * targstr. If the desired target is not in the targetlist findtarget returns NULL
 */
p_targets findtarget(p_targets targetlist, char* targstr){
  while(targetlist != NULL){
    if(strcmp(targstr,targetlist->targetdata[0])==0)
      break;
    targetlist = targetlist->nexttarget;
  }
  return targetlist;
}

/* Process line
 * line    command line to extract command and arguments from
 * This function takes a full command line, with multiple possible arguments
 * and flags, then executes the command in a child process. If line is empty
 * then process line does nothing with it
 */
void processline (char* line) {
  int argc = 0;
  int expandsize = 1024;
  char expandline[expandsize];

  if(!expand(line, expandline, expandsize))
    fprintf(stderr, "unable to expand environment variable, check syntax");

  char** commandArgs = arg_parse(expandline, &argc);

  if(argc>0){
    const pid_t cpid = fork();
    switch(cpid) {

      case -1: {
        perror("fork");
        break;
      }

      case 0: {

        execvp(commandArgs[0],commandArgs);
        perror("execvp");
        exit(EXIT_FAILURE);
        break;
      }

      default: {
        int   status;
        const pid_t pid = wait(&status);
        if(-1 == pid) {
          perror("wait");
        }
        else if (pid != cpid) {
          fprintf(stderr, "wait: expected process %d, but waited for process %d",
          cpid, pid);
        }
        break;
      }
    }
  }
  free(commandArgs);
}

/* Expand
 * orig    The input string that may contain variables to be expanded
 * new     An output buffer that will contain a copy of orig with all
 *         variables expanded
 * newsize The size of the buffer pointed to by new.
 * returns 1 upon success or 0 upon failure.
 */
 int expand(char* orig, char* new, int newsize){
   char* varstart = NULL;

   while(*orig!='\0'){
     if(varstart != NULL){
       if(*orig == '}'){
         *orig = '\0';
         sprintf(new, "%s", getenv(varstart));
         new+=strlen(new);
         varstart = NULL;
       }
       else if(*orig == '{') {
         fprintf(stderr, "Unable to expand, mismatched \'{\'\n");
         break;
       }
     }
     else{
       if(*orig == '$' && orig[1] == '{' && varstart == NULL){
         varstart = orig+2;
         orig += 2;
       }else{
         *new = *orig;
         new[1] = '\0';
         new++;
       }
     }
     orig++;
   }

   return (varstart == NULL);

 }


/*------------------------------------------------------------- 
  --------------- print functions -----------------------------*/

/* print_targets
 * targets
 * iterates through the target list and then
 */
void print_targets(p_targets targets){
  while(targets != NULL){
    int index = 1;
    char** depends = targets->targetdata;
    while(depends[index]!=NULL){
      printf("%s ",depends[index]);
      index++;
    }
    printf("\n");
    print_rules(targets->targetrules);
    targets = targets->nexttarget;
  }
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