#IDIR=../hdr
IDIR=../../con2redis/src
IDIR_DB=../../con2db
ODIR=../obj
BDIR=../bin

CC=g++
# use this when connecting to Postgresql (library lpq)
#CFLAGS=-std=c++11 -I. -I$(IDIR) -I/usr/include/postgresql -lpq -lm 
CFLAGS=-std=c++20 -g -ggdb -fpermissive -L/usr/local/lib -Wall -I. -I$(IDIR) -I$(IDIR_DB) -I/usr/include/postgresql -lpq -lm -lhiredis

# use this when no connection to Postgresql (library lpq) is needed
#CFLAGS=-std=c++11 -I. -I$(IDIR) -lm 

DEPS = $(wildcard $(IDIR)/*.h) $(wildcard $(IDIR_DB)/*.h)
#OBJ = $(wildcard $(ODIR)/*.o)
SRC = $(wildcard *.cpp)
OBJ = $(patsubst %.cpp, $(ODIR)/%.o, $(SRC))

con2redis_OBJ=$(wildcard ../../con2redis/obj/*.o)
con2db_OBJ=$(wildcard ../../con2db/*.o)
tools_OBJ=$(wildcard ../../tools/obj/*.o)


all:	con2redis $(BDIR)/main

con2redis:
	$(MAKE) -C ../../con2redis/src

con2db:
	$(MAKE) -C ../../con2db

tools:
	$(MAKE) -C ../../tools/src


$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BDIR)/main: $(OBJ) $(DEPS)
	$(CC) -o $@ $(OBJ) $(con2redis_OBJ) $(con2db_OBJ) $(tools_OBJ) $(CFLAGS)


.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ $(IDIR)/*~ core 
