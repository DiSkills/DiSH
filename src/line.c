#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "line.h"


static void line_fill_by_default(struct line_t *line)
{
    line->is_finished = 0;
    line->is_escaped = 0;
    line->word_is_empty = 0;

    line->errno = line_error_noerror;
    line->mode = line_split_mode_split;
}


void line_init(struct line_t *line)
{
    line_fill_by_default(line);
    str_init(&line->word);
    wordlist_init(&line->wordlist);
}


void line_del(struct line_t *line)
{
    line_clear(line);
    str_del(&line->word);
}


void line_clear(struct line_t *line)
{
    str_clear(&line->word);
    wordlist_del(&line->wordlist);
    line_fill_by_default(line);
}


const char *line_strerror(const struct line_t *line)
{
    switch (line->errno) {
        case line_error_quotes:
            return msg_line_error_quotes;
        case line_error_escape:
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
        case line_split_mode_split:
            line->mode = line_split_mode_nosplit;
            line->word_is_empty = 0;
            return;
        case line_split_mode_nosplit:
            line->mode = line_split_mode_split;
            line->word_is_empty = (line->word.len == 0) ? 1 : 0;
            return;
    }
}


static void line_escape_char(struct line_t *line, char c)
{
    switch (c) {
        case '\\':
        case '"':
            str_append(&line->word, c);
            line->is_escaped = 0;
            return;
    }
    line->errno = line_error_escape;
}


void line_process_char(struct line_t *line, char c)
{
    if (c == '\n') {
        line->is_finished = 1;
    }

    if (line->errno != line_error_noerror) {
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
    } else if (line->mode == line_split_mode_nosplit || !is_space(c)) {
        str_append(&line->word, c);
    } else if (line->word.len || line->word_is_empty) {
        wordlist_push_back(&line->wordlist, line->word.data);
        str_clear(&line->word);
    }

    if (line->is_finished && line->mode == line_split_mode_nosplit) {
        line->errno = line_error_quotes;
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
