CC = gcc
LIBS = -ljpeg
OPTS = -O2 -fopenmp -march=nocona -mmmx -msse -msse2 -Wall

libgic.so: gic.c resize.c
	$(CC) -shared -fPIC $(LIBS) $(OPTS) -o $@ gic.c resize.c

gic.c: gic.h
resize.c: gic.h
