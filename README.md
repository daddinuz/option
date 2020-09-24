# Option

[![Build Status](https://travis-ci.org/daddinuz/option.svg?branch=master)](https://travis-ci.org/daddinuz/option)

An option type is a type that represents encapsulation of an optional value;
e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.

```C
#include <stdio.h>
#include <option.h>

/*
 * .h
 */
option_declare(OptionalNumber, double);

OptionalNumber divide(double numerator, double denominator);

/*
 * main.c
 */
int main(void) {
    OptionalNumber number = divide(18, 0);
    printf("%f\n", OptionalNumber_expect(&number, "'%s'\nError: expected a number", TRACE));
    return 0;
}

/*
 * .c
 */
option_define(OptionalNumber, double);

OptionalNumber divide(const double numerator, const double denominator) {
    return -0.000001 <= denominator && denominator <= 0.000001
            ? OptionalNumber_none()
            : OptionalNumber_some(numerator / denominator);
}
```
