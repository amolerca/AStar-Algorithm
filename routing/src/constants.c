#include "routing.h"
#include "constants.h"


const unsigned long nnodes = 3472620UL;  //CATALONIA
//unsigned long nnodes = 23895681UL; //SPAIN

const char INPUT_FIELD_DELIMITORS[2] = "|";

const char INPUT_LINE_IGNORE[2] = "#";

const size_t INPUT_MAX_CHARS_PER_LINE = 16777215;

const int NODE_FIELD_ID = 1;

const int NODE_FIELD_NAME = 2;

const int NODE_FIELD_LAT = 9;

const int NODE_FIELD_LON = 10;

const int WAY_FIELD_ONEWAY = 7;

const int WAY_FIELD_NODES = 9;

char TEST_MAP_DIR[100] = "inputs/catalonia.map";

char TEST_BIN_DIR[100] = "bin/testmap.bin";

const unsigned char NOT_VISITED = 0;

const unsigned char OPEN = 1;

const unsigned char CLOSE = 2;

const float EARTH_RADIUS = 6371000;

const float EARTH_EQUATORIAL_RADIUS = 6378137;

const float EARTH_POLAR_RADIUS = 6356.752;

const double PI = 3.141592653589793;

const int DYN_REALLOC_SIZE = 100;

const int ROUTE_REALLOC_SIZE = 10;
