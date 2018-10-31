IDIR=include
CC=g++
CFLAGS=-I$(IDIR) -std=c++17 -O2

SDIR=src
ODIR=$(SDIR)/obj
LDIR=lib

LIBS=-lm -lpthread

_DEPS = token.hpp lexer.hpp ast.hpp parser.hpp interpreter.hpp
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o lexer.o token.o ast.o parser.o interpreter.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

OUTPUT = kale

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

make: $(OBJ)
	$(CC) -o $(OUTPUT) $^ $(CFLAGS) $(LIBS)
	
debug: $(OBJ)
	$(CC) -g -o $(OUTPUT) $^ $(CFLAGS) $(LIBS) 

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 