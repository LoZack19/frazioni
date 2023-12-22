#include <stdio.h>
#include <stdlib.h>
#include <include/fractions.h>

int main()
{
    struct fraction *f, *g, *sum, *mult;
    char *fstr, *gstr, *sumstr, *multstr;

    f = fraction_create(4, 8);
    g = fraction_create(-8, 17);
    sum = fraction_create(0, 1);
    mult = fraction_create(0, 1);

    fraction_sum(sum, f, g);
    fraction_multiply(mult, f, g);

    fstr = fraction_str(f, "/");
    gstr = fraction_str(g, "/");
    sumstr = fraction_str(sum, "/");
    multstr = fraction_str(mult, "/");

    printf("%s + %s = %s\n", fstr, gstr, sumstr);
    printf("%s * %s = %s\n", fstr, gstr, multstr);

    free(fstr);
    free(gstr);
    free(sumstr);
    free(multstr);
    
    fraction_destroy(f);
    fraction_destroy(g);
    fraction_destroy(sum);
    fraction_destroy(mult);

    return 0;
}