#ifndef FRACTIONS_H
#define FRACTIONS_H

#include <include/utils.h>

#define DEFAULT_SEP     "/"

#define ZERO_FRAC_NUM   (0LL)
#define ZERO_FRAC_DEN   (1ULL)
#define ZERO_FRACTION   (&ZERO_FRACTION_)
#define ONE_FRACTION    (&ONE_FRACTION_)

struct fraction {
    llint numerator;
    ullint denominator;
};

enum fraction_err {
    FRAC_SUCCESS = 0,
    FRAC_DIV_0,
    FRAC_LIBC_ERR,
    FRAC_FORMAT_ERR,
    FRAC_OVERFLOW
};

/*** Module Global Data ***/
extern const struct fraction ZERO_FRACTION_;
extern const struct fraction ONE_FRACTION_;

/*** Constructors and Destructors ***/
struct fraction *fraction_create(llint numerator, ullint denominator);
struct fraction *fraction_clone(const struct fraction *f);
void fraction_destroy(struct fraction *f);

/*** Setters and Getters ***/
int fraction_set(struct fraction *f, llint numerator, ullint denominator);
int fraction_copy(struct fraction *dest, const struct fraction *src);

/*** Data Canonization Functions ***/
void fraction_reduce(struct fraction *f);

/*** Error Handling Functions ***/
void fraction_errno_set(enum fraction_err err);
enum fraction_err fraction_errno_get();

/*** Mathematical Operations ***/
int fraction_sum(struct fraction *res, const struct fraction *a, const struct fraction *b);
int fraction_multiply(struct fraction *res, const struct fraction *a, const struct fraction *b);

/*** Type Cast Functions ***/
char *fraction_to_str(const struct fraction *f, char *sep);
int fraction_from_str(struct fraction *f, char *str, char *sep);

llint fraction_to_llint(const struct fraction *f);
int fraction_from_llint(struct fraction *f, llint number);

double fraction_to_double(const struct fraction *f);
int fraction_from_double(struct fraction *f, double value);

#endif