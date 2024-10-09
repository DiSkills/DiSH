#include <unity/unity.h>

#include "macros.h"

#include "src/lexer.h"


static struct lexer_t lexer;


void setUp()
{
    lexer_init(&lexer);
}


void tearDown()
{
    lexer_del(&lexer);
}


/* ================================= |,|| ================================= */
static void test_to_pipe()
{
    lexer_process_char(&lexer, '|');
    TEST_ASSERT_EQUAL_INT(lexer_state_pipe, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "|");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_pipe()
{
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, 'a');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "|", token_type_delimiter);
}


static void test_to_double_pipe()
{
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, '|');
    TEST_ASSERT_EQUAL_INT(lexer_state_double_pipe, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "||");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_double_pipe()
{
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, ' ');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "||", token_type_delimiter);
}


static void test_from_double_pipe_to_error()
{
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, '|');
    lexer_process_char(&lexer, '|');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_delimiter, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "||");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ================================= &,&& ================================= */
static void test_to_ampersand()
{
    lexer_process_char(&lexer, '&');
    TEST_ASSERT_EQUAL_INT(lexer_state_ampersand, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_ampersand()
{
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "&", token_type_delimiter);
}


static void test_to_double_ampersand()
{
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '&');
    TEST_ASSERT_EQUAL_INT(lexer_state_double_ampersand, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "&&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_double_ampersand()
{
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "&&", token_type_delimiter);
}


static void test_from_double_ampersand_to_error()
{
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '&');
    lexer_process_char(&lexer, '&');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_delimiter, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "&&");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ================================= >,>> ================================= */
static void test_to_greater()
{
    lexer_process_char(&lexer, '>');
    TEST_ASSERT_EQUAL_INT(lexer_state_greater, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, ">");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_greater()
{
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, '|');
    TEST_ASSERT_EQUAL_INT(lexer_state_pipe, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "|");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ">", token_type_delimiter);
}


static void test_to_double_greater()
{
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, '>');
    TEST_ASSERT_EQUAL_INT(lexer_state_double_greater, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, ">>");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_double_greater()
{
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, 'a');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ">>", token_type_delimiter);
}


static void test_from_double_greater_to_error()
{
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, '>');
    lexer_process_char(&lexer, '>');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_delimiter, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, ">>");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ================================ ' ',\t ================================ */
static void test_to_initial_ignoring_space()
{
    lexer_process_char(&lexer, ' ');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_to_initial_ignoring_tab()
{
    lexer_process_char(&lexer, '\t');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ================================= <;() ================================= */
static void test_to_initial_adding_less()
{
    lexer_process_char(&lexer, '<');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "<", token_type_delimiter);
}


static void test_to_initial_adding_semicolon()
{
    lexer_process_char(&lexer, ';');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ";", token_type_delimiter);
}


static void test_to_initial_adding_left_parenthesis()
{
    lexer_process_char(&lexer, '(');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "(", token_type_delimiter);
}


static void test_to_initial_adding_right_parenthesis()
{
    lexer_process_char(&lexer, ')');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, ")", token_type_delimiter);
}
/* ======================================================================== */


/* =========================== escaping in word =========================== */
static void test_to_escaping_in_word()
{
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_escaping_in_word_to_error()
{
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, 'a');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_escape, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_escaping_in_word_to_reading_word_by_backslash()
{
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "\\");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_escaping_in_word_to_reading_word_by_quote()
{
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "\"");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ============================= reading word ============================= */
static void test_to_reading_word()
{
    lexer_process_char(&lexer, 'a');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_word_to_reading_word()
{
    lexer_process_char(&lexer, 'a');
    lexer_process_char(&lexer, 'b');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, "ab");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_word_to_escaping_in_word()
{
    lexer_process_char(&lexer, 'a');
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_word_to_reading_string()
{
    lexer_process_char(&lexer, 'a');
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "a");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_word_to_initial()
{
    lexer_process_char(&lexer, 'a');
    lexer_process_char(&lexer, ' ');
    TEST_ASSERT_EQUAL_INT(lexer_state_initial, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");

    TEST_ASSERT_LEXER_TOKENS_TAIL(lexer, "a", token_type_word);
}
/* ======================================================================== */


/* ============================ reading string ============================ */
static void test_to_reading_string()
{
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_string_to_reading_string()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, ' ');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, " ");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);

    lexer_process_char(&lexer, '>');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 2, str_min_size, " >");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_string_to_reading_word()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_word, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_string_to_error()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '\n');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_quotes, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_reading_string_to_escaping_in_string()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_escaping_in_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


/* ========================== escaping in string ========================== */
static void test_from_escaping_in_string_to_error()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, 'a');
    TEST_ASSERT_EQUAL_INT(lexer_state_error, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_escape, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_escaping_in_string_to_reading_string_by_backslash()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, '\\');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "\\");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


static void test_from_escaping_in_string_to_reading_string_by_quote()
{
    lexer_process_char(&lexer, '"');
    lexer_process_char(&lexer, '\\');
    lexer_process_char(&lexer, '"');
    TEST_ASSERT_EQUAL_INT(lexer_state_reading_string, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 1, str_min_size, "\"");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}
/* ======================================================================== */


static void test_to_completed()
{
    lexer_process_char(&lexer, '\n');
    TEST_ASSERT_EQUAL_INT(lexer_state_completed, lexer.state);
    TEST_ASSERT_EQUAL_INT(lexer_error_noerror, lexer.errno);
    TEST_ASSERT_LEXER_BUFFER(lexer, 0, str_min_size, "");
    TEST_ASSERT_LEXER_TOKENS_IS_EMPTY(lexer);
}


int main()
{
    UNITY_BEGIN();

/* ================================= |,|| ================================= */
    RUN_TEST(test_to_pipe);
    RUN_TEST(test_from_pipe);
    RUN_TEST(test_to_double_pipe);
    RUN_TEST(test_from_double_pipe);
    RUN_TEST(test_from_double_pipe_to_error);
/* ======================================================================== */

/* ================================= &,&& ================================= */
    RUN_TEST(test_to_ampersand);
    RUN_TEST(test_from_ampersand);
    RUN_TEST(test_to_double_ampersand);
    RUN_TEST(test_from_double_ampersand);
    RUN_TEST(test_from_double_ampersand_to_error);
/* ======================================================================== */

/* ================================= >,>> ================================= */
    RUN_TEST(test_to_greater);
    RUN_TEST(test_from_greater);
    RUN_TEST(test_to_double_greater);
    RUN_TEST(test_from_double_greater);
    RUN_TEST(test_from_double_greater_to_error);
/* ======================================================================== */

/* ================================ ' ',\t ================================ */
    RUN_TEST(test_to_initial_ignoring_space);
    RUN_TEST(test_to_initial_ignoring_tab);
/* ======================================================================== */

/* ================================= <;() ================================= */
    RUN_TEST(test_to_initial_adding_less);
    RUN_TEST(test_to_initial_adding_semicolon);
    RUN_TEST(test_to_initial_adding_left_parenthesis);
    RUN_TEST(test_to_initial_adding_right_parenthesis);
/* ======================================================================== */

/* =========================== escaping in word =========================== */
    RUN_TEST(test_to_escaping_in_word);
    RUN_TEST(test_from_escaping_in_word_to_error);
    RUN_TEST(test_from_escaping_in_word_to_reading_word_by_backslash);
    RUN_TEST(test_from_escaping_in_word_to_reading_word_by_quote);
/* ======================================================================== */

/* ============================= reading word ============================= */
    RUN_TEST(test_to_reading_word);
    RUN_TEST(test_from_reading_word_to_reading_word);
    RUN_TEST(test_from_reading_word_to_escaping_in_word);
    RUN_TEST(test_from_reading_word_to_reading_string);
    RUN_TEST(test_from_reading_word_to_initial);
/* ======================================================================== */

/* ============================ reading string ============================ */
    RUN_TEST(test_to_reading_string);
    RUN_TEST(test_from_reading_string_to_reading_string);
    RUN_TEST(test_from_reading_string_to_reading_word);
    RUN_TEST(test_from_reading_string_to_error);
    RUN_TEST(test_from_reading_string_to_escaping_in_string);
/* ======================================================================== */

/* ========================== escaping in string ========================== */
    RUN_TEST(test_from_escaping_in_string_to_error);
    RUN_TEST(test_from_escaping_in_string_to_reading_string_by_backslash);
    RUN_TEST(test_from_escaping_in_string_to_reading_string_by_quote);
/* ======================================================================== */

    RUN_TEST(test_to_completed);

    return UNITY_END();
}
