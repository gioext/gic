CC = gcc
OPTS = -O2 -fopenmp -march=nocona -mmmx -msse -msse2 -Wall

gic: main.c
	$(CC) $(OPTS) -ljpeg -lgic -o $@ main.c
