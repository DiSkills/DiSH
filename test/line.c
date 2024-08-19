#include <assert.h>
#include <string.h>

#include "../src/line.h"

#include "line_assert.h"


typedef void (*test_func)();


static void fill_line(struct line_t *line, int is_finished, int is_escaped,
        int is_empty, enum line_errors error, enum line_split_modes mode)
{
    line->is_finished = is_finished;
    line->is_escaped = is_escaped;
    line->errno = error;
    line->mode = mode;
    str_append(&line->current_word, 'a');
    wordlist_push_back(&line->wordlist, "Hello!");
}


static void test_init()
{
    struct line_t line;

    line_init(&line);
    assert_line(line, 0, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(line);
    assert_word_is_default(line);

    line_del(&line);
}


static void test_del()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line, 1, 1, 1, error_quotes, mode_nosplit);
    
    line_del(&line);
    assert_line(line, 0, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(line);
    assert(line.current_word.size == 0);
    assert(line.current_word.data == NULL);
}


static void test_clear()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line, 1, 1, 1, error_quotes, mode_nosplit);
    
    line_clear(&line);
    assert_line(line, 0, 0, 0, noerror, mode_split);
    assert_wordlist_is_empty(line);
    assert_word_is_default(line);

    line_del(&line);
}


static void test_process_correctly()
{
    int i;
    /* abra sch"wa"bra" "kadabra    foo"    b"ar */
    const char s[] = "abra sch\"wa\"bra\" \"kadabra\tfoo\"\tb\"ar\n";
    struct wordlist_item *item;
    struct line_t line;
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    assert_line(line, 1, 0, 0, noerror, mode_split);
    assert(line.current_word.len == 0);
    assert(line.current_word.size == 32);
    assert(strlen(line.current_word.data) == 0);

    item = line.wordlist.first;
    assert(strcmp(item->word, "abra") == 0);

    item = item->next;
    assert(strcmp(item->word, "schwabra kadabra") == 0);

    item = item->next;
    assert(strcmp(item->word, "foo\tbar") == 0);
    assert(item->next == NULL);

    line_del(&line);
}


static void test_process_escape_sequence_correctly()
{
    int i;
    /* It\"s the \\correct\\ "\"escape\" sequence" */
    const char s[] =
        "It\\\"s the \\\\correct\\\\ \"\\\"escape\\\" sequence\"\n";
    struct wordlist_item *item;
    struct line_t line;
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    assert_line(line, 1, 0, 0, noerror, mode_split);
    assert(line.current_word.len == 0);
    assert(line.current_word.size == 32);
    assert(strlen(line.current_word.data) == 0);

    item = line.wordlist.first;
    assert(strcmp(item->word, "It\"s") == 0);

    item = item->next;
    assert(strcmp(item->word, "the") == 0);

    item = item->next;
    assert(strcmp(item->word, "\\correct\\") == 0);

    item = item->next;
    assert(strcmp(item->word, "\"escape\" sequence") == 0);
    assert(item->next == NULL);

    line_del(&line);
}


static void test_process_error_unmatched_quotes()
{
    int i;
    /* abra "schwabra kadabra"  "foo */
    const char s[] = "abra \"schwabra kadabra\"\t\"foo\n";
    struct wordlist_item *item;
    struct line_t line;
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    assert_line(line, 1, 0, 0, error_quotes, mode_nosplit);
    assert(line.current_word.len == 4);
    assert(line.current_word.size == 32);
    assert(strcmp(line.current_word.data, "foo\n") == 0);

    item = line.wordlist.first;
    assert(strcmp(item->word, "abra") == 0);

    item = item->next;
    assert(strcmp(item->word, "schwabra kadabra") == 0);
    assert(item->next == NULL);

    line_del(&line);
}


static void test_process_error_unsupported_escape_sequence()
{
    int i;
    /* It's \"an\" unsupported\? escape sequence */
    const char s[] = "It's \\\"an\\\" unsupported\\? escape sequence\n";
    struct wordlist_item *item;
    struct line_t line;
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    assert_line(line, 1, 1, 0, error_escape, mode_split);
    assert(line.current_word.len == 11);
    assert(line.current_word.size == 16);
    assert(strcmp(line.current_word.data, "unsupported") == 0);

    item = line.wordlist.first;
    assert(strcmp(item->word, "It's") == 0);

    item = item->next;
    assert(strcmp(item->word, "\"an\"") == 0);
    assert(item->next == NULL);

    line_del(&line);
}


static void test_process_check_to_save_first_error()
{
    int i;
    /* check "to save\? the first error */
    const char s[] = "check \"to save\\? the first error\n";
    struct wordlist_item *item;
    struct line_t line;
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    assert_line(line, 1, 1, 0, error_escape, mode_nosplit);
    assert(line.current_word.len == 7);
    assert(line.current_word.size == 16);
    assert(strcmp(line.current_word.data, "to save") == 0);

    item = line.wordlist.first;
    assert(strcmp(item->word, "check") == 0);
    assert(item->next == NULL);

    line_del(&line);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init, test_del, test_clear,

        test_process_correctly,
        test_process_escape_sequence_correctly,

        test_process_error_unmatched_quotes,
        test_process_error_unsupported_escape_sequence,

        test_process_check_to_save_first_error
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
