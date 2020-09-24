/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Davide Di Carlo
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

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
