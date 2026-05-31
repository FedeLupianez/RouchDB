
#include "common/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    Conditional cond;
    memcpy(cond.column, "hola", sizeof(char) * 4);
    printf("%s\n", cond.column);

    return EXIT_SUCCESS;
}
