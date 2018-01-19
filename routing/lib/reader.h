unsigned long CountLines(char line_type[], FILE *f);

Node GetNodeFromFields(char *fields);

void GetSuccessorsFromFields(char *fields, Node *node, unsigned long nnodes);

void FileParser(FILE *f, char line_type[], Node *node, unsigned long nnodes);

FILE *OpenFile(char file_dir[]);

void ReadFile(char file_dir[]);
