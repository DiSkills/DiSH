#include <unity/unity.h>

#include "line_assert.h"

#include "src/line.h"


static struct line_t line;


void setUp()
{
    line_init(&line);
}


void tearDown()
{
    line_del(&line);
}


static void test_toggle_split_mode()
{
    /* split */
    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 1, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_set_is_escaped()
{
    /* split */
    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_mark_end_of_line()
{
    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_ignore_spaces()
{
    /* space */
    line_process_char(&line, ' ');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* tab */
    line_process_char(&line, '\t');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_add_spaces()
{
    line.mode = line_split_mode_nosplit;

    /* space */
    line_process_char(&line, ' ');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 1, 16, " ");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* tab */
    line_process_char(&line, '\t');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, " \t");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_add_symbol()
{
    /* split */
    line_process_char(&line, 'a');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, 'b');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "ab");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_add_quotes()
{
    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\"");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.is_escaped = 1;
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "\"\"");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_add_backslash()
{
    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\\");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line.is_escaped = 1;
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 2, 16, "\\\\");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_empty_quotes_beginning_word()
{
    /* char */
    line.word_is_empty = 1;

    line_process_char(&line, 'a');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* \ */
    line_clear(&line);
    line.word_is_empty = 1;

    line_process_char(&line, '\\');
    TEST_ASSERT_LINE(line, 0, 1, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* " */
    line_clear(&line);
    line.word_is_empty = 1;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_empty_quotes_inside_word()
{
    str_append(&line.word, 'a');
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, '"');
    TEST_ASSERT_LINE(line, 0, 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "a");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_add_word_using_separator(char separator,
        const char *expected, const char *word)
{
    int i;
    const char *p = line.word.data;

    for (i = 0; word[i]; i++) {
        line_process_char(&line, word[i]);
    }
    line_process_char(&line, separator);

    TEST_ASSERT_LINE(line, separator == '\n', 0, 0, line_error_noerror,
            line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_EQUAL_PTR(p, line.word.data);
    TEST_ASSERT_EQUAL_STRING(expected, line.wordlist.last->word);
}


static void test_add_word()
{
    int i;
    const char word[] = "word",
          separators[] = { ' ', '\t', '\n' };

    for (i = 0; i < sizeof(separators); i++) {
        test_add_word_using_separator(separators[i], word, word);
        line_clear(&line);
    }
}


static void test_add_empty_word()
{
    int i;
    const char word[] = "\"\"",
          expected[] = "",
          separators[] = { ' ', '\t', '\n' };

    for (i = 0; i < sizeof(separators); i++) {
        test_add_word_using_separator(separators[i], expected, word);
        line_clear(&line);
    }
}


static void test_line_error_quotes()
{
    line.mode = line_split_mode_nosplit;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_quotes,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 1, 16, "\n");
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_line_error_escape()
{
    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '?');
    TEST_ASSERT_LINE(line, 0, 1, 0, line_error_escape, line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = line_split_mode_nosplit;
    line.is_escaped = 1;

    line_process_char(&line, '?');
    TEST_ASSERT_LINE(line, 0, 1, 0, line_error_escape,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
}


static void test_line_error_escape_new_line()
{
    /* split */
    line.is_escaped = 1;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 1, 0, line_error_escape, line_split_mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    /* nosplit */
    line_clear(&line);
    line.mode = line_split_mode_nosplit;
    line.is_escaped = 1;

    line_process_char(&line, '\n');
    TEST_ASSERT_LINE(line, 1, 1, 0, line_error_escape,
            line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);
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

    RUN_TEST(test_line_error_quotes);
    RUN_TEST(test_line_error_escape);
    RUN_TEST(test_line_error_escape_new_line);

    return UNITY_END();
}
