gic: gic.c main.c
	gcc -Wall -L/opt/local/lib -I/opt/local/include -ljpeg -o gic gic.c main.c
