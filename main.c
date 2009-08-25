#include <stdio.h>
#include <stdlib.h>
#include "gic.h"

int
main(int argc, char *argv[])
{
    gic_write_ppm(argv[1], argv[2]);
    return 0;
}
