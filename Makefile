IDIR =../include
CC=g++
CFLAGS= -I$(IDIR) -g -O0

ODIR=.

LIBS=-lncurses

_DEPS = functions.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ1 = main.o functions.o
OBJ1 = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: main
	
main: $(_OBJ1)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)
.PHONY: clean

clean:
	rm -f *~ core $(INCDIR)/*~ 
	rm -f main 
	rm -f *.o

etags: 
	find . -type f -iname "*.[ch]" | xargs etags --append 
