EXEC_FILE=badapple
IDIR=include
CC=gcc
CFLAGS=-I$(IDIR) -O3

SDIR=src
ODIR=obj
LDIR =lib
BDIR=bin

LIBS=-lm

_DEPS = vga.h types.h util.h decoder.h keyboard.h lz4.h graphics.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = main.o vga.o util.o decoder.o keyboard.o lz4.o graphics.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC_FILE): $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 