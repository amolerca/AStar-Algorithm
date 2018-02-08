typedef struct
{
    char *input_file;
    char *output_file;
    bool fast;
} ReaderArguments;

typedef struct
{
    char *input_file;
    char *output_file;
    unsigned long starting_node;
    unsigned long ending_node;
    unsigned int heuristic_method;
    unsigned int weight_method;

/*    double starting_latitude;
    double starting_longitude;
    double goal_latitude;
    double goal_longitude;*/

} RoutingArguments;

void CopyString(char *target, char *source);

bool EqualStrings(const char *s1, const char *s2);

bool StartsWith(const char *pre, const char *str);

bool EndsWith(const char *suffix, const char *str);

char *SplitFields(char * str, char const * delims);

int FieldsCounter(const char array[], char delimiter[]);

bool ParseYesNo();

void ExitError(char * message, int num);

FILE *OpenFile(const char file_dir[], char mode[], int i);

const char *SingleArgumentParser(int argc, char *argv[]);

char *Concat(const char *s1, const char *s2);

void Compress(const char *bin_dir);

void Decompress(const char *bin_dir);

double ToRadians(double degrees);

unsigned int MakeAQuery(char query[], unsigned int min, unsigned int max);

void PrintOutDistOptions();

void AStarWelcome();

void PrintOutIterationInfo(unsigned int current_iteration, double g, double h);

void PrintOutResults(unsigned int current_iteration, double g, double h,
                     double AStar_CPU_time);

void PrintOutReaderCLUsage();

void PrintOutReaderCLHelp();

void SetDefaultReaderArgs(ReaderArguments *args);

void ParseReaderArgs(int argc, char **argv, ReaderArguments *args);

void CheckReaderArgs(ReaderArguments *args);

void PrintOutRoutingCLUsage();

void PrintOutRoutingCLHelp();

void SetDefaultRoutingArgs(RoutingArguments *args);

void ParseRoutingArgs(int argc, char **argv, RoutingArguments *args);

void CheckRoutingArgs(RoutingArguments *args);
