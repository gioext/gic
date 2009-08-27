gic: gic.c main.c resize.c
	gcc -Wall -O2 -fopenmp -ljpeg -o gic gic.c resize.c main.c
