# Option

An option type or maybe type is a polymorphic type that represents encapsulation of an optional value; 
e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.

```C

#include <stdio.h>

typedef const double *Number;

static Number Number_new(double number);

static Number zero(void);
static Number cube(Number number);
static OptionOf(Number) division(Number dividend, Number divisor);
static OptionOf(Number) squareRoot(Number number);

int main() {
    Number number = Option_unwrap(
            Option_alt(
                    Option_map(Option_chain(division(Number_new(36), Number_new(4)), squareRoot), cube),
                    Option_some(zero())
            )
    );
    printf("Number is: %f\n", *number);
    return 0;
}

```
