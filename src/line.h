#ifndef LINE_H
#define LINE_H

#include "str.h"
#include "wordlist.h"

enum line_split_mode_t {
    line_split_mode_split = 0,
    line_split_mode_nosplit = 1
};

enum line_error_t {
    line_error_noerror = 0,
    line_error_quotes = -1,
    line_error_escape = -2
};

struct line_t {
    int is_finished,
        is_escaped,
        word_is_empty;

    enum line_error_t errno;
    enum line_split_mode_t mode;

    struct str_t word;
    struct wordlist_t wordlist;
};

void line_init(struct line_t *line);
void line_del(struct line_t *line);
void line_clear(struct line_t *line);

const char *line_strerror(const struct line_t *line);
void line_process_char(struct line_t *line, char c);
void line_from_chars(struct line_t *line, const char *s);

#endif
