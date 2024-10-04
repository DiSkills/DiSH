#include <unity/unity.h>

#include "line_assert.h"

#include "src/line.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init()
{
    struct line_t line;

    line_init(&line);
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_del()
{
    struct line_t line;
    line_init(&line);
    FILL_LINE(line, 1, 1, 1, line_error_quotes, line_split_mode_nosplit);
    
    line_del(&line);
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_EQUAL_INT(0, line.word.size);
    TEST_ASSERT_NULL(line.word.data);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_clear()
{
    struct line_t line;
    line_init(&line);
    FILL_LINE(line, 1, 1, 1, line_error_quotes, line_split_mode_nosplit);
    
    line_clear(&line);
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_del);
    RUN_TEST(test_clear);

    return UNITY_END();
}
