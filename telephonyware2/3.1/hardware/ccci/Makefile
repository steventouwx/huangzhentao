# SPDX-License-Identifier:MediaTekProprietary
CFLAGS = -O2 -Wall -fPIC

INCLUDES=-Iinclude
LIBS=$(TARGET_LDFLAGS) -L. -lprop

LIBNAME=libccci.so

all: $(LIBNAME)

%.o: %.c
	$(CC) $(WFLAGS) -c -o $@ $(INCLUDES) $(CFLAGS) $<

LIBCCCI_OBJ=ccci_lib.o platform/ccci_lib_platform.o

$(LIBNAME): $(LIBCCCI_OBJ)
	$(CC) $(CFLAGS) -Wl,-Bsymbolic-functions -shared -o $@ $^

clean:
	rm -f *.o
	rm -f *.so
	rm -f platform/*.o