/* linetype.c 
 * 9 NOV 2018, Joe Gildner
 */

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "linetype.h"

char validchars[12] = {'-','$','{','}','_','-','.',':',' ','=','/','*'};

/*
        start     space
        +-----+  +--+
  	      	  |  |  |
  	       	 +v--+--v-+
	  	     |        |                         valid
    '\t'     |   q0   |   valid               +--+
       +----+|        |+-------------------+  |  |
       |     +--------+                  +-v--+--v+
       |                                 |        |
       |                            ':'  |   q1   | '='
       |                         +-------+        +------+
       |                         |       +----+---+      |
       |                         |            |          |
       |                         |           |else       |
       |                         |          |            |
val +--v-----+               +-v------+     |       +--v-----+
+--->        |            +-->        |     |       |        <--+
|   |  RULE  |       valid|  | TARGET |     |       |  VAR   |  |valid
+---+        |            +--+        |     |       |        +--+
    +---+----+               +---+----+     |       +-----+--+
        |                        |          |             |
        |else                    |else      |             |else
        |                        |          |             |
        |                   +----v-----+    |             |
        |                   |          <----+             |
        +------------------->  invalid <------------------+
				        	|          |
						    +----------+
*/

/* linetype
 * thischar 	current character in line
 * the following give functions represent states on the above statemachine, except invalid.
 * the functions call eachother based on given input and follow the edges of the state machine
 */
int linetype(char* thischar){
	if(*thischar == ' ')
		return linetype(++thischar);
	else if(*thischar == '\t')
		return linetype_rule(++thischar);
	else if(isalnum(*thischar))
		return linetype_q1(++thischar);
	else return LINE_INVALID;
}

int linetype_q1(char* thischar){
	if(*thischar == ':')
		return linetype_targ(++thischar);
	else if(*thischar == '=')
		return linetype_var(++thischar);
	else if(isvalidchar(*thischar))
			return linetype_q1(++thischar);
	else return LINE_INVALID;
}

int linetype_targ(char* thischar){
	if(isvalidchar(*thischar))
		return linetype_targ(++thischar);
	else if(*thischar == '#')
		*thischar = '\0';
		return LINE_TARGET;
	else if(*thischar == '\0')
		return LINE_TARGET;
	else
		return LINE_INVALID;
}

int linetype_rule(char* thischar){
	if(isvalidchar(*thischar))
		return linetype_rule(++thischar);
	else if(*thischar == '#')
		*thischar = '\0';
		return LINE_RULE;
	else if(*thischar == '\0')
		return LINE_RULE;
	else
		return LINE_INVALID;
}


int linetype_var(char* thischar){
	if(isvalidchar(*thischar))
		return linetype_var(++thischar);
	else if(*thischar == '#')
		*thischar = '\0';
		return LINE_VAR;
	else if(*thischar == '\0')
		return LINE_VAR;
	else
		return LINE_INVALID;
}


/* isvalidchar
 * thischar 	character of interest
 * This function checks whether thischar belongs to the array of valid
 * characters (validchars) above or if it is alphanumeric, and returns true if either
 */
bool isvalidchar(char thischar){
	bool contains = false;
	for(int i=0; i<sizeof(validchars)/sizeof(char); i++){
		if(validchars[i] == thischar) contains = true;
	}
	return (contains || isalnum(thischar));
}
