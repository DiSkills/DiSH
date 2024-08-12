#include <assert.h>
#include <string.h>

#include "../src/str.h"


static void test_str_append()
{
    char c;
    struct str_t str;

    str_init(&str);
    for (c = 'a'; c <= 'a' + 14; c++) {
        str_append(c, &str);
    }
    assert(str.len == 15);
    assert(str.size == 16);
    assert(strcmp(str.data, "abcdefghijklmno") == 0);

    str_append('p', &str);
    assert(str.len == 16);
    assert(str.size == 32);
    assert(strcmp(str.data, "abcdefghijklmnop") == 0);

    str_del(&str);
}


int main()
{
    test_str_append();
    return 0;
}
