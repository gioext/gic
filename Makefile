gic: gic.c main.c resize.c
	gcc -O2 -fopenmp -march=nocona -mmmx -msse -msse2 -msse3 -Wall -I/opt/local/include -L/opt/local/lib -ljpeg -o gic gic.c resize.c main.c
