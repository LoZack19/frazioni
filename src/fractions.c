#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <aio.h>
#include <string.h>
#include <errno.h>
#include <include/fractions.h>

/*** Module State Variables ***/
enum fraction_err fraction_errno = FRAC_SUCCESS;

/*** Module Global Data ***/
const struct fraction ZERO_FRACTION_ = {
    .numerator = ZERO_FRAC_NUM,
    .denominator = ZERO_FRAC_DEN
};

const struct fraction ONE_FRACTION_ = {
    .numerator = 1LL,
    .denominator = 1ULL
};

/*** Error Handling ***/

void fraction_errno_set(enum fraction_err err)
{
    fraction_errno = err;
}

enum fraction_err fraction_errno_get()
{
    return fraction_errno;
}

/*** Constructors and Destructors ***/

void fraction_destroy(struct fraction *f)
{
    free(f);
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
    if (err < 0) {
        free(f);
        return NULL;
    }

    return f;
}

struct fraction *fraction_clone(const struct fraction *f)
{
    return fraction_create(f->numerator, f->denominator);
}

/*** Setters and Getters ***/

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

int fraction_copy(struct fraction *dest, const struct fraction *src)
{
    return fraction_set(dest, src->numerator, src->denominator);
}

/*** Data Canonization ***/

void fraction_reduce(struct fraction *f)
{
    int gcd;

    gcd = greatest_common_divisor(llabs(f->numerator), f->denominator);

    f->numerator = f->numerator / gcd;
    f->denominator = f->denominator / gcd;
}

/*** Mathematical Operations ***/

int fraction_sum(struct fraction *res, const struct fraction *a, const struct fraction *b)
{
    llint denominator;
    llint numerator;

    denominator = lowest_common_multiplier(a->denominator, b->denominator);
    numerator = a->numerator * (denominator / a->denominator)
        + b->numerator * (denominator / b->denominator);

    return fraction_set(res, numerator, denominator);
}

int fraction_multiply(struct fraction *res, const struct fraction *a, const struct fraction *b)
{
    struct fraction across;
    struct fraction bcross;
    int err;

    err = fraction_set(&across, a->numerator, b->denominator);
    if (err < 0)
        return err;
    
    err = fraction_set(&bcross, b->numerator, a->denominator);
    if (err < 0)
        return err;

    res->numerator = across.numerator * bcross.numerator;
    res->denominator = across.denominator * bcross.denominator;

    return 0;
}

/*** Type Casting ***/

char *fraction_tostr(const struct fraction *f, char *sep)
{
    static ssize_t ullint_aschar_size = -1;

    char *fstr;
    size_t size;

    /* caching */
    if (ullint_aschar_size == -1) {
        ullint_aschar_size = floor(log10(ULLINT_MAX) + 1);
    }

    size = 2 * ullint_aschar_size + 1 + strlen(sep);

    fstr = malloc(size * sizeof(*fstr));
    if (!fstr) {
        fraction_errno_set(FRAC_LIBC_ERR);
        return NULL;
    }

    snprintf(fstr, size, "%lld%s%llu", f->numerator, sep, f->denominator);

    return fstr;
}

int fraction_fromstr(struct fraction *f, char *sep, char *str)
{
    llint numerator;
    ullint denominator;
    int err;

    errno = 0;
    numerator = strtoll(str, NULL, 10);
    if (errno != 0) {
        fraction_errno_set(FRAC_LIBC_ERR);
        return -1;
    }

    str = strskip(str, sep);
    if (*str == 0) {
        fraction_errno_set(FRAC_FORMAT_ERR);
        return -1;
    }

    denominator = strtoull(str, NULL, 10);
    if (errno != 0) {
        fraction_errno_set(FRAC_LIBC_ERR);
        return -1;
    }

    err = fraction_set(f, numerator, denominator);
    if (err < 0)
        return -1;

    return 0;
}