#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../src/str.h"


typedef void (*test_func)();


static char *create_str(const char *str)
{
    char *new_str = malloc(strlen(str) + 1);
    strcpy(new_str, str);
    return new_str;
}


static void test_init()
{
    struct str_t str = {3, 4, "Hello, World!"};
    str_init(&str);

    assert(str.len == 0);
    assert(str.size == 16);
    assert(str.data != NULL);
    assert(strlen(str.data) == 0);

    free(str.data);
}


static void test_clear()
{
    struct str_t str;
    str.data = create_str("Hello, World!");
    str.len = strlen(str.data);
    str.size = str.len + 1;

    str_clear(&str);

    assert(str.len == 0);
    assert(str.size == 14);
    assert(strlen(str.data) == 0);

    free(str.data);
}


static void test_del()
{
    struct str_t str;
    str.data = create_str("Hello, World!");
    str.len = strlen(str.data);
    str.size = str.len + 1;

    str_del(&str);

    assert(str.len == 0);
    assert(str.size == 0);
    assert(str.data == NULL);
}


static void test_append()
{
    int i;
    struct str_t str;
    const char hello[] = "Hello, my name is DiSkills";
    str_init(&str);

    for (i = 0; i < 15; i++) {
        str_append(hello[i], &str);
    }
    assert(str.len == 15);
    assert(str.size == 16);
    assert(strcmp(str.data, "Hello, my name ") == 0);

    for (; i < sizeof(hello) - 1; i++) {
        str_append(hello[i], &str);
    }
    assert(str.len == strlen(hello));
    assert(str.size == 32);
    assert(strcmp(str.data, hello) == 0);

    str_del(&str);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init, test_clear, test_del, test_append
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
