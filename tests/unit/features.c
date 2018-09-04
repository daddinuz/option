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

#include <option.h>
#include <traits/traits.h>
#include "features.h"

Feature(None) {
    assert_true(Option_isNone(None));
    assert_false(Option_isSome(None));
}

Feature(Option_some) {
    const Option sut = Option_some("");
    assert_false(Option_isNone(sut));
    assert_true(Option_isSome(sut));

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const Option _ = Option_some(NULL);
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Option_fromNullable) {
    {
        const Option sut = Option_fromNullable(NULL);
        assert_true(Option_isNone(sut));
        assert_false(Option_isSome(sut));
    }

    {
        const Option sut = Option_fromNullable("");
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
    }
}

const void *mapNone(const void *_) {
    (void) _;
    assert_true(false);
    return NULL;
}

const void *mapSome(const void *_) {
    (void) _;
    return "B";
}

const void *mapSomeToNull(const void *_) {
    (void) _;
    return NULL;
}

Feature(Option_map) {
    {
        const Option sut = Option_map(None, mapNone);
        assert_true(Option_isNone(sut));
        assert_false(Option_isSome(sut));
    }

    {
        const Option sut = Option_map(Option_some("A"), mapSome);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "B");
    }

    {
        const Option sut = Option_map(Option_some("A"), mapSomeToNull);
        assert_true(Option_isNone(sut));
        assert_false(Option_isSome(sut));
    }
}

Option chainNone(const void *_) {
    (void) _;
    assert_true(false);
    return None;
}

Option chainSome(const void *_) {
    (void) _;
    return Option_some("B");
}

Option chainSomeToNone(const void *_) {
    (void) _;
    return None;
}

Feature(Option_chain) {
    {
        const Option sut = Option_chain(None, chainNone);
        assert_true(Option_isNone(sut));
        assert_false(Option_isSome(sut));
    }

    {
        const Option sut = Option_chain(Option_some("A"), chainSome);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "B");
    }

    {
        const Option sut = Option_chain(Option_some("A"), chainSomeToNone);
        assert_true(Option_isNone(sut));
        assert_false(Option_isSome(sut));
    }
}

Feature(Option_alt) {
    const Option alternative = Option_some("X");

    {
        const Option sut = Option_alt(None, alternative);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "X");
    }

    {
        const Option sut = Option_alt(Option_some("A"), alternative);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "A");
    }
}

Option orElse(void) {
    return Option_some("X");
}

Feature(Option_orElse) {
    {
        const Option sut = Option_orElse(None, orElse);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "X");
    }

    {
        const Option sut = Option_orElse(Option_some("A"), orElse);
        assert_false(Option_isNone(sut));
        assert_true(Option_isSome(sut));
        assert_string_equal(Option_unwrap(sut), "A");
    }
}

Feature(Option_unwrap) {
    Option sut = Option_some("A");

    const char *value = Option_unwrap(sut);
    assert_string_equal(value, "A");

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Option_unwrap(None);
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Option_unwrapAsMutable) {
    char value[] = "A";
    Option sut = Option_some(value);

    assert_equal(value, Option_unwrapAsMutable(sut));

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Option_unwrap(None);
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Option_expect) {
    Option sut = Option_some("A");

    const char *value = Option_expect(sut, "%s", "Expected a value");
    assert_string_equal(value, "A");

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Option_expect(None, "%s", "Expected a value");
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}

Feature(Option_expectAsMutable) {
    char value[] = "A";
    Option sut = Option_some(value);

    assert_equal(value, Option_expectAsMutable(sut, "%s", "Expected a value"));

    const size_t counter = traits_unit_get_wrapped_signals_counter();
    traits_unit_wraps(SIGABRT) {
        const char *_ = Option_expect(None, "%s", "Expected a value");
        (void) _;
    }
    assert_equal(traits_unit_get_wrapped_signals_counter(), counter + 1);
}
