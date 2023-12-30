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

/*** Error Handling ***/

void fraction_errno_set(enum fraction_err err)
{
    fraction_errno = err;
}

enum fraction_err fraction_errno_get()
{
    return fraction_errno;
}

/*** Mathematical Operations ***/

int fraction_negate(struct fraction *res, const struct fraction *f)
{
    int err;

    err = fraction_copy(res, f);
    if (err < 0)
        return err;
    
    res->numerator = -res->numerator;

    return 0;
}

int fraction_reciprocate(struct fraction *res, const struct fraction *f)
{
    if (f->denominator > (ullint)(LLINT_MAX)) {
        fraction_errno_set(FRAC_OVERFLOW);
        return -1;
    }

    return fraction_set(res, llcopysign(f->denominator, f->numerator), abs(f->numerator));
}

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

int fraction_subtract(struct fraction *res, const struct fraction *minuend, const struct fraction *subtrahend)
{
    struct fraction temp;
    int err;

    err = fraction_negate(&temp, subtrahend);
    if (err < 0)
        return err;
    
    return fraction_sum(res, minuend, &temp);
}

int fraction_divide(struct fraction *res, const struct fraction *dividend, const struct fraction *divisor)
{
    struct fraction temp;
    int err;

    err = fraction_reciprocate(&temp, divisor);
    if (err < 0)
        return err;
    
    return fraction_multiply(res, dividend, &temp);
}

/*** Type Casting ***/

char *fraction_to_str(const struct fraction *f, char *sep)
{
    static ssize_t ullint_aschar_size = -1;

    char *fstr;
    size_t size;

    if (!sep)
        sep = DEFAULT_SEP;

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

int fraction_from_str(struct fraction *f, char *str, char *sep)
{
    llint numerator;
    ullint denominator;
    int err;

    if (!sep)
        sep = DEFAULT_SEP;

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

llint fraction_to_llint(const struct fraction *f)
{
    return f->numerator / f->denominator;
}

int fraction_from_llint(struct fraction *f, llint number)
{
    return fraction_set(f, number, 1);
}

double fraction_to_double(const struct fraction *f)
{
    return (double)(f->numerator) / (double)(f->denominator);
}

int fraction_from_double(struct fraction *f, double value)
{
    int sign = 1;

    if (value < 0) {
        sign = -1;
        value = -value;
    }

    if (value > (double)LLINT_MAX) {
        fraction_errno_set(FRAC_OVERFLOW);
        return -1;
    }

    f->numerator = (llint)(floor(value));
    f->denominator = 1;
    value -= (double)(f->numerator);

    while (value > 0) {
        int units_digit;

        value *= 2;
        units_digit = (llint)(floor(value));
        value -= (double)(units_digit);

        f->denominator <<= 1;
        f->numerator = (f->numerator << 1) + units_digit;
    }

    f->numerator *= sign;

    fraction_reduce(f);

    return 0;
}