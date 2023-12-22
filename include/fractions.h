typedef unsigned long long ullint;
typedef long long llint;

#define ULLINT_MAX ~0ULL
#define LLINT_MAX ~0ULL >> 1

struct fraction {
    llint numerator;
    ullint denominator;
};

enum fraction_err {
    FRAC_SUCCESS = 0,
    FRAC_DIV_0,
    FRAC_LIBC_ERR,
};

int greatest_common_divisor(ullint a, ullint b);
ullint lowest_common_multiplier(ullint a, ullint b);
llint llcopysign(llint x, llint y);
int signof(llint a);

void fraction_errno_set(enum fraction_err err);
enum fraction_err fraction_errno_get();

struct fraction *fraction_create(llint numerator, ullint denominator);
void fraction_destroy(struct fraction *f);
int fraction_set(struct fraction *f, llint numerator, ullint denominator);
void fraction_reduce(struct fraction *f);
int fraction_sum(struct fraction *res, struct fraction *a, struct fraction *b);
int fraction_multiply(struct fraction *res, struct fraction *a, struct fraction *b);
char *fraction_str(struct fraction *f, char *sep);