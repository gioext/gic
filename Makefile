gic: gic.c main.c
	gcc -Wall -ljpeg -o gic gic.c gic_resize.c main.c
