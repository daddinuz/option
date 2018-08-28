/*
Author: daddinuz
email:  daddinuz@gmail.com

Copyright (c) 2018 Davide Di Carlo

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>
#include <stdio.h>
#include <option.h>
#include <stdlib.h>

double *Number(const double n) {
    double *self = malloc(sizeof(*self));
    *self = n;
    return self;
}

Option divide(const double a, const double b) {
    if (0 == b) {
        return None;
    }
    return Option_some(Number(a / b));
}

Option squareRoot(void *out) {
    double *number = out;
    if (*number < 0) {
        return None;
    } else {
        *number = sqrt(*number);
        return Option_some(number);
    }
}

void *square(void *out) {
    double *number = out;
    *number = (*number) * (*number);
    return out;
}

void *zero(void) {
    return Number(0);
}

int main() {
    double *number = Option_fold(Option_chain(divide(20, 0), squareRoot), zero, square);
    printf("Result: %f\n", *number);
    free(number);
    return 0;
}
