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
   fp = fopen("/home/dsalgador/Dropbox/Master/Optimization/Astar_Algorithm/AStar_Deliver/catalunya_test.txt", "r");

   if (fp == NULL){ 
    ExitError("when reading the file", 32);
    exit(EXIT_FAILURE);}

   while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
    }

   free(line);
   exit(EXIT_SUCCESS);


	return 0;
}

