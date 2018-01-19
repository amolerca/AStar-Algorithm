 #<Routing - AStar Algorithm> makefile

# define the C compiler to use
CC = gcc

# detect OS
UNAME := $(shell uname)

# define any compile-time flags
ifeq ($(UNAME), Linux)
	CFLAGS = -D LINUX -Ofast
endif
ifeq ($(UNAME), Darwin)
	CFLAGS = -D OSX -Ofast
endif

# define any directories containing header files other than /usr/include
INCLUDES = -I lib

# define custom library paths
LFLAGS = -L lib

# define any libraries to link into executable (only Linux needs this)
ifeq ($(UNAME), Linux)
	LFLAGS = -D LINUX -lm -lbsd
endif

# define C source files
_SRCS1 = map_reader.c constants.c util.c graph.c reader.c binary.c

# place them into the right repository folder
SDIR = src
SRCS1 = $(patsubst %,$(SDIR)/%,$(_SRCS1))

# define executable files
P1 = map_reader.exe

.PHONY: clean

# run the compiler
all: $(P1)
	@echo The executable \'$(P1)\' has been compiled

$(P1):
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(P1) $(SRCS1) $(LFLAGS) $(LIBS)

# in case we want to delete the executables
clean:
	@$(RM) -r $(P1)
	@echo The executable \'$(P1)\' has been deleted
