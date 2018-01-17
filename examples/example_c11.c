/*
 * C Source File
 *
 * Author: daddinuz
 * email:  daddinuz@gmail.com
 * Date:   January 12, 2018 
 */

#include <stdio.h>
#include <option.h>

int main() {
    printf("%s %s!\n",
           (const char *) unwrap(Result_ok("Hello")),
           (const char *) unwrap(Option_new("World")));

    printf("%s %s!\n",
           (const char *) expect(Result_ok("Hello"), "Expected a string."),
           (const char *) expect(Option_new("World"), "Expected a string."));

    unwrap(Ok);
    expect(Ok, "Expected no errors");
}
