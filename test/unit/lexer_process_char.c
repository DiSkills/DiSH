#include <unity/unity.h>

#include "macros.h"

#include "src/lexer.h"


static struct lexer_t lexer;


static void lexer_set_state(struct lexer_t *lexer, enum lexer_state_t state,
        enum lexer_error_t errno, const char *buffer)
{
    lexer->state = state;
    lexer->errno = errno;

    for (; buffer && *buffer; buffer++) {
        str_append(&lexer->buffer, *buffer);
    }
}


void setUp()
{
    lexer_init(&lexer);
}


void tearDown()
{
    lexer_del(&lexer);
}


/* =============================== initial ================================ */
static void test_initial_ignoring_space()
{
    lexer_process_char(&lexer, ' ');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_ignoring_tab()
{
    lexer_process_char(&lexer, '\t');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_adding_delimiter_less()
{
    lexer_process_char(&lexer, '<');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "<", token_type_delimiter);
}


static void test_initial_adding_delimiter_semicolon()
{
    lexer_process_char(&lexer, ';');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ";", token_type_delimiter);
}


static void test_initial_adding_delimiter_left_parenthesis()
{
    lexer_process_char(&lexer, '(');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "(", token_type_delimiter);
}


static void test_initial_adding_delimiter_right_parenthesis()
{
    lexer_process_char(&lexer, ')');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ")", token_type_delimiter);
}


static void test_initial_to_completed()
{
    lexer_process_char(&lexer, '\n');

    TEST_ASSERT_EQUAL_INT(lexer_state_completed, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_escaping_in_word()
{
    lexer_process_char(&lexer, '\\');

    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_reading_word()
{
    lexer_process_char(&lexer, 'a');

    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_reading_string()
{
    lexer_process_char(&lexer, '"');

    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_pipe()
{
    lexer_process_char(&lexer, '|');

    TEST_ASSERT_EQUAL_INT(lexer_state_pipe, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "|");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_ampersand()
{
    lexer_process_char(&lexer, '&');

    TEST_ASSERT_EQUAL_INT(lexer_state_ampersand, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_initial_to_greater()
{
    lexer_process_char(&lexer, '>');

    TEST_ASSERT_EQUAL_INT(lexer_state_greater, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, ">");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ================================= pipe ================================= */
static void test_pipe_to_double_pipe()
{
    lexer_set_state(&lexer, lexer_state_pipe, lexer_error_noerror, "|");
    lexer_process_char(&lexer, '|');

    TEST_ASSERT_EQUAL_INT(lexer_state_double_pipe, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "||");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_pipe_adding_delimiter()
{
    lexer_set_state(&lexer, lexer_state_pipe, lexer_error_noerror, "|");
    lexer_process_char(&lexer, 'a');

    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "|", token_type_delimiter);
}
/* ======================================================================== */


/* ============================= double pipe ============================== */
static void test_double_pipe_to_error()
{
    lexer_set_state(&lexer, lexer_state_double_pipe,
            lexer_error_noerror, "||");
    lexer_process_char(&lexer, '|');

    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_delimiter, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "||");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_double_pipe_adding_delimiter()
{
    lexer_set_state(&lexer, lexer_state_double_pipe,
            lexer_error_noerror, "||");
    lexer_process_char(&lexer, ' ');

    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "||", token_type_delimiter);
}
/* ======================================================================== */


/* ============================== ampersand =============================== */
static void test_ampersand_to_double_ampersand()
{
    lexer_set_state(&lexer, lexer_state_ampersand, lexer_error_noerror, "&");
    lexer_process_char(&lexer, '&');

    TEST_ASSERT_EQUAL_INT(lexer_state_double_ampersand, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "&&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_ampersand_adding_delimiter()
{
    lexer_set_state(&lexer, lexer_state_ampersand, lexer_error_noerror, "&");
    lexer_process_char(&lexer, '\\');

    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "&", token_type_delimiter);
}
/* ======================================================================== */


/* =========================== double ampersand =========================== */
static void test_double_ampersand_to_error()
{
    lexer_set_state(&lexer, lexer_state_double_ampersand,
            lexer_error_noerror, "&&");
    lexer_process_char(&lexer, '&');

    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_delimiter, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "&&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_double_ampersand_adding_delimiter()
{
    lexer_set_state(&lexer, lexer_state_double_ampersand,
            lexer_error_noerror, "&&");
    lexer_process_char(&lexer, '"');

    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "&&", token_type_delimiter);
}
/* ======================================================================== */


int main()
{
    UNITY_BEGIN();

/* =============================== initial ================================ */
    RUN_TEST(test_initial_ignoring_space);
    RUN_TEST(test_initial_ignoring_tab);

    RUN_TEST(test_initial_adding_delimiter_less);
    RUN_TEST(test_initial_adding_delimiter_semicolon);
    RUN_TEST(test_initial_adding_delimiter_left_parenthesis);
    RUN_TEST(test_initial_adding_delimiter_right_parenthesis);

    RUN_TEST(test_initial_to_completed);
    RUN_TEST(test_initial_to_escaping_in_word);
    RUN_TEST(test_initial_to_reading_word);
    RUN_TEST(test_initial_to_reading_string);

    RUN_TEST(test_initial_to_pipe);
    RUN_TEST(test_initial_to_ampersand);
    RUN_TEST(test_initial_to_greater);
/* ======================================================================== */

/* ================================= pipe ================================= */
    RUN_TEST(test_pipe_to_double_pipe);
    RUN_TEST(test_pipe_adding_delimiter);
/* ======================================================================== */

/* ============================= double pipe ============================== */
    RUN_TEST(test_double_pipe_to_error);
    RUN_TEST(test_double_pipe_adding_delimiter);
/* ======================================================================== */

/* ============================== ampersand =============================== */
    RUN_TEST(test_ampersand_to_double_ampersand);
    RUN_TEST(test_ampersand_adding_delimiter);
/* ======================================================================== */


/* =========================== double ampersand =========================== */
    RUN_TEST(test_double_ampersand_to_error);
    RUN_TEST(test_double_ampersand_adding_delimiter);
/* ======================================================================== */

    return UNITY_END();
}
