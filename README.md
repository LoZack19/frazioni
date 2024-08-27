# Fraction Library

This library provides a set of functions to create, manipulate, and perform operations on fractions. It includes constructors, destructors, setters, getters, mathematical operations, and type conversion functions.

## Table of Contents
- [Installation](#installation)
- [Usage](#usage)
- [API](#api)
  - [Constructors and Destructors](#constructors-and-destructors)
  - [Setters and Getters](#setters-and-getters)
  - [Data Canonization Functions](#data-canonization-functions)
  - [Error Handling Functions](#error-handling-functions)
  - [Mathematical Operations](#mathematical-operations)
  - [Type Cast Functions](#type-cast-functions)
- [Error Codes](#error-codes)
- [Constants](#constants)

## Installation

Include the `fractions.h` header file in your project and link against the corresponding implementation file.

```c
#include "fractions.h"
```

## Usage

### Creating and Destroying Fractions

```c
struct fraction *frac = fraction_create(1, 2);
fraction_destroy(frac);
```

### Performing Operations

```c
struct fraction *a = fraction_create(1, 2);
struct fraction *b = fraction_create(3, 4);
struct fraction res;

fraction_sum(&res, a, b);
fraction_destroy(a);
fraction_destroy(b);
```

## API

### Constructors and Destructors

- `struct fraction *fraction_create(llint numerator, ullint denominator);`
- `struct fraction *fraction_clone(const struct fraction *f);`
- `void fraction_destroy(struct fraction *f);`

### Setters and Getters

- `int fraction_set(struct fraction *f, llint numerator, ullint denominator);`
- `int fraction_copy(struct fraction *dest, const struct fraction *src);`

### Data Canonization Functions

- `void fraction_reduce(struct fraction *f);`

### Error Handling Functions

- `void fraction_errno_set(enum fraction_err err);`
- `enum fraction_err fraction_errno_get();`

### Mathematical Operations

- `int fraction_negate(struct fraction *res, const struct fraction *f);`
- `int fraction_reciprocate(struct fraction *res, const struct fraction *f);`
- `int fraction_sum(struct fraction *res, const struct fraction *a, const struct fraction *b);`
- `int fraction_multiply(struct fraction *res, const struct fraction *a, const struct fraction *b);`
- `int fraction_subtract(struct fraction *res, const struct fraction *minuend, const struct fraction *subtrahend);`
- `int fraction_divide(struct fraction *res, const struct fraction *dividend, const struct fraction *divisor);`

### Type Cast Functions

- `char *fraction_to_str(const struct fraction *f, char *sep);`
- `int fraction_from_str(struct fraction *f, char *str, char *sep);`
- `llint fraction_to_llint(const struct fraction *f);`
- `int fraction_from_llint(struct fraction *f, llint number);`
- `double fraction_to_double(const struct fraction *f);`
- `int fraction_from_double(struct fraction *f, double value);`

## Error Codes

- `FRAC_SUCCESS`: Operation successful
- `FRAC_DIV_0`: Division by zero error
- `FRAC_LIBC_ERR`: Standard library error
- `FRAC_FORMAT_ERR`: Format error
- `FRAC_OVERFLOW`: Overflow error

## Constants

- `DEFAULT_SEP`: Default separator for string representation
- `ZERO_FRACTION`: Zero fraction constant
- `ONE_FRACTION`: One fraction constant
