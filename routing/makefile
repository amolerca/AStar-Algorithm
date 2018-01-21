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
_SRCS1 = bincreator.c constants.c util.c graph.c reader.c binary.c
_SRCST = test.c constants.c util.c graph.c reader.c binary.c

# place them into the right repository folder
SDIR = src
SRCS1 = $(patsubst %,$(SDIR)/%,$(_SRCS1))
SRCST = $(patsubst %,$(SDIR)/%,$(_SRCST))

# define executable files
P1 = bincreator.exe
T = test.exe

.PHONY: clean

# run the compiler
all: $(P1) $(T)
	@echo All executables have been compiled

$(P1):
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(P1) $(SRCS1) $(LFLAGS)

$(T):
	@$(CC) $(CFLAGS) $(INCLUDES) -o $(T) $(SRCST) $(LFLAGS)

install: $(P1)
	@echo The executable \'$(P1)\' has been compiled

test: $(T)
	@echo The executable \'$(T)\' has been compiled

# in case we want to delete the executables
clean:
	@$(RM) -r $(P1) $(T)
	@echo All executables have been deleted

help:
	@echo --------------------
	@echo \ make instructions:
	@echo --------------------
	@echo  - Type \'make\' to install all executables
	@echo  - Type \'make install\' to install only the main executables of the program
	@echo  - Type \'make test\' to install the testing executable of the program
	@echo  - Type \'make clean\' to remove all executables
