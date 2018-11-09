#ifndef LNTYPE
#define LNTYPE
/* State machine functions */
int linetype(char* thischar);
int linetype_q1(char* thischar);
int linetype_rule(char* thischar);
int linetype_targ(char* thischar);
int linetype_var(char* thischar);

/* language checker */
bool isvalidchar(char thischar);

enum{
	LINE_INVALID = -1,
	LINE_TARGET,
	LINE_RULE,
	LINE_VAR
};

#endif
