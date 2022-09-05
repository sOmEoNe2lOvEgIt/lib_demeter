#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main (int ac, char **av)
{
    long i = 0;

    if (ac != 2) {
        printf("usage: %s SizeOfMemInBytes\n", av[0]);
        return (1);
    }
    long size = atol(av[1]);
    char *mem = malloc(sizeof(char) * size);
    if (mem == NULL) {
        printf("malloc failed\n");
        return (1);
    }
    for (i = 0; i < size; i++) {
        mem[i] = 'a';
    }
    free(mem);
    return (0);
}