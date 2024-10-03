#ifndef LINE_H
#define LINE_H

#include "str.h"
#include "wordlist.h"

enum line_split_modes {
    mode_split = 0,
    mode_nosplit = 1
};

enum line_errors {
    noerror = 0,
    error_quotes = -1,
    error_escape = -2
};

struct line_t {
    int is_finished,
        is_escaped,
        word_is_empty;

    enum line_errors errno;
    enum line_split_modes mode;

    struct str_t current_word;
    struct wordlist_t wordlist;
};

void line_init(struct line_t *line);
void line_del(struct line_t *line);
void line_clear(struct line_t *line);
const char *line_strerror(const struct line_t *line);
void line_process_char(struct line_t *line, char c);
void line_from_chars(struct line_t *line, const char *s);

#endif
