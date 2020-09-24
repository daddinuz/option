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

#include <traits/traits.h>
#include "features.h"

option_define(OptionalNumber, double);

Feature(Option_some) {
    const double value = 5.0;
    OptionalNumber sut = OptionalNumber_some(value);

    assert_that(OptionalNumber_isSome(&sut));
    assert_false(OptionalNumber_isNone(&sut));

    {
        const double number = OptionalNumber_unwrap(&sut);
        assert_equal(value, number, "%f %f\n", value, number);
    }

    {
        const double number = OptionalNumber_expect(&sut, "expected a value");
        assert_equal(value, number);
    }
}

Feature(Option_none) {
    OptionalNumber sut = OptionalNumber_none();

    assert_that(OptionalNumber_isNone(&sut));
    assert_false(OptionalNumber_isSome(&sut));

    const size_t sigcount = traitsUnit_getWrappedSignalsCounter();

    traitsUnit_wrap(SIGABRT) {
        const double value = OptionalNumber_unwrap(&sut);
        (void) value;
    }

    assert_equal(sigcount + 1, traitsUnit_getWrappedSignalsCounter());

    traitsUnit_wrap(SIGABRT) {
        const double value = OptionalNumber_expect(&sut, "expected a value");
        (void) value;
    }

    assert_equal(sigcount + 2, traitsUnit_getWrappedSignalsCounter());
}
