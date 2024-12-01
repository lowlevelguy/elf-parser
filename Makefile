CC=gcc
SRCDIR=src
CFLAGS=-I$(SRCDIR) -g -DDEBUG_MODEc -Wall

ODIR=obj
_OBJ=main.o elf_header.o prog_header.o section_header.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))


_DEPS=elf_header.h prog_header.h section_header.h
DEPS=$(patsubst %,$(SRCDIR)/%,$(_DEPS))

$(ODIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)


elfparse: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o $(ODIR)/*/*.o *~ core
