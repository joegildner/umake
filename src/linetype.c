#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int linetype(char* thischar);
int linetype_rule(char* thischar);
int linetype_targ(char* thischar);

enum{
	LINE_INVALID = -1,
	LINE_TARGET,
	LINE_RULE
};

int main(int argc, char* argv[]){
	/*
	FILE* makefile = fopen("./uMakefile", "r");

	size_t  bufsize = 0;
	char*   line    = NULL;
	ssize_t linelen = getline(&line, &bufsize, makefile);
	int type = 0;

	while(-1 != linelen) {

		if(line[linelen-1]=='\n') {
			linelen -= 1;
			line[linelen] = '\0';
		}

		linelen = getline(&line, &bufsize, makefile);

		type = linetype(line);

		printf("%d\n",type);
	}

	free(line);
	fclose(makefile);

	return EXIT_SUCCESS;
}

int linetype(char* thischar){
	if(*thischar == ' ')
		return linetype(thischar++);
	else if(*thischar == '\t')
		return linetype_rule(thischar++);
	else if(isalnum(*thischar))
		return linetype_targ(thischar++);
	else return LINE_INVALID;
}

int linetype_rule(char* thischar){
	if(isalnum(*thischar) || *thischar == ' ' || *thischar == '\t')
		return linetype_rule(thischar++);
	else if(*thischar == '\0')
		return LINE_RULE;
	else
		return LINE_INVALID;
}

int linetype_targ(char* thischar){
	if(isalnum(*thischar) || *thischar == ' ' || *thischar == '\t')
		return linetype_targ(thischar++);
	else if(*thischar == '\0')
		return LINE_TARGET;
	else
		return LINE_INVALID;
}
