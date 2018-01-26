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

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <option.h>

/*
 *
 */
#define truth(x) \
    ((x) ? "true" : "false")

const Error OutOfMemory = Error_new("Out of memory");
const Error TooLongString = Error_new("Too long string");

static ResultOf(char *, OutOfMemory, TooLongString)
SmallString_new(const char *literal);

/*
 *
 */
int main() {
    ResultOf(char *, OutOfMemory, TooLongString) result = SmallString_new("Hello World!");

    printf("isOk: %s\n", truth(Result_isOk(result)));
    printf("isError: %s\n", truth(Result_isError(result)));
    printf("data: %s\n", (char *) Result_unwrap(result));
    assert(Ok == Result_inspect(result));

    puts("----------------------------");

    result = SmallString_new("Too long string that will take too much memory");

    printf("isOk: %s\n", truth(Result_isOk(result)));
    printf("isError: %s\n", truth(Result_isError(result)));
    printf("error: %s\n", Result_explain(result));
    printf("details: %s\n", Result_details(result));
    // printf("data: %s\n", (const char *) Result_unwrap(result)); /* this would abort */
    assert(TooLongString == Result_inspect(result));

    /* ------------------------------- */

    result = Result_ok(NULL);                /* It's perfectly fine with Result unwrap an Ok value containing NULL. */
    assert(NULL == Result_expect(result, "%s", "this should be NULL")); /* If it would be an Option it would abort. */

    return 0;
}

/*
 *
 */
ResultOf(char *, OutOfMemory, TooLongString) SmallString_new(const char *literal) {
    assert(literal);

    if (strlen(literal) > 16) {
        return Result_errorWithDetails(TooLongString, "Got a literal longer than 16 characters");
    }

    OptionOf(char *) optionalString = Option_new(strdup(literal));
    return Option_isSome(optionalString) ? Result_ok(Option_unwrap(optionalString)) : Result_error(OutOfMemory);
}
