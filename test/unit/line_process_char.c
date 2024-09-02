#include <unity.h>

#include "line_assert.h"

#include "src/line.h"


void setUp()
{
}


void tearDown()
{
}


static void test_toggle_split_mode()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 1, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_set_is_escaped()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = mode_nosplit;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_mark_end_of_line()
{
    struct line_t line;
    line_init(&line);

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_ignore_spaces()
{
    struct line_t line;
    line_init(&line);

    /* space */
    line_process_char(&line, ' ');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* tab */
    line_process_char(&line, '\t');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_add_spaces()
{
    struct line_t line;
    line_init(&line);
    line.mode = mode_nosplit;

    /* space */
    line_process_char(&line, ' ');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 1, 16, " ");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* tab */
    line_process_char(&line, '\t');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, " \t");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_add_symbol()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line_process_char(&line, 'a');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.mode = mode_nosplit;

    line_process_char(&line, 'b');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "ab");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_add_quotes()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\"");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.is_escaped = 1;
    line.mode = mode_nosplit;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "\"\"");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_add_backslash()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\\");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.is_escaped = 1;
    line.mode = mode_nosplit;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "\\\\");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_empty_quotes_beginning_word()
{
    struct line_t line;
    line_init(&line);

    /* char */
    line.word_is_empty = 1;

    line_process_char(&line, 'a');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* \ */
    line_clear(&line);
    line.word_is_empty = 1;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* " */
    line_clear(&line);
    line.word_is_empty = 1;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_empty_quotes_inside_word()
{
    struct line_t line;
    line_init(&line);
    str_append(&line.current_word, 'a');
    line.mode = mode_nosplit;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_add_word_using_separator(char separator)
{
    int i;
    const char *p;
    struct line_t line;
    const char word[] = "word";
    line_init(&line);

    p = line.current_word.data;
    for (i = 0; word[i]; i++) {
        line_process_char(&line, word[i]);
    }

    line_process_char(&line, separator);
    TEST_ASSERT_LINE(line, separator == '\n', 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_EQUAL_PTR(p, line.current_word.data);
    TEST_ASSERT_EQUAL_STRING(word, line.wordlist.last->word);

    line_del(&line);
}


static void test_add_word()
{
    int i;
    const char separators[] = { ' ', '\t', '\n' };
    for (i = 0; i < sizeof(separators); i++) {
        test_add_word_using_separator(separators[i]);
    }
}


static void test_add_empty_word_using_separator(char separator)
{
    int i;
    const char *p;
    const char word[] = "\"\"";
    struct line_t line;
    line_init(&line);

    p = line.current_word.data;
    for (i = 0; word[i]; i++) {
        line_process_char(&line, word[i]);
    }

    line_process_char(&line, separator);
    TEST_ASSERT_LINE(line, separator == '\n', 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_EQUAL_PTR(p, line.current_word.data);
    TEST_ASSERT_EMPTY(line.wordlist.last->word);

    line_del(&line);
}


static void test_add_empty_word()
{
    int i;
    const char separators[] = { ' ', '\t', '\n' };
    for (i = 0; i < sizeof(separators); i++) {
        test_add_empty_word_using_separator(separators[i]);
    }
}


static void test_error_quotes()
{
    struct line_t line;
    line_init(&line);
    line.mode = mode_nosplit;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 0, 0, error_quotes, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\n");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_error_escape()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '?');
    TEST_ASSERT_LINE(line, 0, 1, 0, error_escape, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = mode_nosplit;
    line.is_escaped = 1;

    line_process_char(&line, '?');
    TEST_ASSERT_LINE(line, 0, 1, 0, error_escape, mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


static void test_error_escape_new_line()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 1, 0, error_escape, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = mode_nosplit;
    line.is_escaped = 1;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 1, 0, error_escape, mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_toggle_split_mode);
    RUN_TEST(test_set_is_escaped);
    RUN_TEST(test_mark_end_of_line);

    RUN_TEST(test_ignore_spaces);

    RUN_TEST(test_add_spaces);
    RUN_TEST(test_add_symbol);
    RUN_TEST(test_add_quotes);
    RUN_TEST(test_add_backslash);

    RUN_TEST(test_empty_quotes_beginning_word);
    RUN_TEST(test_empty_quotes_inside_word);

    RUN_TEST(test_add_word);
    RUN_TEST(test_add_empty_word);

    RUN_TEST(test_error_quotes);
    RUN_TEST(test_error_escape);
    RUN_TEST(test_error_escape_new_line);

    return UNITY_END();
}
