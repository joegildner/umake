#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>

int linetype(char* thischar);
int linetype_rule(char* thischar);
int linetype_targ(char* thischar);
void printchars(char* chars);
bool validchar(char thischar, int charset);

enum{
	LINE_INVALID = -1,
	LINE_TARGET,
	LINE_RULE
};

char rulechars[4] = {'-','.','\t',' '};
char targetchars[5] = {'-','.','\t',' ',':'};


int main(int argc, char* argv[]){
	FILE* makefile = fopen("./uMakefile", "r");

	size_t  bufsize = 0;
	char*   line    = NULL;
	ssize_t linelen = getline(&line, &bufsize, makefile);
	//int type = 0;

	while(-1 != linelen) {

		if(line[linelen-1]=='\n') {
			linelen -= 1;
			line[linelen] = '\0';
		}



		printf("%d\n", linetype(line));
		//printchars(line);
		linelen = getline(&line, &bufsize, makefile);

	}
	printf("\n%d\n", validchar(':',LINE_TARGET));

	free(line);
	fclose(makefile);

	return EXIT_SUCCESS;
}

int linetype(char* thischar){
	if(*thischar == ' ')
		return linetype(++thischar);
	else if(*thischar == '\t')
		return linetype_rule(++thischar);
	else if(isalnum(*thischar))
		return linetype_targ(++thischar);
	else return LINE_INVALID;
}

int linetype_rule(char* thischar){
	if(isalnum(*thischar) || *thischar == ' ' || *thischar == '\t' || *thischar == ':' || *thischar == '.')
		return linetype_rule(++thischar);
	else if(*thischar == '\0')
		return LINE_RULE;
	else
		return LINE_RULE;
}

int linetype_targ(char* thischar){
	if(isalnum(*thischar) || *thischar == ' ' || *thischar == '\t' || *thischar == ':' || *thischar == '.')
		return linetype_targ(++thischar);
	else if(*thischar == '\0')
		return LINE_TARGET;
	else{
		//printf("",*thischar);
		return LINE_TARGET;
	}
}

bool validchar(char thischar, int charset){
	char* validchars;
	switch(charset){
		case LINE_TARGET:
			validchars = targetchars;
			break;
		case LINE_RULE:
			validchars = rulechars;
			break;
		default:
			perror("validchar: invalid charset");
			break;
	}

	bool contains = false;
	for(int i=0; i<sizeof(validchars)/sizeof(char); i++){
		if(validchars[i] == thischar) contains = true;
	}
	return contains;
}

void printchars(char* chars){
	if(*chars != '\0'){
		printf("%c",*chars);
		//chars++;
		printchars(++chars);
	}else{
		printf("done\n");
	}
}
