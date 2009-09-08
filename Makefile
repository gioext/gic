CC = gcc
INCLUDES = -I/usr/local/include -I/opt/local/include -I.
LIBS = -L/usr/local/lib -L. -ljpeg
OPTS = -O2 -fopenmp -march=nocona -mmmx -msse -msse2 -Wall

gic: main.c libgic.so
	$(CC) $(INCLUDES) $(LIBS) $(OPTS) -lgic -o $@ main.c

libgic.so: gic.c resize.c
	$(CC) -shared -fPIC $(INCLUDES) $(LIBS) $(OPTS) -o $@ gic.c resize.c

gic.c: gic.h
resize.c: gic.h
