/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   December 3, 2017
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <option.h>

/*
 *
 */
#define assert_eq(expected, actual)         assert((expected) == (actual))
#define assert_eq_str(expected, actual)     assert_eq(0, strcmp((char *) (expected), (char *) (actual)))

/*
 *
 */
typedef char *String_T;

static MutableOptional(String_T) String_new(const char *s);
static void String_delete(String_T s);

static MutableOptional(const char *) Greet(MutableOptional(String_T) s);
static MutableOptional(const char *) DidNotGetIt(void);

/*
 *
 */
int main() {
    MutableOptional(String_T) string = String_new("Hello, World!");

    printf("  Some: %s\n", MutableOption_isSome(string) ? "true" : "false");
    printf("  None: %s\n", MutableOption_isNone(string) ? "true" : "false");
    printf("string: %s\n", (String_T) MutableOption_expect(string, "Needed a string"));

    assert_eq(true, MutableOption_isNone(MutableOption_new(NULL)));
    assert_eq_str("Hello!", MutableOption_unwrap(MutableOption_map(MutableOption_new("Hi!"), Greet)));
    assert_eq_str("Bla Bla Bla", MutableOption_unwrap(MutableOption_mapOr(MutableOption_None, MutableOption_new("Bla Bla Bla"), Greet)));
    assert_eq_str("Oops, didn't get it.", MutableOption_unwrap(MutableOption_mapOrElse(MutableOption_None, DidNotGetIt, Greet)));
    assert_eq_str("Stop bother me!.", MutableOption_unwrap(MutableOption_mapOrElse(MutableOption_None, DidNotGetIt, Greet)));
    assert_eq_str("Goodbye!", MutableOption_unwrap(MutableOption_map(MutableOption_new("Bye!"), Greet)));

    String_delete(MutableOption_unwrap(string));
    return EXIT_SUCCESS;
}

/*
 *
 */
MutableOptional(String_T) String_new(const char *s) {
    return MutableOption_new(strdup(s));
}

void String_delete(String_T s) {
    free(s);
}

MutableOptional(const char *) Greet(MutableOptional(String_T) s) {
    String_T str = MutableOption_unwrap(s);
    if (strcmp(str, "Hi!") == 0) {
        return MutableOption_new("Hello!");
    } else if (strcmp(str, "Bye!") == 0) {
        return MutableOption_new("Goodbye!");
    }
    return MutableOption_None;
}

MutableOptional(const char *) DidNotGetIt(void) {
    static int i = -1;
    i++;
    return MutableOption_new((i <= 0) ? "Oops, didn't get it." : "Stop bother me!.");
}
