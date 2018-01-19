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
