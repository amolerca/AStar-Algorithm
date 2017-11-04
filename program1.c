#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "program1.h"



int main(){
	unsigned long nnodes = 3472620UL;
	node *nodes;
	if((nodes = (node *) malloc(nnodes*sizeof(node))) == NULL) ExitError("when allocating memory for the nodes vector", 5);
  
  FILE *fp;
  char *line = NULL;
  size_t len = 0; 
  ssize_t read;
   //fp = fopen("C:/Users/Daniel/Dropbox/Master/Optimization/Astar_Algorithm/AStar_Deliver/catalunya_test", "r");
  fp = fopen("/home/dsalgador/Dropbox/Master/Optimization/Astar_Algorithm/AStar_deliver2/catalunya.csv", "r");
  char delims[] = "|";


  if (fp == NULL){ 
    ExitError("when reading the file", 32);
    exit(EXIT_FAILURE);}


   int node_index = 0;
   char *ptr;

  while ((read = getline(&line, &len, fp)) != -1) {
        
        char * p    = strtok_single (line, delims);
        int count = 1;
        int name_size;

        //read node lines
        if(strcmp("node", p) == 0){ 
          while (p) {
                          
              if(count == 2){                 
                nodes[node_index].id = strtoul(p, &ptr,10);
                //printf("node id: %lu\n", nodes[node_index].id);

                }
              else if(count == 3){//node name
                //if( (nodes[node_index].name = (char *) malloc(strlen(p)*sizeof(char))) == NULL) ExitError("when allocating memory for the name of a node", 5);
                name_size = strlen(p);
                if( (nodes[node_index].name = (char *) malloc((name_size +1)*sizeof(char))) == NULL) ExitError("when allocating memory for the name of a node", 5);

                nodes[node_index].name = strcpy(nodes[node_index].name, p);
                //printf("node %i, name %s\n", node_index, nodes[node_index].name);
              
              }
              else if(count ==10){ //node lat
                nodes[node_index].lat = atof(p);
                //printf("node %i, lat: %lf\n", node_index, nodes[node_index].lat);
              }

              else if(count ==11){ //node lon
                nodes[node_index].lon = atof(p);
                //printf("node %i, lon: %.8lf\n", node_index, nodes[node_index].lon);
              }              

              //printf ("%s\n", *p ? p : "");
              p = strtok_single (NULL, delims);
              count++;
            }
          node_index++;

        }

        else if(strcmp("way", p) == 0){
          



        }       



   }

  free(line);
  exit(EXIT_SUCCESS);


	return 0;
}

void ExitError(char * message, int num){
     fprintf(stderr, "%s\n", message);
     exit(num);
}



char * strtok_single (char * str, char const * delims)
{
  //source: https://stackoverflow.com/questions/8705844/need-to-know-when-no-data-appears-between-two-token-separators-using-strtok
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
