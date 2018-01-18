#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int binary_search(int Vector[], int Vlen, int key);

int main(){
	int  Vector[] = {1, 3, 4, 6, 8, 9, 11};
	int key = 4;
	int Vlen = 7;

	//Algorithm
	int pos_key;
	pos_key = binary_search(Vector, Vlen, key);
	printf("The vector position of the key is the %dth (remember we start numbering from 0)\n", pos_key);

	return 0;
}

int binary_search(int Vector[], int Vlen, int key){
	int imin=0,imax= Vlen-1;
	int imid;
    while(imax >= imin){
		imid = (int) ( (imin+imax)/2.0 );
		if(Vector[imid] == key) return imid;
		else if(Vector[imid] < key) imin = imid+1;
		else imax = imid-1;

	} 

   return -1;

}