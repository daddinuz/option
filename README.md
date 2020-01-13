# Option

[![Build Status](https://travis-ci.org/daddinuz/option.svg?branch=master)](https://travis-ci.org/daddinuz/option)

An option type is a type that represents encapsulation of an optional value;
e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.

```C
/*
 * .h
 */
#include <stdio.h>
#include <option.h>

OptionDeclare(OptionalNumber, double);

struct OptionalNumber divide(double numerator, double denominator);

int main() {
    struct OptionalNumber number = divide(18, 0);
    printf("%f\n", OptionalNumber_expect(number, "'%s': expected a number", __TRACE__));
    return 0;
}

/*
 * .c
 */
OptionDefine(OptionalNumber, double);

struct OptionalNumber divide(const double numerator, const double denominator) {
    return -0.0001 <= denominator && denominator <= 0.0001 ? OptionalNumber_none() : OptionalNumber_some(numerator / denominator);
}
```
