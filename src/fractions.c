#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <aio.h>
#include <string.h>
#include <include/fractions.h>

/* struct fraction : utilities */

int greatest_common_divisor(ullint a, ullint b)
{
    ullint temp;

    while (b != 0) {
        temp = b;
        b = a % b;
        a = temp;
    }

    return a;
}

ullint lowest_common_multiplier(ullint a, ullint b)
{
    return (a / greatest_common_divisor(a, b)) * b;
}

llint llcopysign(llint x, llint y)
{

    if (x == 0)
        return 0;

    llint absx = llabs(x);
    llint y_is_negative = y >> ((sizeof(y) << 3) - 1);
    
    return y_is_negative ? -absx : absx;
}

int signof(llint a)
{
    return (int) llcopysign(1, a);
}

/* struct fraction : error handling */

enum fraction_err fraction_errno = FRAC_SUCCESS;

void fraction_errno_set(enum fraction_err err)
{
    fraction_errno = err;
}

enum fraction_err fraction_errno_get()
{
    return fraction_errno;
}

/* struct fraction : methods*/

void fraction_destroy(struct fraction *f)
{
    free(f);
}

int fraction_set(struct fraction *f, llint numerator, ullint denominator)
{
    if (denominator == 0) {
        fraction_errno_set(FRAC_DIV_0);
        return -1;
    }

    f->numerator = numerator;
    f->denominator = denominator;

    fraction_reduce(f);

    return 0;
}

struct fraction *fraction_create(llint numerator, ullint denominator)
{
    struct fraction *f;
    int err;

    f = malloc(sizeof(*f));
    if (!f) {
        fraction_errno_set(FRAC_LIBC_ERR);
        return NULL;
    }

    err = fraction_set(f, numerator, denominator);
    if (err == -1) {
        free(f);
        return NULL;
    }

    return f;
}

void fraction_reduce(struct fraction *f)
{
    int gcd;

    gcd = greatest_common_divisor(llabs(f->numerator), f->denominator);

    f->numerator = f->numerator / gcd;
    f->denominator = f->denominator / gcd;
}

int fraction_sum(struct fraction *res, struct fraction *a, struct fraction *b)
{
    llint denominator;
    llint numerator;

    denominator = lowest_common_multiplier(a->denominator, b->denominator);
    numerator = a->numerator * (denominator / a->denominator)
        + b->numerator * (denominator / b->denominator);

    return fraction_set(res, numerator, denominator);
}

int fraction_multiply(struct fraction *res, struct fraction *a, struct fraction *b)
{
    struct fraction across;
    struct fraction bcross;
    int err;

    err = fraction_set(&across, a->numerator, b->denominator);
    if (err == -1)
        return err;
    
    err = fraction_set(&bcross, b->numerator, a->denominator);
    if (err == -1)
        return err;

    res->numerator = across.numerator * bcross.numerator;
    res->denominator = across.denominator * bcross.denominator;

    return 0;
}

char *fraction_str(struct fraction *f, char *sep)
{
    static ssize_t uint_aschar_size = -1;

    char *fstr;
    size_t size;

    /* caching */
    if (uint_aschar_size == -1) {
        uint_aschar_size = ceil(log10(ULLINT_MAX));
    }

    size = 2 * uint_aschar_size + 1 + strlen(sep);

    fstr = malloc(size * sizeof(*fstr));
    if (!fstr) {
        fraction_errno_set(FRAC_LIBC_ERR);
        return NULL;
    }

    snprintf(fstr, size, "%lld%s%llu", f->numerator, sep, f->denominator);

    return fstr;
}