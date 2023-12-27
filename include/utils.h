#ifndef UTILS_H
#define UTILS_H

typedef unsigned long long ullint;
typedef long long llint;

#define ULLINT_MAX  (~(0ULL))
#define LLINT_MAX   (~(0ULL) >> 1)

/*** Maths ***/
int greatest_common_divisor(ullint a, ullint b);
ullint lowest_common_multiplier(ullint a, ullint b);
llint llcopysign(llint x, llint y);
int signof(llint a);

/*** Strings ***/
char *strskip(char *str, char *sep);

#endif