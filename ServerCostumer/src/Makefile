# Directory paths
IDIR=../../con2redis/src
IDIR_DB=../../con2db
ODIR=../obj
BDIR=../bin

# Compiler
# Compiler
CC=g++
CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -I$(IDIR_DB) -I/usr/include/postgresql -lpq -lm -lhiredis

# Dependency and source files
DEPS = $(wildcard $(IDIR)/*.h) $(wildcard $(IDIR_DB)/*.h)
SRC = $(wildcard *.cpp) logger.cpp
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

# Object files for con2redis and con2db
con2redis_OBJ=$(wildcard ../../con2redis/obj/*.o)
con2db_OBJ=$(wildcard ../../con2db/*.o)

# Targets
all: con2redis con2db $(BDIR)/main

con2redis:
	$(MAKE) -C ../../con2redis/src

con2db:
	$(MAKE) -C ../../con2db

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(con2redis_OBJ) $(con2db_OBJ) $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core
