#include <stdlib.h>
#include <string.h>

#include "line.h"
#include "message.h"


static void line_fill_by_default(struct line_t *line)
{
    line->is_finished = 0;
    line->is_escaped = 0;
    line->word_is_empty = 0;

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


const char *line_strerror(const struct line_t *line)
{
    switch (line->errno) {
        case error_quotes:
            return msg_line_error_quotes;
        case error_escape:
            return msg_line_error_escape;
        default:
            return NULL;
    }
}


static int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}


static void line_toggle_split_mode(struct line_t *line)
{
    switch (line->mode) {
        case mode_split:
            line->mode = mode_nosplit;
            line->word_is_empty = 0;
            return;
        case mode_nosplit:
            line->mode = mode_split;
            line->word_is_empty = (line->current_word.len == 0);
            return;
    }
}


static void line_set_error(struct line_t *line, enum line_errors error)
{
    line->errno = error;
}


static void line_add_char(struct line_t *line, char c)
{
    str_append(&line->current_word, c);
}


static void line_escape_char(struct line_t *line, char c)
{
    switch (c) {
        case '\\':
        case '"':
            line_add_char(line, c);
            line->is_escaped = 0;
            return;
    }
    line_set_error(line, error_escape);
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
    if (c == '\n') {
        line->is_finished = 1;
    }

    if (line->errno != noerror) {
        return;
    }

    if (line->is_escaped) {
        line_escape_char(line, c);
        return;
    }

    if (c == '"') {
        line_toggle_split_mode(line);
        return;
    }

    if (c == '\\') {
        line->is_escaped = 1;
    } else if (line->mode == mode_nosplit || !is_space(c)) {
        line_add_char(line, c);
    } else if (line->current_word.len || line->word_is_empty) {
        line_add_word(line);
        line_clear_word(line);
    }

    if (line->is_finished && line->mode == mode_nosplit) {
        line_set_error(line, error_quotes);
    }
    line->word_is_empty = 0;
}


void line_from_chars(struct line_t *line, const char *s)
{
    for (; *s; s++) {
        line_process_char(line, *s);
    }

    if (!line->is_finished) {
        line_process_char(line, '\n');
    }
}
