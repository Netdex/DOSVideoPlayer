IDIR =include
CC=gcc
CFLAGS=-I$(IDIR)

SDIR=src
ODIR=obj
LDIR =lib
BDIR=bin

LIBS=-lm

_DEPS = vga.h types.h util.h decoder.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o vga.o util.o decoder.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

badapple: $(OBJ)
	gcc -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 