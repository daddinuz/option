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

#pragma once

#include <stdbool.h>

#if !(defined(__GNUC__) || defined(__clang__))
__attribute__(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define OPTION_VERSION_MAJOR        1
#define OPTION_VERSION_MINOR        0
#define OPTION_VERSION_PATCH        0
#define OPTION_VERSION_SUFFIX       ""
#define OPTION_VERSION_IS_RELEASE   0
#define OPTION_VERSION_HEX          0x010000

/**
 * An option-type or maybe-type is a polymorphic type that represents encapsulation of an optional value;
 * e.g. it is used as the return type of functions which may or may not return a meaningful value when they are applied.
 */

/**
 * @attention this struct must be treated as opaque therefore its members must not be accessed directly.
 */
typedef struct {
    const void *__value;
} Option;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention the annotated type is the wrapped value type.
 */
#define OptionOf(type) \
    Option

/**
 * The `None` instance used to represent the absence of a value.
 */
extern const Option None;

/**
 * Constructs a new `Option` wrapping a value.
 *
 * @attention value must not be `NULL`.
 */
extern Option Option_some(const void *value)
__attribute__((__warn_unused_result__));

/**
 * Constructs a new `Option` from a nullable pointer.
 * If the value is `NULL`, returns `None`, otherwise returns the value wrapped in a `Option`
 */
extern Option Option_fromNullable(const void *value)
__attribute__((__warn_unused_result__));

/**
 * Returns `true` if this `Option` is `None`, `false` otherwise.
 */
extern bool Option_isNone(Option self)
__attribute__((__warn_unused_result__));

/**
 * Returns `true` if this `Option` is wrapping a value, `false` otherwise.
 */
extern bool Option_isSome(Option self)
__attribute__((__warn_unused_result__));

/**
 * Returns an `Option` wrapping the result of f applied to the value of this `Option` if this `Option` is not `None` else `None`.
 * If f returns `NULL`, this functions will return `None`.
 *
 * @attention f must not be `NULL`.
 */
extern Option Option_map(Option self, const void *f(const void *))
__attribute__((__warn_unused_result__));

/**
 * Chains several possibly failing computations.
 *
 * @attention f must not be `NULL`.
 */
extern Option Option_chain(Option self, Option f(const void *))
__attribute__((__warn_unused_result__));

/**
 * If this `Option` is wrapping a value then it will be returned, else the next `Option` will be returned.
 */
extern Option Option_alt(Option self, Option other)
__attribute__((__warn_unused_result__));

/**
 * Lazy version of `Option_alt(...)`.
 *
 * @attention f must not be `NULL`.
 */
extern Option Option_orElse(Option self, Option f(void))
__attribute__((__warn_unused_result__));

/**
 * Unwraps the value of this `Option` if it's an `OptionSome` or panics if this is an `OptionNone`.
 */
#define Option_unwrap(self) \
    __Option_unwrap((__FILE__), (__LINE__), (self))

/**
 * Unwraps as mutable the value of this `Option` if it's an `OptionSome` or panics if this is an `OptionNone`.
 */
#define Option_unwrapAsMutable(self) \
    __Option_unwrapAsMutable((__FILE__), (__LINE__), (self))

/**
 * Unwraps the value of this `Option` if it's an `OptionSome` or panics if this is an `OptionNone` and prints a custom message.
 */
#define Option_expect(self, ...) \
    __Option_expect((__FILE__), (__LINE__), (self), __VA_ARGS__)

/**
 * Unwraps as mutable the value of this `Option` if it's an `OptionSome` or panics if this is an `OptionNone` and prints a custom message.
 */
#define Option_expectAsMutable(self, ...) \
    __Option_expectAsMutable((__FILE__), (__LINE__), (self), __VA_ARGS__)

/**
 * @attention this function must be treated as opaque therefore must not be called directly.
 */
extern const void *__Option_unwrap(const char *file, int line, Option self)
__attribute__((__nonnull__(1)));

/**
 * @attention this function must be treated as opaque therefore must not be called directly.
 */
extern void *__Option_unwrapAsMutable(const char *file, int line, Option self)
__attribute__((__nonnull__(1)));

/**
 * @attention this function must be treated as opaque therefore must not be called directly.
 */
extern const void *__Option_expect(const char *file, int line, Option self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

/**
 * @attention this function must be treated as opaque therefore must not be called directly.
 */
extern void *__Option_expectAsMutable(const char *file, int line, Option self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

#ifdef __cplusplus
}
#endif
