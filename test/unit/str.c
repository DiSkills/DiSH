#include <unity/unity.h>

#include "src/str.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init_from_array()
{
    const char hello[] = "Hello, World!";
    struct str_t str = { 2, 3, "Hi" };

    str_init_from_array(&str, hello);
    TEST_ASSERT_EQUAL_INT(13, str.len);
    TEST_ASSERT_EQUAL_INT(14, str.size);
    TEST_ASSERT_FALSE(str.data == hello);
    TEST_ASSERT_EQUAL_STRING(hello, str.data);

    str_del(&str);
}


static void test_init()
{
    struct str_t str = { 13, 14, "Hello, World!" };

    str_init(&str);
    TEST_ASSERT_EQUAL_INT(0, str.len);
    TEST_ASSERT_EQUAL_INT(16, str.size);
    TEST_ASSERT_NOT_NULL(str.data);
    TEST_ASSERT_EMPTY(str.data);

    str_del(&str);
}


static void test_clear()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");

    str_clear(&str);
    TEST_ASSERT_EQUAL_INT(0, str.len);
    TEST_ASSERT_EQUAL_INT(14, str.size);
    TEST_ASSERT_NOT_NULL(str.data);
    TEST_ASSERT_EMPTY(str.data);

    str_del(&str);
}


static void test_del()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");

    str_del(&str);
    TEST_ASSERT_EQUAL_INT(0, str.len);
    TEST_ASSERT_EQUAL_INT(0, str.size);
    TEST_ASSERT_NULL(str.data);
}


static void test_append_one_char_to_empty_str()
{
    struct str_t str;
    str_init(&str);

    str_append(&str, 'a');
    TEST_ASSERT_EQUAL_INT(1, str.len);
    TEST_ASSERT_EQUAL_INT(16, str.size);
    TEST_ASSERT_EQUAL_STRING("a", str.data);

    str_del(&str);
}


static void test_append_check_correctness()
{
    struct str_t str;
    str_init_from_array(&str, "Hello, World!");
    str_clear(&str);

    /* str is empty, but memory contains garbage */
    str_append(&str, 'a');
    TEST_ASSERT_EQUAL_INT(1, str.len);
    TEST_ASSERT_EQUAL_INT(14, str.size);
    TEST_ASSERT_EQUAL_STRING("a", str.data);

    str_del(&str);
}


static void test_append_check_resize()
{
    struct str_t str;
    str_init_from_array(&str, "Hi");

    str_append(&str, '!');
    TEST_ASSERT_EQUAL_INT(3, str.len);
    TEST_ASSERT_EQUAL_INT(6, str.size);
    TEST_ASSERT_EQUAL_STRING("Hi!", str.data);

    str_del(&str);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init_from_array);
    RUN_TEST(test_init);
    RUN_TEST(test_clear);
    RUN_TEST(test_del);
    RUN_TEST(test_append_one_char_to_empty_str);
    RUN_TEST(test_append_check_correctness);
    RUN_TEST(test_append_check_resize);

    return UNITY_END();
}
