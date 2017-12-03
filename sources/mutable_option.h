/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 03, 2017 
 */

#ifndef OPTION_MUTABLE_OPTION_INCLUDED
#define OPTION_MUTABLE_OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdbool.h>

/**
 * MutableOption type should never be used directly.
 * Must be treated as opaque.
 */
typedef struct MutableOption_T {
    void *__some;
} MutableOption;

/**
 * Just for type hinting
 */
#define MutableOptional(Type) MutableOption

/**
 * The None MutableOption instance
 */
extern const MutableOption MutableOption_None;

/**
 * Creates an MutableOption starting from some value.
 * If @param some is NULL returns MutableOption_None.
 *
 * @param some The option value.
 * @return A new MutableOption instance or MutableOption_None.
 */
extern MutableOption
MutableOption_new(void *some);

/**
 * Returns true if the option is a Some value.
 *
 * @param self The MutableOption instance.
 * @return true if is Some value else false.
 */
extern bool
MutableOption_isSome(MutableOption self);

/**
 * Returns true if the option is a None value.
 *
 * @param self The MutableOption instance.
 * @return true if is None value else false.
 */
extern bool
MutableOption_isNone(MutableOption self);

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a custom panic message.
 *
 * @param self The MutableOption instance.
 * @param message The custom error message.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern void *
__MutableOption_expect(const char *__file, size_t __line, MutableOption self, const char *message);

#define MutableOption_expect(Self, Message)   \
    __MutableOption_expect(__FILE__, __LINE__, (Self), (Message))

/**
 * Unwraps an option, yielding the content of a Some.
 * Panics if the value is a None with a predefined panic message.
 *
 * @param self The MutableOption instance.
 * @param file The file name.
 * @param line The line number.
 * @return The unwrapped option or terminates the execution.
 */
extern void *
__MutableOption_unwrap(const char *__file, size_t __line, MutableOption self);

#define MutableOption_unwrap(Self)            \
    __MutableOption_unwrap(__FILE__, __LINE__, (Self))

/**
 * Maps an MutableOption<T> to MutableOption<U> by applying a function to a contained value.
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_map(MutableOption self, MutableOption mapFn(MutableOption));

/**
 * Applies a function to the contained value (if any), or returns a default (if not).
 *
 * Checked runtime errors:
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param def The default option if self is None.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_mapOr(MutableOption self, MutableOption def, MutableOption mapFn(MutableOption));

/**
 * Applies a function to the contained value (if any), or computes a default (if not).
 *
 * Checked runtime errors:
 *      - @param defFn must not be NULL.
 *      - @param mapFn must not be NULL.
 *
 * @param self The MutableOption instance.
 * @param def The default function to compute if self is None.
 * @param mapFn The map function.
 * @return A new MutableOption instance.
 */
extern MutableOption
MutableOption_mapOrElse(MutableOption self, MutableOption defFn(void), MutableOption mapFn(MutableOption));

#ifdef __cplusplus
}
#endif

#endif /* OPTION_MUTABLE_OPTION_INCLUDED */
