/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 03, 2017 
 */

#ifndef OPTION_IMMUTABLE_OPTION_INCLUDED
#define OPTION_IMMUTABLE_OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

/**
 * ImmutableOption type should never be used directly.
 * Must be treated as opaque.
 */
typedef struct ImmutableOption_T {
    const void *__some;
} ImmutableOption;

/**
 * Just for type hinting
 */
#define ImmutableOptional(Type) ImmutableOption

/**
 * The None ImmutableOption instance
 */
extern const ImmutableOption ImmutableOption_None;

/**
 * Creates an ImmutableOption starting from some value.
 * If @param some is NULL returns ImmutableOption_None.
 *
 * @param some The option value.
 * @return A new ImmutableOption instance or ImmutableOption_None.
 */
extern ImmutableOption
ImmutableOption_new(const void *some);

/**
 * Returns true if the option is a Some value.
 *
 * @param self The ImmutableOption instance.
 * @return true if is Some value else false.
 */
extern bool
ImmutableOption_isSome(ImmutableOption self);

/**
 * Returns true if the option is a None value.
 *
 * @param self The ImmutableOption instance.
 * @return true if is None value else false.
 */
extern bool
ImmutableOption_isNone(ImmutableOption self);

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a custom panic message.
 *
 * @param self The ImmutableOption instance.
 * @param message The custom error message.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern const void *
__ImmutableOption_expect(const char *__file, size_t __line, ImmutableOption self, const char *message);

#define ImmutableOption_expect(Self, Message)   \
    __ImmutableOption_expect(__FILE__, __LINE__, (Self), (Message))

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a predefined panic message.
 *
 * @param self The ImmutableOption instance.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern const void *
__ImmutableOption_unwrap(const char *__file, size_t __line, ImmutableOption self);

#define ImmutableOption_unwrap(Self)            \
    __ImmutableOption_unwrap(__FILE__, __LINE__, (Self))

/**
 * Maps an ImmutableOption<T> to ImmutableOption<U> by applying a function to a contained value.
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 * 
 * @param self The ImmutableOption instance.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_map(ImmutableOption self, ImmutableOption mapFn(ImmutableOption));

/**
 * Applies a function to the contained value (if any), or returns a default (if not).
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The ImmutableOption instance.
 * @param def The default option if self is None.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_mapOr(ImmutableOption self, ImmutableOption def, ImmutableOption mapFn(ImmutableOption));

/**
 * Applies a function to the contained value (if any), or computes a default (if not).
 *
 * Checked runtime errors:
 *      - @param defFn must not be NULL.
 *      - @param mapFn must not be NULL.
 * 
 * @param self The ImmutableOption instance.
 * @param def The default function to compute if self is None.
 * @param mapFn The map function.
 * @return A new ImmutableOption instance.
 */
extern ImmutableOption
ImmutableOption_mapOrElse(ImmutableOption self, ImmutableOption defFn(void), ImmutableOption mapFn(ImmutableOption));

#ifdef __cplusplus
}
#endif

#endif /* OPTION_IMMUTABLE_OPTION_INCLUDED */
