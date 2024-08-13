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


static void assert_line_is_default(struct line_t *line)
{
    assert(line->is_finished == 0);
    assert(line->is_escaped == 0);
    assert(line->errno == noerror);
    assert(line->mode == mode_split);
   
    assert(line->wordlist.first == NULL);
    assert(line->wordlist.last == NULL);
    
    assert(line->current_word.len == 0);
}


static void test_init()
{
    struct line_t line;

    line_init(&line);
    assert_line_is_default(&line);
    assert(line.current_word.size == 16);
    assert(line.current_word.data != NULL);
    assert(strlen(line.current_word.data) == 0);

    line_del(&line);
}


static void test_del()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line);
    
    line_del(&line);
    assert_line_is_default(&line);
    assert(line.current_word.size == 0);
    assert(line.current_word.data == NULL);
}


static void test_clear()
{
    struct line_t line;
    line_init(&line);
    fill_line(&line);
    
    line_clear(&line);
    assert_line_is_default(&line);
    assert(line.current_word.size == 16);
    assert(line.current_word.data != NULL);
    assert(strlen(line.current_word.data) == 0);

    line_del(&line);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init,
        test_del,
        test_clear
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
