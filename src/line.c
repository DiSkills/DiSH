#include <stdlib.h>
#include <string.h>

#include "line.h"


static int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}


static void line_toggle_split_mode(struct line_t *line)
{
    switch (line->mode) {
        case mode_split:
            line->mode = mode_nosplit;
            return;
        case mode_nosplit:
            line->mode = mode_split;
            return;
    }
}


static void line_set_is_finished(struct line_t *line)
{
    if (line->mode == mode_nosplit) {
        line->errno = error_quotes;
    }
    line->is_finished = 1;
}


static void line_add_char(struct line_t *line, char c)
{
    str_append(&line->current_word, c);
}


static void line_add_word(struct line_t *line)
{
    wordlist_push_back(&line->wordlist, line->current_word.data);
}


static void line_clear_word(struct line_t *line)
{
    str_clear(&line->current_word);
}


void line_process_char(struct line_t *line, char c)
{
    if (c == '"') {
        line_toggle_split_mode(line);
        return;
    }

    if (line->mode == mode_nosplit || !is_space(c)) {
        line_add_char(line, c);
    } else if (line->current_word.len) {
        line_add_word(line);
        line_clear_word(line);
    }

    if (c == '\n') {
        line_set_is_finished(line);
    }
}


static void line_fill_by_default(struct line_t *line)
{
    line->is_finished = 0;
    line->is_escaped = 0;

    line->errno = noerror;
    line->mode = mode_split;
}


void line_init(struct line_t *line)
{
    line_fill_by_default(line);
    str_init(&line->current_word);
    wordlist_init(&line->wordlist);
}


void line_del(struct line_t *line)
{
    line_clear(line);
    str_del(&line->current_word);
}


void line_clear(struct line_t *line)
{
    str_clear(&line->current_word);
    wordlist_del(&line->wordlist);
    line_fill_by_default(line);
}
