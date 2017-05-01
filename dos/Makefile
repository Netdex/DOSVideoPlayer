EXEC_FILE=badapple

SDIR=src
ODIR=obj
LDIR=lib
BDIR=bin
IDIR=include

CC=gcc
CFLAGS=-I$(IDIR) -O2 -Werror
LIBS=-lm

DEPS = $(wildcard $(IDIR)/*.h)
SOURCES = $(notdir $(wildcard $(SDIR)/*.c))
OBJ = $(addprefix $(ODIR)/, $(SOURCES:.c=.o))


$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(EXEC_FILE): $(OBJ)
	$(CC) -o $(BDIR)/$@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 