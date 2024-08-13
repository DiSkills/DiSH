#include <assert.h>
#include <string.h>

#include "../src/line.h"


typedef void (*test_func)();


static void fill_line(struct line_t *line)
{
    line->is_finished = 1;
    line->is_escaped = 1;
    line->errno = error_quotes;
    line->mode = mode_nosplit;
    str_append(&line->current_word, 'a');
    wordlist_push_back(&line->wordlist, "Hello!");
}


static void assert_word_is_default(struct line_t *line)
{
    assert(line->current_word.len == 0);
    assert(line->current_word.size == 16);
    assert(line->current_word.data != NULL);
    assert(strlen(line->current_word.data) == 0);
}


static void assert_wordlist_is_empty(struct line_t *line)
{
    assert(line->wordlist.first == NULL);
    assert(line->wordlist.last == NULL);
}


static void assert_line(struct line_t *line, int is_finished,
        int is_escaped, enum line_errors error, enum line_split_modes mode)
{
    assert(line->is_finished == is_finished);
    assert(line->is_escaped == is_escaped);
    assert(line->errno == error);
    assert(line->mode == mode);
}


static void test_init()
{
    struct line_t line;

    line_init(&line);
    assert_line(&line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(&line);
    assert_word_is_default(&line);

    line_del(&line);
}


static void test_del()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line);
    
    line_del(&line);
    assert_line(&line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(&line);
    assert(line.current_word.size == 0);
    assert(line.current_word.data == NULL);
}


static void test_clear()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line);
    
    line_clear(&line);
    assert_line(&line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(&line);
    assert_word_is_default(&line);

    line_del(&line);
}


static void test_process_char_ignore_spaces()
{
    struct line_t line;
    line_init(&line);

    /* space */
    line_process_char(&line, ' ');
    assert_line(&line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(&line);
    assert_word_is_default(&line);

    /* tab */
    line_process_char(&line, '\t');
    assert_line(&line, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(&line);
    assert_word_is_default(&line);

    line_del(&line);
}


static void test_process_char_add_symbol()
{
    struct line_t line;
    line_init(&line);

    /* split */
    line_process_char(&line, 'a');
    assert_line(&line, 0, 0, noerror, mode_split);

    assert(line.current_word.len == 1);
    assert(strcmp(line.current_word.data, "a") == 0);
    assert_wordlist_is_empty(&line);

    /* nosplit */
    line.mode = mode_nosplit;
    line_process_char(&line, 'b');
    assert_line(&line, 0, 0, noerror, mode_nosplit);

    assert(line.current_word.len == 2);
    assert(strcmp(line.current_word.data, "ab") == 0);
    assert_wordlist_is_empty(&line);

    line_del(&line);
}


static void test_process_char_toggle_split_mode()
{
    struct line_t line;
    line_init(&line);

    /* nosplit */
    line_process_char(&line, '"');
    assert_line(&line, 0, 0, noerror, mode_nosplit);

    assert_word_is_default(&line);
    assert_wordlist_is_empty(&line);

    /* split */
    line_process_char(&line, '"');
    assert_line(&line, 0, 0, noerror, mode_split);

    assert_word_is_default(&line);
    assert_wordlist_is_empty(&line);

    line_del(&line);
}


static void test_process_char_new_line()
{
    struct line_t line;
    line_init(&line);

    line_process_char(&line, '\n');
    assert_line(&line, 1, 0, noerror, mode_split);

    assert_word_is_default(&line);
    assert_wordlist_is_empty(&line);

    line_del(&line);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init,
        test_del,
        test_clear,
        test_process_char_ignore_spaces,
        test_process_char_add_symbol,
        test_process_char_toggle_split_mode,
        test_process_char_new_line
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
