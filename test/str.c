#include <assert.h>
#include <string.h>

#include "../src/str.h"


typedef void (*test_func)();


static void test_init_from_array()
{
    const char *hello = "Hello, World!";
    struct str_t str = { 2, 3, "Hi" };

    str_init_from_array(&str, hello);
    assert(str.len == 13);
    assert(str.size == 14);
    assert(str.data != hello);
    assert(strcmp(str.data, hello) == 0);

    str_del(&str);
}


static void test_init()
{
    struct str_t str = {13, 14, "Hello, World!"};

    str_init(&str);
    assert(str.len == 0);
    assert(str.size == 16);
    assert(str.data != NULL);
    assert(strlen(str.data) == 0);

    str_del(&str);
}


static void test_clear()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");

    str_clear(&str);
    assert(str.len == 0);
    assert(str.size == 14);
    assert(str.data != NULL);
    assert(strlen(str.data) == 0);

    str_del(&str);
}


static void test_del()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");

    str_del(&str);
    assert(str.len == 0);
    assert(str.size == 0);
    assert(str.data == NULL);
}


static void test_append_one_char_to_empty_str()
{
    struct str_t str;
    str_init(&str);

    str_append(&str, 'a');
    assert(str.len == 1);
    assert(str.size == 16);
    assert(strcmp(str.data, "a") == 0);

    str_del(&str);
}


static void test_append_check_correctness()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");
    str_clear(&str);

    /* str is empty, but memory contains garbage */
    str_append(&str, 'a');
    assert(str.len == 1);
    assert(str.size == 14);
    assert(strcmp(str.data, "a") == 0);

    str_del(&str);
}


static void test_append_check_resize()
{
    struct str_t str;
    str_init_from_array(&str, "Hi");

    str_append(&str, '!');
    assert(str.len == 3);
    assert(str.size == 6);
    assert(strcmp(str.data, "Hi!") == 0);

    str_del(&str);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init_from_array,
        test_init,
        test_clear,
        test_del,
        test_append_one_char_to_empty_str,
        test_append_check_correctness,
        test_append_check_resize
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
