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
#include <assert.h>

typedef const double *Number;

static Number zero(void);
static Number Number_new(double number);

static OptionOf(Number) ln(Number number);
static OptionOf(Number) division(Number dividend, Number divisor);
static OptionOf(Number) squareRoot(Number number);

int main() {
    Number number = Option_fold(Option_chain(division(Number_new(32), Number_new(4)), squareRoot), zero, ln);
    printf("Number is: %f\n", *number);
    return 0;
}

/*
 *
 */
static double numbers[4] = {};
static double *numbersCursor = numbers;
static const double *numbersEnd = numbers + sizeof(numbers) / sizeof(numbers[0]);

Number zero(void) {
    static const double instance = 0;
    return &instance;
}

Number Number_new(const double number) {
    assert(numbersCursor <= numbersEnd);
    return (0 == number) ? zero() : (*numbersCursor = number, numbersCursor++);
}

OptionOf(Number) ln(Number number) {
    return *number < 0 ? None : Option_some(Number_new(log(*number)));
}

OptionOf(Number) division(Number dividend, Number divisor) {
    return 0 == divisor ? None : Option_some(Number_new(*dividend / *divisor));
}

OptionOf(Number) squareRoot(Number number) {
    return *number < 0 ? None : Option_some(Number_new(sqrt(*number)));
}
