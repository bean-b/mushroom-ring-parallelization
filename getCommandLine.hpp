// functions in getCommandLine.c file
void getArguments(int argc, char *argv[], int * dim, int * iterations, int * debug, int *verbose, int *numThreads);
int isNumber(char s[]);
void exitWithError(char cmdFlag, char ** argv);
void Usage(char *program);