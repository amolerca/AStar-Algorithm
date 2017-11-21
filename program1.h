typedef struct {
	unsigned long id; // Node identification
	char *name;
	double lat, lon; // Node position
	unsigned short nsucc; // Number of node successors; i. e. length of successors
	unsigned long *successors;
} node;

void ExitError(char * message, int num);
char * strtok_single (char * str, char const * delims);

/*

int binary_searchID(node * Vector, int Vlen, int key){
	int imin=0,imax= Vlen-1;
	int imid;
    while(imax >= imin){
		imid = (int) ( (imin+imax)/2.0 );
		if(Vector[imid] == key) return imid;
		else if(Vector[imid] < key) imin = imid+1;
		else imax = imid-1;

	} 

   return -1;

}*/