#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "program1.h"


void ExitError(char * message, int num){
     fprintf(stderr, "%s", message);
     exit(num);
}



char * strtok_single (char * str, char const * delims)
{
  static char  * src = NULL;
  char  *  p,  * ret = 0;

  if (str != NULL)
    src = str;

  if (src == NULL)
    return NULL;

  if ((p = strpbrk (src, delims)) != NULL) {
    *p  = 0;
    ret = src;
    src = ++p;

  } else if (*src) {
    ret = src;
    src = NULL;
  }

  return ret;
}

int main(){
	unsigned long nnodes = 3472620UL;
	node *nodes;
	if((nodes = (node *) malloc(nnodes*sizeof(node))) == NULL) ExitError("when allocating memory for the nodes vector", 5);
    


    //printf("Hola que ase");
    FILE *fp;
    char *line = NULL;
    size_t len = 0; 
    ssize_t read;
   //fp = fopen("C:/Users/Daniel/Dropbox/Master/Optimization/Astar_Algorithm/AStar_Deliver/catalunya_test", "r");
   fp = fopen("/home/dsalgador/Dropbox/Master/Optimization/Astar_Algorithm/AStar_deliver2/catalunya_test.txt", "r");
   char delims[] = "|";


   if (fp == NULL){ 
    ExitError("when reading the file", 32);
    exit(EXIT_FAILURE);}

   while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);

        char * p    = strtok_single (line, delims);
        while (p) {
            printf ("%s\n", *p ? p : "");

            p = strtok_single (NULL, delims);
          }

        



    }

   free(line);
   exit(EXIT_SUCCESS);


	return 0;
}

