#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* arg parse
 * line   the command line to extract individual arguments from.
 * This function is used to convert a command line into an array of char pointers,
 * where each pointer is the beginning of an argument or flag.
 */
char** arg_parse(char* line);


/* Count args
 * line   command line to count the arguments in
 * the Count args function takes a command line argument and counts each
 * individual argument, where whitespace is the delimiter
 */
int count_args(char* line);
