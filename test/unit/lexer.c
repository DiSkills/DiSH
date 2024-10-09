#include <unity/unity.h>

#include "macros.h"

#include "src/lexer.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init()
{
    struct lexer_t lexer;
    
    lexer_init(&lexer);
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);

    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);

    lexer_del(&lexer);
}


/* TODO fill in the lexer */
static void test_clear()
{
    struct lexer_t lexer;

    lexer_init(&lexer);
    
    lexer_clear(&lexer);
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);

    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);

    lexer_del(&lexer);
}


/* TODO fill in the lexer */
static void test_del()
{
    struct lexer_t lexer;
    
    lexer_init(&lexer);

    lexer_del(&lexer);
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);

    TEST_ASSERT_EQUAL_INT(0, lexer.buffer.len);
    TEST_ASSERT_EQUAL_INT(0, lexer.buffer.size);
    TEST_ASSERT_NULL(lexer.buffer.data);
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_clear);
    RUN_TEST(test_del);

    return UNITY_END();
}
