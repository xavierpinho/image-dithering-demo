LIBNAME=imgdither
LIBFILE=lib$(LIBNAME).a
LIBOBJS=image.o netpbm.o png.o greyscale.o dither.o
CFLAGS+=-pedantic -Wall -W -Wpointer-arith -Wmissing-prototypes -Wstrict-prototypes -Wreturn-type -Wcast-qual -Wwrite-strings -Wshadow -Winline -Wredundant-decls -Wcast-align -Wchar-subscripts -Wno-uninitialized
LDFLAGS+=-lpng -l$(LIBNAME) -L.
CLI=cli

.PHONY: all clean

all: $(LIBFILE) $(CLI)

clean:
	-rm -vf $(LIBFILE) $(LIBOBJS) $(CLI)

$(LIBFILE): $(LIBOBJS)
	ar rc $@ $^
	ranlib $@

$(CLI): cli.c $(LIBFILE)
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@
