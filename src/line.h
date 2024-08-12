#ifndef LINE_H
#define LINE_H

#include "str.h"

enum chars_reading_modes {
    mode_non_spaces = 0,
    mode_all = 1
};

enum line_states {
    state_ok = 0,
    state_finished = 1,
    state_unmatched_quotes = -1
};

struct word_item {
    char *word;
    struct word_item *next;
};

struct line_t {
    enum line_states state;
    enum chars_reading_modes mode;

    struct str_t current_word;
    struct word_item *first,
                     *last;
};

void line_init(struct line_t *line);
void line_del(struct line_t *line);
void line_clear(struct line_t *line);
void line_process_char(char c, struct line_t *line);

#endif
