CFLAGS=-g -Wall -Werror
GCC= gcc

all: tests lib_tar.o

lib_tar.o: lib_tar.c lib_tar.h
	cc -g -Wall -Werror   -c -o lib_tar.o lib_tar.c

tests: tests.c lib_tar.h lib_tar.c
	gcc tests.c lib_tar.h lib_tar.c -o test
	./test dir.tar

clean:
	rm -f lib_tar.o tests soumission.tar

submit: all
	tar --posix --pax-option delete=".*" --pax-option delete="*time*" --no-xattrs --no-acl --no-selinux -c *.h *.c Makefile > soumission.tar
	
