#include <stdio.h>
#include <stdlib.h>
#include <include/fractions.h>

int main()
{
    struct fraction *f;
    double value;

    f = fraction_clone(ZERO_FRACTION);

    fraction_from_str(f, "9/40", NULL);
    value = fraction_to_double(f);
    fraction_from_double(f, value);

    if (1) {
        char *fstr = fraction_to_str(f, NULL);
        printf("%s\n", fstr);
        free(fstr);
    }

    fraction_destroy(f);

    return 0;
}