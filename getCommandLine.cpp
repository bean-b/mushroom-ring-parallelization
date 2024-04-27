#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "getCommandLine.hpp"


void getArguments(int argc, char *argv[], int * dim, int * iterations, int * debug, int *verbose, int *numThreads)
{
  // arguments expected that have default values in the code: 
  //
  char *tvalue;  // number of threads
  char *dimvalue;      // number of rows, columns (square grid)
  char *iters_value; // number of iterations
    
  int c;        // result from getopt calls

  // flags indicating that command line flag was encountered
  //
  // debug gnplot results immediately (not yet implemented)
  int debug_flag = 0; 
  // for verbose printing output
  int verbose_flag = 0;

  double converted;   // for floating point threshold value

// for threads later
// while ((c = getopt (argc, argv, "t:m:n:i:p:dv")) != -1) { 
  while ((c = getopt (argc, argv, "m:i:t:dv")) != -1) {

    switch (c)
      {
      case 't':
        if (isNumber(optarg)) {
          tvalue = optarg;
          *numThreads = atoi(tvalue);
        } else {
          exitWithError(c, argv);
        }
       break;

      case 'm':
        if (isNumber(optarg)) {
          dimvalue = optarg;
          *dim = atoi(dimvalue);
        } else {
          exitWithError(c, argv);
        }
        break;
    
      case 'i':
        if (isNumber(optarg)) {
          iters_value = optarg;
          *iterations = atoi(iters_value);
        } else {
          exitWithError(c, argv);
        } 
        break;

      // case 'p':
      //   probThreshold_flag = 1;
      //   probThreshold_value = optarg;
      //   converted = strtod(probThreshold_value, NULL);
      //   if (converted != 0 ) {
      //     *thresh = converted;
      //   } else {
      //     exitWithError(c, argv);
      //   } 
      //   break;

      case 'd':
        debug_flag = 1;
        *debug = debug_flag;
        break;

      case 'v':
        verbose_flag = 1;
        *verbose = verbose_flag;
        break;

      case ':':
        printf("Missing arg for %c\n", optopt);
        Usage(argv[0]);
        exit(EXIT_FAILURE);
        break;

      case '?':
        if (
            (optopt == 'm') ||
            (optopt == 't') ||
            (optopt == 'i') ||
            (optopt == 'd') ||
            (optopt == 'v') 
           ) 
        {
          Usage(argv[0]);
          exit(EXIT_FAILURE);
        } else if (isprint (optopt)) {
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          Usage(argv[0]);
          exit(EXIT_FAILURE);
        } else {
          fprintf (stderr,
                   "Unknown non-printable option character `\\x%x'.\n",
                   optopt);
          Usage(argv[0]);
          exit(EXIT_FAILURE);
        }
        break;
      
      }
  }
}

int isNumber(char s[])
{
    for (int i = 0; s[i]!= '\0'; i++)
    {
        if (isdigit(s[i]) == 0)
              return 0;
    }
    
    return 1;
}

void exitWithError(char cmdFlag, char ** argv) {
  fprintf(stderr, "Option -%c needs a number value\n", cmdFlag);
  Usage(argv[0]);
  exit(EXIT_FAILURE);
}

void Usage(char *program) {

  fprintf(stderr, "Usage: %s [-m dim] [-i iterations] [-t numThreads] [-d] [-v]\n", program);
}
