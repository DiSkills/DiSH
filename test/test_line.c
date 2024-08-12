#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "../src/line.h"


#define RUN_TEST(TEST) \
    { \
        struct line_t line; \
        line_init(&line); \
        TEST(&line); \
        line_del(&line); \
    }

typedef void (*func)(struct line_t *);


static void test_switch_mode(struct line_t *line)
{
    line_process_char('"', line);
    assert(line->mode == mode_all);

    line_process_char('"', line);
    assert(line->mode == mode_non_spaces);
}


static void test_add_char(struct line_t *line)
{
    line_process_char('c', line);
    assert(line->current_word.len == 1);
    assert(strcmp(line->current_word.data, "c") == 0);
}


static void test_ignore_spaces(struct line_t *line)
{
    line_process_char(' ', line);
    assert(line->current_word.len == 0);

    line_process_char('\t', line);
    assert(line->current_word.len == 0);
}


static void test_add_spaces(struct line_t *line)
{
    line->mode = mode_all;
    line_process_char('a', line);
    assert(strcmp(line->current_word.data, "a") == 0);

    line_process_char(' ', line);
    assert(strcmp(line->current_word.data, "a ") == 0);

    line_process_char('\t', line);
    assert(strcmp(line->current_word.data, "a \t") == 0);

    line_process_char('a', line);
    assert(strcmp(line->current_word.data, "a \ta") == 0);
}


static void test_empty_word_add_endl(struct line_t *line)
{
    line_process_char('\n', line);
    assert(line->first == NULL && line->last == NULL);
    assert(line->state == state_finished);
    assert(line->current_word.len == 0);
}


static void test_unmatched_quotes(struct line_t *line)
{
    line->mode = mode_all;
    line_process_char('\n', line);
    assert(line->first == NULL && line->last == NULL);
    assert(line->state == state_unmatched_quotes);
    assert(line->current_word.len == 1);
}


static void test_add_new_words(struct line_t *line)
{
    char *p,
         hello[] = "Hello,",
         world[] = "World!",
         myname[] = "My\" name is \"DiSkills";

    for (p = hello; *p; p++) {
        line_process_char(*p, line);
    }
    line_process_char(' ', line);
    assert(line->current_word.len == 0);
    assert(line->first == line->last && line->first != NULL);
    assert(strcmp(line->last->word, hello) == 0);

    for (p = world; *p; p++) {
        line_process_char(*p, line);
    }
    line_process_char('\t', line);
    assert(line->current_word.len == 0);
    assert(line->first != line->last);
    assert(strcmp(line->last->word, world) == 0);

    for (p = myname; *p; p++) {
        line_process_char(*p, line);
    }
    line_process_char('\n', line);
    assert(line->current_word.len == 0);
    assert(line->first != line->last);
    assert(strcmp(line->last->word, "My name is DiSkills") == 0);
    assert(line->state == state_finished);
}


int main()
{
    int i;
    func tests[] = {
        test_switch_mode, test_add_char,
        test_ignore_spaces, test_add_spaces,
        test_empty_word_add_endl, test_unmatched_quotes,
        test_add_new_words
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        RUN_TEST(tests[i]);
    }
    return 0;
}
