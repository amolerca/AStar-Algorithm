/*****************************************************************************
 *                                                                           *
 *                        <Routing - AStar Algorithm>                        *
 *             Copyright (C) <2018>   <Municoy, M., Salgado, D.>             *
 *                                                                           *
 *   Contact the authors at: mail@martimunicoy.com                           *
 *                           daniel.salgado@e-campus.uab.cat                 *
 *                                                                           *
 *   This program is free software: you can redistribute it and/or modify    *
 *   it under the terms of the GNU General Public License as published by    *
 *   the Free Software Foundation, either version 3 of the License, or       *
 *   (at your option) any later version.                                     *
 *                                                                           *
 *   This program is distributed in the hope that it will be useful,         *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of          *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           *
 *   GNU General Public License for more details.                            *
 *                                                                           *
 *****************************************************************************/

// Include Libraries
//#define _GNU_SOURCE
#include "routing.h"
#include "constants.h"
#include "util.h"
#include "graph.h"
#include "reader.h"


/*
unsigned long IDtoPOSITION(node * Vector, unsigned long Vlen, unsigned long id){
    unsigned long imin=0,imax= Vlen-1;
    unsigned long imid;
    while(imax >= imin){
        imid = (int) ( (imin+imax)/2.0 );
        if(Vector[imid].id == id) return imid;
        else if(Vector[imid].id < id) imin = imid+1;
        else imax = imid-1;

  }

  return -1; //if the id does not exist on de Vector of nodes

}

unsigned long POSITIONtoID(node * Vector, unsigned long position){

  return Vector[position].id;

}*/


int main()
{

    Node *nodes;
    nodes = (Node *) malloc(NNODES*sizeof(Node));
    if(nodes == NULL)
        ExitError("when allocating memory to the nodes vector", 5);

      //printf("Hola que ase");

    ReadFile("inputs/spain.map");




/*
   unsigned long node_index = 0;
   char *ptr;

   unsigned short nsuccdim[nnodes];
   void * newpointer;

   while ((read = getline(&line, &len, fp)) != -1) {
        //printf("Retrieved line of length %zu :\n", read);
        //printf("%s", line);

        char * p    = strtok_single (line, INPUT_FIELD_DELIMITORS);
        int count = 1; //to unsigned long??
        int name_size; //to unsigned long¿
        _Bool is_oneway = 0; //initialised to false

        //read node lines
        if(strcmp("node", p) == 0){
          nsuccdim[node_index] = 0; //Initialise the number of successors of the current node to 0
          //nodes[node_index].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
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

            //RESERVAR MEMÒRIA PELS  *successors???
          node_index++;

        }

        else if(strcmp("way", p) == 0){

            unsigned long idA, idB;
            unsigned long posA, posB;
            unsigned long way_nodes = 0;
            unsigned long way_id; // to record de way id
            _Bool nonvalidnodefound = 0;



              ///Codi o funció que faci tot lo de enmagatzemar successors
          //això és el 50% de la feina que faltaria, que és la part chunga
          while (p) {
             if(count == 2){way_id = strtoul(p, &ptr,10);}
             //if this field is empty --> twoways,if there is "oneway" --> oneway
             if( (count == 8) & (strcmp("oneway",p) == 0) ) is_oneway = 1;

             //   |A|B|C|D|.... IDS

             else if(count >= 10){
                if(count % 2 == 0){
                  idA =  strtoul(p, &ptr,10); //nodes[way_nodes].id;
                  if(  (posA = IDtoPOSITION(nodes, nnodes, idA)) == -1  ){
                    //printf("Found a way with nonvalid nodes\n");
                    //exit(0);
                    //                    break;
                    count--;
                    nonvalidnodefound = 1;
                  }
                  else{
                    way_nodes += 1;
                    nonvalidnodefound = 0;
                  }
                }
                else{
                  idB = strtoul(p, &ptr,10);
                  if(  (posB = IDtoPOSITION(nodes, nnodes, idB)) == -1  ){
                    //printf("FOund a way with nonvalid nodes\n");
                    //exit(0);
                    //break;
                    count--;
                    nonvalidnodefound = 1;
                  }
                  else{way_nodes += 1;
                                      nonvalidnodefound = 0;}
                  //posB = IDtoPOSITION(nodes, nnodes, idB);

                }



                //way_nodes += 1;
             }


             if(!nonvalidnodefound){



               if( (count >= 10) & (way_nodes % 2 == 0) & (way_nodes >=2)  ){
                  if(!is_oneway){

                    if(nsuccdim[posA] == 0){
                        nodes[posA].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                        ++nsuccdim[posA];
                                              }

                    else {
                      ++nsuccdim[posA];
                      if( (nodes[posA].successors = (unsigned long *) realloc(nodes[posA].successors, nsuccdim[posA]*sizeof(unsigned long)))  == NULL){
                        printf("Error when allocating memory for the successors of node posA = %lu\n",posA);
                        exit(0);
                      }
                                          }

                    if(nsuccdim[posB] == 0){
                      nodes[posB].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                      ++nsuccdim[posB];
                      }

                    else{
                      ++nsuccdim[posB];
                      //nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long));
                      if( (nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long)) ) == NULL){
                      printf("Error when allocating memory for the successors of node posB = %lu\n",posB);
                        exit(0);

                      }

                    }

                    nodes[posA].successors[nsuccdim[posA]-1] = idB;
                    nodes[posB].successors[nsuccdim[posB]-1] = idA;



                  }
                  else{

                   if(nsuccdim[posA] == 0){
                        nodes[posA].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                        ++nsuccdim[posA];
                                              }

                   else {
                      ++nsuccdim[posA];

                      if( (nodes[posA].successors = (unsigned long *) realloc(nodes[posA].successors, nsuccdim[posA]*sizeof(unsigned long)))  == NULL){
                        printf("Error when allocating memory for the successors of node posA = %lu\n",posA);
                        exit(0);

                          }
                      }
                      nodes[posA].successors[nsuccdim[posA]-1] = idB;



               }
             }

               else if( (count >= 10) & (way_nodes % 2 == 1) & (way_nodes >=3) ){
                 //|B|C|   B is in idB C is in idA so B does the rol of A and C does the rol of B
                  if(!is_oneway){
                    //idB is a succesor of idA


                    if(nsuccdim[posA] == 0){
                        nodes[posA].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                        ++nsuccdim[posA];
                                              }

                    else {
                      ++nsuccdim[posA];
                      if( (nodes[posA].successors = (unsigned long *) realloc(nodes[posA].successors, nsuccdim[posA]*sizeof(unsigned long)))  == NULL){
                        printf("Error when allocating memory for the successors of node posA = %lu\n",posA);
                        exit(0);
                      }
                                          }

                    if(nsuccdim[posB] == 0){
                      nodes[posB].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                      ++nsuccdim[posB];

                      }

                    else{
                      ++nsuccdim[posB];

                      //nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long));
                      if( (nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long)) ) == NULL){
                      printf("Error when allocating memory for the successors of node posB = %lu\n",posB);
                        exit(0);

                      }
                    }
                    nodes[posA].successors[nsuccdim[posA]-1] = idB;
                    nodes[posB].successors[nsuccdim[posB]-1] = idA;


                  }


                  else{

                    if(nsuccdim[posB] == 0){
                      nodes[posB].successors = (unsigned long *) malloc(1*sizeof(unsigned long));
                      ++nsuccdim[posB];
                      }

                    else{
                      ++nsuccdim[posB];
                      //nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long));
                      //printf("nsuccdimposB = %d \n", nsuccdim[posB]);
                      if( (nodes[posB].successors = (unsigned long *) realloc(nodes[posB].successors, nsuccdim[posB]*sizeof(unsigned long)) ) == NULL){
                      printf("Error when allocating memory for the successors of node posB = %lu\n",posB);
                        exit(0);

                      }
                    }
                    nodes[posB].successors[nsuccdim[posB]-1] = idA;
                  }

                  //note that we have only changed the else inside with respect to the big if above


               }
          }



            p = strtok_single (NULL, delims);
            count++;

         }//endwhile p of ways

          //if(way_nodes >0){
           //printf("Number of nodes of the way with id %lu: %lu \n", way_id, way_nodes);
         //}
       }//end elseif (ways)


   }

   //testing IDtoPOSITION:
   //unsigned long id = 8670491;*/
   /*unsigned long id = 8670500;
   printf("\nThe position of the id %lu is %d \n ", id, IDtoPOSITION(nodes, nnodes, id) );
   int pos = 8;
   printf("\nThe id of the position %d is %lu \n ", pos, POSITIONtoID(nodes, pos) );*/



   /*for(int i = 1; i<=nnodes;i++){
    free(nodes[i].successors);
    free(nodes[i].name);
   }*/
   //FILE * f = fopen("nsuccdims.txt", "a");
   //FILE * f = fopen("nsuccdims_spain.txt", "a");
    /*
  for(unsigned long i = 0; i<nnodes;i++){
      //fprintf(f, "%u,", nsuccdim[i]);
      nodes[i].nsucc = nsuccdim[i];

   }

  //sed "s/,$//" nsuccdims.txt | tr "," "\n" | sort -rn | head -10
    printf("\n HE ACABAT! \n");
   free(line);
   exit(EXIT_SUCCESS);


	return 0;*/
}//endmain
