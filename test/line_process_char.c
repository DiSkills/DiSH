#include <assert.h>
#include <string.h>

#include "../src/line.h"

#include "line_assert.h"


typedef void (*test_func)();


static void test_toggle_split_mode()
{
    struct line_t line;
    line_init(&line);

    /* nosplit */
    line_process_char(&line, '"');
    assert_line(line, 0, 0, noerror, mode_nosplit);
    assert_word_is_default(line);
    assert_wordlist_is_empty(line);

    /* split */
    line_process_char(&line, '"');
    assert_line(line, 0, 0, noerror, mode_split);
    assert_word_is_default(line);
    assert_wordlist_is_empty(line);

    line_del(&line);
}


static void test_ignore_spaces()
{
    struct line_t line;
    line_init(&line);

    /* space */
    line_process_char(&line, ' ');
    assert_line(line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(line);
    assert_word_is_default(line);

    /* tab */
    line_process_char(&line, '\t');
    assert_line(line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(line);
    assert_word_is_default(line);

    line_del(&line);
}


static void test_add_spaces()
{
    struct line_t line;
    line_init(&line);
    line.mode = mode_nosplit;

    /* space */
    line_process_char(&line, ' ');
    assert_line(line, 0, 0, noerror, mode_nosplit);
    assert(line.current_word.len == 1);
    assert(strcmp(line.current_word.data, " ") == 0);
    assert_wordlist_is_empty(line);

    /* tab */
    line_process_char(&line, '\t');
    assert_line(line, 0, 0, noerror, mode_nosplit);
    assert(line.current_word.len == 2);
    assert(strcmp(line.current_word.data, " \t") == 0);
    assert_wordlist_is_empty(line);

    line_del(&line);
}


static void test_add_symbol()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line_process_char(&line, 'a');
    assert_line(line, 0, 0, noerror, mode_split);

    assert(line.current_word.len == 1);
    assert(strcmp(line.current_word.data, "a") == 0);
    assert_wordlist_is_empty(line);

    /* nosplit */
    line.mode = mode_nosplit;
    line_process_char(&line, 'b');
    assert_line(line, 0, 0, noerror, mode_nosplit);

    assert(line.current_word.len == 2);
    assert(strcmp(line.current_word.data, "ab") == 0);
    assert_wordlist_is_empty(line);

    line_del(&line);
}


static void test_mark_end_of_line()
{
    struct line_t line;
    line_init(&line);

    line_process_char(&line, '\n');
    assert_line(line, 1, 0, noerror, mode_split);
    assert_word_is_default(line);
    assert_wordlist_is_empty(line);

    line_del(&line);
}


static void test_error_unmatched_quotes()
{
    struct line_t line;
    line_init(&line);
    line.mode = mode_nosplit;

    line_process_char(&line, '\n');
    assert_line(line, 1, 0, error_quotes, mode_nosplit);
    assert(line.current_word.len == 1);
    assert(strcmp(line.current_word.data, "\n") == 0);
    assert_wordlist_is_empty(line);

    line_del(&line);
}


static void test_add_word_using_separator(char separator)
{
    int i;
    const char word[] = "word",
          *p;
    struct line_t line;
    line_init(&line);

    p = line.current_word.data;
    for (i = 0; word[i]; i++) {
        line_process_char(&line, word[i]);
    }

    line_process_char(&line, separator);
    assert_line(line, separator == '\n', 0, noerror, mode_split);
    assert_word_is_default(line);
    assert(p == line.current_word.data);
    assert(strcmp(line.wordlist.last->word, word) == 0);

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


int main()
{
    int i;
    test_func tests[] = {
        test_toggle_split_mode,
        test_mark_end_of_line,

        test_ignore_spaces,

        test_add_spaces,
        test_add_symbol,

        test_add_word,

        test_error_unmatched_quotes
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
