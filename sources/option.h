/*
 * C Header File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 3, 2017
 */

/*
 * Option type trying to resemble Rust's Option.
 * Here Option is basically a wrapper around NULL values
 * in order make safer code avoiding to directly deref
 * pointers without NULL checking and getting segfaults.
 *
 * Any function or member that starts with underscores
 * should be treated as private and should not be used directly.
 */

#ifndef OPTION_INCLUDED
#define OPTION_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#define OPTION_VERSION          "0.2.1"
#define OPTION_VERSION_MAJOR    0
#define OPTION_VERSION_MINOR    2
#define OPTION_VERSION_PATCH    1

#include "mutable_option.h"
#include "immutable_option.h"

#ifdef __cplusplus
}
#endif

#endif /* OPTION_INCLUDED */
