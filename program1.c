//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "program1.h"

int IDtoPOSITION(node * Vector, int Vlen, unsigned long id){
  int imin=0,imax= Vlen-1;
  int imid;
  while(imax >= imin){
    imid = (int) ( (imin+imax)/2.0 );
    if(Vector[imid].id == id) return imid;
    else if(Vector[imid].id < id) imin = imid+1;
    else imax = imid-1;

  } 

  return -1;

}

unsigned long POSITIONtoID(node * Vector, int position){  

  return Vector[position].id;

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
  fp = fopen("/home/dsalgador/Dropbox/Master/Optimization/Astar_Algorithm/AStar_deliver2/catalunya.csv", "r");
  char delims[] = "|";


   if (fp == NULL){ 
    ExitError("when reading the file", 32);
    exit(EXIT_FAILURE);}


   int node_index = 0;
   char *ptr;

   unsigned short nsuccdim[nnodes];

   while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);

        char * p    = strtok_single (line, delims);
        int count = 1;
        int name_size;
        _Bool is_oneway = 0; //initialised to false

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
/*
        else if(strcmp("way", p) == 0){

          
              ///Codi o funció que faci tot lo de enmagatzemar successors
          //això és el 50% de la feina que faltaria, que és la part chunga
          while (p) {
             //if this field is empty --> twoways,if there is "oneway" --> oneway
             if(count == 7 & strcmp("oneway",p) == 0) is_oneway = 1;




                

             











            p = strtok_single (NULL, delims);
            count++;




         }//endwhile p of ways       


        



       }//end elseif (ways)*/


   }

   //testing IDtoPOSITION:
   //unsigned long id = 8670491;
   unsigned long id = 8670500;
   printf("\nThe position of the id %lu is %d \n ", id, IDtoPOSITION(nodes, nnodes, id) );
   int pos = 8;
   printf("\nThe id of the position %d is %lu \n ", pos, POSITIONtoID(nodes, pos) );




   free(line);
   exit(EXIT_SUCCESS);


	return 0;
}//endmain




void ExitError(char * message, int num){
     fprintf(stderr, "%s\n", message);
     exit(num);
}



char * strtok_single(char * str, char const * delims)
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
