#include <stdlib.h>
#include <string.h>
#include <include/utils.h>

/*** Support functions ***/

/*
 * Function: greatest_common_divisor
 *
 * Description: Determines the greatest common divisor of two unsigned integers.
 *
 * Parameters:
 *   a: The first unsigned integer.
 *   b: The second unsigned integer.
 *
 * Return Value:
 *   The greatest common divisor of the two integers.
 */

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



/*
 * Function: lowest_common_multiplier
 *
 * Description: Calculates the lowest common multiple of two unsigned integers.
 *
 * Parameters:
 *   a: The first unsigned integer.
 *   b: The second unsigned integer.
 *
 * Return Value:
 *   The lowest common multiple of the two integers.
 */

ullint lowest_common_multiplier(ullint a, ullint b)
{
    return (a / greatest_common_divisor(a, b)) * b;
}



/*
 * Function: llcopysign
 *
 * Description: Copies the sign of the second argument to the first argument.
 *
 * Parameters:
 *   x: The first long long integer.
 *   y: The second long long integer.
 *
 * Return Value:
 *   The result of copying the sign of y to x.
 */

llint llcopysign(llint x, llint y)
{

    if (x == 0)
        return 0;

    llint absx = llabs(x);
    llint y_is_negative = y >> ((sizeof(y) << 3) - 1);
    
    return y_is_negative ? -absx : absx;
}



/*
 * Function: signof
 *
 * Description: Returns the sign of the long long integer.
 *
 * Parameter:
 *   a: The long long integer to check the sign of.
 *
 * Return Value:
 *   The sign of the long long integer (-1, 0, or 1).
 */

int signof(llint a)
{
    return (int) llcopysign(1, a);
}



/*
 * Function: strskip
 *
 * Description: Skips over a given separator in a string.
 *
 * Parameter:
 *   str: The string to search for the separator in.
 *   sep: The separator to search for.
 *
 * Return Value:
 *   A pointer to the first character after the separator in the string, or a null pointer if the separator is not found.
 */

char *strskip(char *str, char *sep)
{
    size_t seplen;

    seplen = strlen(sep);

    while (*str && strncmp(str, sep, seplen) != 0) {
        str += 1;
    }

    if (*str)
        str += seplen;
    
    return str;
}