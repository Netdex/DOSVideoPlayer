EXEC_FILE=badapple

SDIR=src
ODIR=obj
LDIR=lib
BDIR=bin
IDIR=include

CC=gcc
CFLAGS=-I$(IDIR) -O3 -Werror
LIBS=-lm

DEPS = $(wildcard $(IDIR)/*.h)
SOURCES = $(wildcard $(SDIR)/*.c)
OBJ = $(SOURCES:.c=.o)


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC_FILE): $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 