/*
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 *
 * Copyright (c) 2018 Davide Di Carlo
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

#ifndef OPTION_INCLUDED
#define OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define OPTION_VERSION          "0.11.3"
#define OPTION_VERSION_MAJOR    0
#define OPTION_VERSION_MINOR    11
#define OPTION_VERSION_PATCH    3

#include <stddef.h>
#include <stdbool.h>

#if !(defined(__GNUC__) || defined(__clang__))
#define __attribute__(...)
#endif

/**
 * Option represents encapsulation of an optional value; it is used as the return type of functions which may or may
 * not return a meaningful value when they are applied.
 *
 * @attention
 *  The `struct __Option` type must be treated as opaque therefore should be used directly.
 *  Option type must be treated as opaque therefore should be used only through the functions exported by this interface,
 *  its members should never be accessed directly.
 */
typedef struct __Option {
    void *__data;
} Option;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention the annotated type is the wrapped data type.
 */
#define OptionOf(xType) \
    Option

/**
 * The None Option instance used instead of NULL to represent the absence of a value.
 */
extern const Option None;

/**
 * Creates an Option.
 *
 * @param data The optional value.
 * @return None if data is NULL else a new Option instance wrapping data.
 */
extern Option
Option_new(void *data)
__attribute__((__warn_unused_result__));

/**
 * Tests if Option is None
 *
 * @param self The Option instance.
 * @return false if Option is None else true.
 */
extern bool
Option_isSome(Option self)
__attribute__((__warn_unused_result__));

/**
 * Tests if Option is None
 *
 * @param self The Option instance.
 * @return true if Option is None else false.
 */
extern bool
Option_isNone(Option self)
__attribute__((__warn_unused_result__));

/**
 * Unwraps an Option, yielding its wrapped value if it's not None.
 * Aborts execution if Option is None with a custom panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Option instance.
 * @param format The custom panic message.
 * @param ... The format params.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Option_expect(const char *__file, size_t __line, Option self, const char *format, ...)
__attribute__((__warn_unused_result__, __nonnull__(1, 4), __format__(__printf__, 4, 5)));

/**
 * @see __Option_expect(const char *__file, size_t __line, Option self, const char *format, ...)
 */
#define Option_expect(xSelf, ...) \
  __Option_expect(__FILE__, __LINE__, (xSelf), __VA_ARGS__)

/**
 * Unwraps an Option, yielding its wrapped value if it's not None.
 * Aborts execution if Option is None with a pre-defined panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Option instance.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Option_unwrap(const char *__file, size_t __line, Option self)
__attribute__((__warn_unused_result__, __nonnull__(1)));

/**
 * @see __Option_unwrap(const char *__file, size_t __line, Option self)
 */
#define Option_unwrap(xSelf) \
  __Option_unwrap(__FILE__, __LINE__, (xSelf))

/**
 * Maps self applying mapFn.
 *
 * @param self The Option instance.
 * @param mapFn The map function [<b>must not be NULL</b>].
 * @return The mapped Option instance.
 */
extern Option
Option_map(Option self, Option mapFn(Option))
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Maps self applying mapFn if self is not None, otherwise returns a default Option.
 *
 * @param self The Option instance.
 * @param def The default option.
 * @param mapFn The map function [<b>must not be NULL</b>].
 * @return The mapped Option instance.
 */
extern Option
Option_mapOr(Option self, Option def, Option mapFn(Option))
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Maps self applying mapFn if self is not None, otherwise compute a default Option applying defFn.
 *
 * @param self The Option instance.
 * @param defFn The default function to compute if self is None [<b>must not be NULL</b>].
 * @param mapFn The map function to apply if self is not None [<b>must not be NULL</b>].
 * @return The mapped Option instance.
 */
extern Option
Option_mapOrElse(Option self, Option defFn(void), Option mapFn(Option))
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Error represents recoverable errors that may occur at runtime.
 * Every error instance must be a singleton, in order to check if two errors are equal a simple comparison
 * between pointers can be done.
 *
 * @attention
 *  The `struct __Error` type must be treated as opaque therefore should be used directly.
 */
typedef struct __Error {
    const char *message;
} const *Error;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention the annotations are the errors that may be returned.
 */
#define OneOf(...) \
    Error

/**
 * Helper macro to create new singleton errors.
 * This macro should be used only to define new global errors, and Errors defined with this macro must be declared const:
 *
 * @code
 * const Error OutOfMemoryError = Error_new("Out of memory");
 * @endcode
 */
#define Error_new(xMessage) \
     ((Error) &((const struct __Error) {.message=(xMessage)}))

/**
 * The Ok Error instance to notify a successful execution.
 */
extern const Error Ok;

/**
 * Unwraps an Error, aborts execution if Error is not Ok with a custom panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Error instance.
 * @param format The custom panic message.
 * @param ... The format params.
 */
extern void
__Error_expect(const char *__file, size_t __line, Error self, const char *format, ...)
__attribute__((__nonnull__(1, 4), __format__(__printf__, 4, 5)));

/**
 * @see __Error_expect(const char *__file, size_t __line, Error self, const char *format, ...)
 */
#define Error_expect(xSelf, ...) \
  __Error_expect(__FILE__, __LINE__, (xSelf), __VA_ARGS__)

/**
 * Unwraps an Error, aborts execution if Error is not Ok with a pre-defined panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Error instance.
 */
extern void
__Error_unwrap(const char *__file, size_t __line, Error self)
__attribute__((__nonnull__(1)));

/**
 * @see __Error_unwrap(const char *__file, size_t __line, Error self)
 */
#define Error_unwrap(xSelf) \
  __Error_unwrap(__FILE__, __LINE__, (xSelf))

/**
 * Result holds a returned value or an error code providing a way of handling errors, without resorting to exception
 * handling; when a function that may fail returns a result type, the programmer is forced to consider success or failure
 * paths, before getting access to the expected result; this eliminates the possibility of an erroneous programmer assumption.
 *
 * @attention
 *  The `struct __Result` type must be treated as opaque therefore should be used directly.
 *  Result type must be treated as opaque therefore should be used only through the functions exported by this interface,
 *  its members should never be accessed directly.
 */
typedef struct __Result {
    void *__data;
    Error __error;
    const char *__details;
} Result;

/**
 * An helper macro used for type hinting, useful when writing interfaces.
 * By convention the first annotated type is the wrapped data type, every types following are Error types.
 */
#define ResultOf(...) \
    Result

/**
 * Creates a Result notifying a successful execution.
 *
 * @param data The result value.
 * @return A new Result instance wrapping data.
 */
extern Result
Result_ok(void *data)
__attribute__((__warn_unused_result__));

/**
 * Creates a Result notifying a unsuccessful execution.
 *
 * @param error The Error instance.
 * @return A new Result instance wrapping an error.
 */
extern Result
Result_error(Error error)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Creates a Result notifying a unsuccessful execution specifying reason.
 *
 * @param error The Error instance.
 * @param details A detailed explanation of the result.
 * @return A new Result instance wrapping an error.
 */
extern Result
Result_errorWithDetails(Error error, const char *details)
__attribute__((__warn_unused_result__, __nonnull__));

/**
 * Tests if Result is Ok
 *
 * @param self The Result instance.
 * @return true if Result is Ok else false.
 */
extern bool
Result_isOk(Result self)
__attribute__((__warn_unused_result__));

/**
 * Tests if Result is Error
 *
 * @param self The Result instance.
 * @return true if Result is Error else false.
 */
extern bool
Result_isError(Result self)
__attribute__((__warn_unused_result__));

/**
 * Unwraps a Result, yielding its wrapped value if it's Ok .
 * Aborts execution if Result is Error with a custom panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Result instance.
 * @param format The custom panic message (overrides the associated Error panic message).
 * @param ... The format params.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Result_expect(const char *__file, size_t __line, Result self, const char *format, ...)
__attribute__((__warn_unused_result__, __nonnull__(1, 4), __format__(__printf__, 4, 5)));

/**
 * @see __Result_expect(const char *__file, size_t __line, Result self, const char *format, ...)
 */
#define Result_expect(xSelf, ...) \
  __Result_expect(__FILE__, __LINE__, (xSelf), __VA_ARGS__)

/**
 * Unwraps a Result, yielding its wrapped value if it's Ok .
 * Aborts execution if Result is Error with the error defined panic message.
 *
 * @attention
 *  This function should never be used directly, used the exported macro instead.
 *
 * @param __file The file name.
 * @param __line The line number.
 * @param self The Result instance.
 * @return The unwrapped value or terminates the execution.
 */
extern void *
__Result_unwrap(const char *__file, size_t __line, Result self)
__attribute__((__warn_unused_result__, __nonnull__(1)));

/**
 * @see __Result_unwrap(const char *__file, size_t __line, Result self)
 */
#define Result_unwrap(xSelf) \
  __Result_unwrap(__FILE__, __LINE__, (xSelf))

/**
 * Inspects the result error.
 *
 * @param self The Result instance.
 * @return The Error associated to the result.
 */
extern Error
Result_inspect(Result self)
__attribute__((__warn_unused_result__));

/**
 * Explains the result error.
 *
 * @param self The Result instance.
 * @return The Error message.
 */
extern const char *
Result_explain(Result self)
__attribute__((__warn_unused_result__));

/**
 * A detailed explanation of the result error.
 *
 * @param self The Result instance.
 * @return A detailed explanation of the error.
 */
extern const char *
Result_details(Result self)
__attribute__((__warn_unused_result__));

/*
 * C11 Support
 */
#if !defined(OPTION_DISABLE_C11_SUPPORT) && defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
    #define expect(x, ...)                  \
        _Generic((x),                       \
            Option  :   __Option_expect,    \
            Result  :   __Result_expect,    \
            Error   :   __Error_expect      \
        )(__FILE__, __LINE__, (x), __VA_ARGS__)

    #define unwrap(x)                       \
        _Generic((x),                       \
            Option  :   __Option_unwrap,    \
            Result  :   __Result_unwrap,    \
            Error   :   __Error_unwrap      \
        )(__FILE__, __LINE__, (x))
#endif

#ifdef __cplusplus
}
#endif

#endif /* OPTION_INCLUDED */
