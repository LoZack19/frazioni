#include <stdio.h>
#include <stdlib.h>
#include <include/fractions.h>

int main()
{
    struct fraction *f;

    f = fraction_create(10, 10);

    {
        char *str = fraction_tostr(f, "/");
        printf("%s\n", str);
        free(str);
    }

    fraction_destroy(f);

    return 0;
}