/*
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

#pragma once

#include <stringify/stringify.h>
#include <trace/trace.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>

#if !defined(__GNUC__)
#define __attribute__(...)
#endif

static inline void
__traits_assert(bool condition, const char *__trace, const char *__assertion, const char *__format, ...)
__attribute__((nonnull(2, 3), __format__(__printf__, 4, 5)));

static inline void
__traits_assert(const bool condition, const char *const __trace, const char *const __assertion, const char *const __format, ...) {
    if (!condition) {
        fprintf(stderr, "At '%s'\r\nAssertion `%s` failed.\r\n", __trace, __assertion);
        if (NULL != __format) {
            va_list args;
            va_start(args, __format);
            vfprintf(stderr, __format, args);
            va_end(args);
        }
        abort();
    }
}

#define __traits_tokenJoinPhase2(x, ...)            x ## __VA_ARGS__
#define __traits_tokenJoin(x, ...)                  __traits_tokenJoinPhase2(x, __VA_ARGS__)

#define __traits_overloadPack1Selector(\
        A1, \
        _11, _12, _13, _14, _15, _16, _17, _18, \
        _21, _22, _23, _24, _25, _26, _27, _28, \
        _31, _32, _33, _34, _35, _36, _37, _38, \
        _41, _42, _43, _44, _45, _46, _47, _48, \
        N, ...)                                     N

#define __traits_overloadPack1(...)                 __traits_overloadPack1Selector(       \
                                                        __VA_ARGS__,                      \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        0, __UNUSED__                     \
                                                    )

#define __traits_overloadPack2Selector(\
        A1, A2, \
        _11, _12, _13, _14, _15, _16, _17, _18, \
        _21, _22, _23, _24, _25, _26, _27, _28, \
        _31, _32, _33, _34, _35, _36, _37, _38, \
        _41, _42, _43, _44, _45, _46, _47, _48, \
        N, ...)                                     N

#define __traits_overloadPack2(...)                 __traits_overloadPack2Selector(       \
                                                        __VA_ARGS__,                      \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        0, __UNUSED__                     \
                                                    )

#define __traits_overloadPack3Selector(\
        A1, A2, A3, \
        _11, _12, _13, _14, _15, _16, _17, _18, \
        _21, _22, _23, _24, _25, _26, _27, _28, \
        _31, _32, _33, _34, _35, _36, _37, _38, \
        _41, _42, _43, _44, _45, _46, _47, _48, \
        N, ...)                                     N

#define __traits_overloadPack3(...)                 __traits_overloadPack3Selector(       \
                                                        __VA_ARGS__,                      \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        1, 1, 1, 1, 1, 1, 1, 1,           \
                                                        0, __UNUSED__                     \
                                                    )

#define __traits_constIf0(ff, tt)                   ff
#define __traits_constIf1(ff, tt)                   tt
#define __traits_constIf(c)                         __traits_tokenJoin(__traits_constIf, c)

#define __traits_overload1(ff, tt, ...)             __traits_constIf(__traits_overloadPack1(__VA_ARGS__))(ff, tt)(__VA_ARGS__)
#define __traits_overload2(ff, tt, ...)             __traits_constIf(__traits_overloadPack2(__VA_ARGS__))(ff, tt)(__VA_ARGS__)
#define __traits_overload3(ff, tt, ...)             __traits_constIf(__traits_overloadPack3(__VA_ARGS__))(ff, tt)(__VA_ARGS__)

/*
 * Helpers
 */
#define eq(e, a)                                    ((e) == (a))
#define ne(e, a)                                    ((e) != (a))
#define gt(e, a)                                    ((e)  > (a))
#define ge(e, a)                                    ((e) >= (a))
#define lt(e, a)                                    ((e)  < (a))
#define le(e, a)                                    ((e) <= (a))

#define as(T, x)                                    ((T)(x))
#define eq_as(T, e, a)                              eq(as(T, e), as(T, a))
#define ne_as(T, e, a)                              ne(as(T, e), as(T, a))
#define gt_as(T, e, a)                              gt(as(T, e), as(T, a))
#define ge_as(T, e, a)                              ge(as(T, e), as(T, a))
#define lt_as(T, e, a)                              lt(as(T, e), as(T, a))
#define le_as(T, e, a)                              le(as(T, e), as(T, a))

/*
 * Basic
 */
#define __assert_that(c, ...)                       __traits_assert((c), TRACE, stringify_lazyQuote(c), __VA_ARGS__)
#define __assert_that0(c)                           __assert_that(c, NULL)
#define __assert_that1(c, ...)                      __assert_that(c, __VA_ARGS__)
#define assert_that(...)                            __traits_overload1(__assert_that0, __assert_that1, __VA_ARGS__)

#define traits_assert(...)                          assert_that(__VA_ARGS__)

/*
 * Boolean
 */
#define __assert_true(c, ...)                       __traits_assert(eq(true, c), TRACE, stringify_lazyQuote(c), __VA_ARGS__)
#define __assert_true0(c)                           __assert_true(c, "Expected to be true.\r\n")
#define __assert_true1(c, ...)                      __assert_true(c, __VA_ARGS__)
#define assert_true(...)                            __traits_overload1(__assert_true0, __assert_true1, __VA_ARGS__)

#define __assert_false(c, ...)                      __traits_assert(eq(false, c), TRACE, stringify_lazyQuote(c), __VA_ARGS__)
#define __assert_false0(c)                          __assert_false(c, "Expected to be false.\r\n")
#define __assert_false1(c, ...)                     __assert_false(c, __VA_ARGS__)
#define assert_false(...)                           __traits_overload1(__assert_false0, __assert_false1, __VA_ARGS__)

/*
 * Numerical
 */
#define __assert_equal(e, a, ...)                   __traits_assert(eq(e, a), TRACE, stringify_lazyQuote(eq(e, a)), __VA_ARGS__)
#define __assert_equal0(e, a)                       __assert_equal(e, a, "Expected to be equal.\r\n")
#define __assert_equal1(e, a, ...)                  __assert_equal(e, a, __VA_ARGS__)
#define assert_equal(...)                           __traits_overload2(__assert_equal0, __assert_equal1, __VA_ARGS__)

#define __assert_not_equal(e, a, ...)               __traits_assert(ne(e, a), TRACE, stringify_lazyQuote(ne(e, a)), __VA_ARGS__)
#define __assert_not_equal0(e, a)                   __assert_not_equal(e, a, "Expected to be not equal.\r\n")
#define __assert_not_equal1(e, a, ...)              __assert_not_equal(e, a, __VA_ARGS__)
#define assert_not_equal(...)                       __traits_overload2(__assert_not_equal0, __assert_not_equal1, __VA_ARGS__)

#define __assert_greater(e, a, ...)                 __traits_assert(gt(e, a), TRACE, stringify_lazyQuote(gt(e, a)), __VA_ARGS__)
#define __assert_greater0(e, a)                     __assert_greater(e, a, "Expected to be greater.\r\n")
#define __assert_greater1(e, a, ...)                __assert_greater(e, a, __VA_ARGS__)
#define assert_greater(...)                         __traits_overload2(__assert_greater0, __assert_greater1, __VA_ARGS__)

#define __assert_greater_equal(e, a, ...)           __traits_assert(ge(e, a), TRACE, stringify_lazyQuote(ge(e, a)), __VA_ARGS__)
#define __assert_greater_equal0(e, a)               __assert_greater_equal(e, a, "Expected to be greater or equal.\r\n")
#define __assert_greater_equal1(e, a, ...)          __assert_greater_equal(e, a, __VA_ARGS__)
#define assert_greater_equal(...)                   __traits_overload2(__assert_greater_equal0, __assert_greater_equal1, __VA_ARGS__)

#define __assert_less(e, a, ...)                    __traits_assert(lt(e, a), TRACE, stringify_lazyQuote(lt(e, a)), __VA_ARGS__)
#define __assert_less0(e, a)                        __assert_less(e, a, "Expected to be less.\r\n")
#define __assert_less1(e, a, ...)                   __assert_less(e, a, __VA_ARGS__)
#define assert_less(...)                            __traits_overload2(__assert_less0, __assert_less1, __VA_ARGS__)

#define __assert_less_equal(e, a, ...)              __traits_assert(le(e, a), TRACE, stringify_lazyQuote(le(e, a)), __VA_ARGS__)
#define __assert_less_equal0(e, a)                  __assert_less_equal(e, a, "Expected to be less or equal.\r\n")
#define __assert_less_equal1(e, a, ...)             __assert_less_equal(e, a, __VA_ARGS__)
#define assert_less_equal(...)                      __traits_overload2(__assert_less_equal0, __assert_less_equal1, __VA_ARGS__)

/*
 * Pointer
 */
#define __assert_null(x, ...)                       __traits_assert(eq(NULL, x), TRACE, stringify_lazyQuote(x), __VA_ARGS__)
#define __assert_null0(x)                           __assert_null(x, "Expected to be null.\r\n")
#define __assert_null1(x, ...)                      __assert_null(x, __VA_ARGS__)
#define assert_null(...)                            __traits_overload1(__assert_null0, __assert_null1, __VA_ARGS__)

#define __assert_not_null(x, ...)                   __traits_assert(ne(NULL, x), TRACE, stringify_lazyQuote(x), __VA_ARGS__)
#define __assert_not_null0(x)                       __assert_not_null(x, "Expected to be not null.\r\n")
#define __assert_not_null1(x, ...)                  __assert_not_null(x, __VA_ARGS__)
#define assert_not_null(...)                        __traits_overload1(__assert_not_null0, __assert_not_null1, __VA_ARGS__)

/*
 * Memory
 */
#define __assert_memory_equal(e, a, s, ...)         __traits_assert(eq(0, memcmp(e, a, s)), TRACE, stringify_lazyQuote(eq(0, memcmp(e, a, s))), __VA_ARGS__)
#define __assert_memory_equal0(e, a, s)             __assert_memory_equal(e, a, s, NULL)
#define __assert_memory_equal1(e, a, s, ...)        __assert_memory_equal(e, a, s, __VA_ARGS__)
#define assert_memory_equal(...)                    __traits_overload3(__assert_memory_equal0, __assert_memory_equal1, __VA_ARGS__)

#define __assert_memory_not_equal(e, a, s, ...)     __traits_assert(ne(0, memcmp(e, a, s)), TRACE, stringify_lazyQuote(ne(0, memcmp(e, a, s))), __VA_ARGS__)
#define __assert_memory_not_equal0(e, a, s)         __assert_memory_not_equal(e, a, s, NULL)
#define __assert_memory_not_equal1(e, a, s, ...)    __assert_memory_not_equal(e, a, s, __VA_ARGS__)
#define assert_memory_not_equal(...)                __traits_overload3(__assert_memory_not_equal0, __assert_memory_not_equal1, __VA_ARGS__)

/*
 * String
 */
#define __assert_string_equal(e, a, ...)            __traits_assert(eq(0, strcmp(e, a)), TRACE, stringify_lazyQuote(eq(0, strcmp(e, a))), __VA_ARGS__)
#define __assert_string_equal0(e, a)                __assert_string_equal(e, a, "Expected to be equal.\r\n")
#define __assert_string_equal1(e, a, ...)           __assert_string_equal(e, a, __VA_ARGS__)
#define assert_string_equal(...)                    __traits_overload2(__assert_string_equal0, __assert_string_equal1, __VA_ARGS__)

#define __assert_string_not_equal(e, a, ...)        __traits_assert(ne(0, strcmp(e, a)), TRACE, stringify_lazyQuote(ne(0, strcmp(e, a))), __VA_ARGS__)
#define __assert_string_not_equal0(e, a)            __assert_string_not_equal(e, a, "Expected to be not equal.\r\n")
#define __assert_string_not_equal1(e, a, ...)       __assert_string_not_equal(e, a, __VA_ARGS__)
#define assert_string_not_equal(...)                __traits_overload2(__assert_string_not_equal0, __assert_string_not_equal1, __VA_ARGS__)

#ifdef __cplusplus
}
#endif
