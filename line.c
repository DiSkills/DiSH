#include <stdlib.h>
#include <string.h>

#include "line.h"


static int is_space(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}


static void line_switch_mode(struct line_t *line)
{
    switch (line->mode) {
        case mode_non_spaces:
            line->mode = mode_all;
            return;
        case mode_all:
            line->mode = mode_non_spaces;
            return;
    }
}


static void line_set_last_state(struct line_t *line)
{
    if (line->mode == mode_all) {
        line->state = state_unmatched_quotes;
    } else {
        line->state = state_finished;
    }
}


static void line_add_char(char c, struct line_t *line)
{
    str_append(c, &line->current_word);
}


static void line_add_word(struct line_t *line)
{
    struct word_item *tmp = malloc(sizeof(struct word_item));
    tmp->word = malloc(line->current_word.size + 1);
    strcpy(tmp->word, line->current_word.data);
    tmp->next = NULL;

    if (line->last) {
        line->last->next = tmp;
    } else {
        line->first = tmp;
    }
    line->last = tmp;
}


static void line_clear_word(struct line_t *line)
{
    str_clear(&line->current_word);
}


void line_process_char(char c, struct line_t *line)
{
    if (c == '"') {
        line_switch_mode(line);
        return;
    }

    if (line->mode == mode_all || !is_space(c)) {
        line_add_char(c, line);
    } else if (line->current_word.size) {
        line_add_word(line);
        line_clear_word(line);
    }
    if (c == '\n') {
        line_set_last_state(line);
    }
}


static void line_set_default(struct line_t *line)
{
    line->state = state_ok;
    line->mode = mode_non_spaces;

    line->first = NULL;
    line->last = NULL;
}


void line_init(struct line_t *line)
{
    line_set_default(line);
    str_init(&line->current_word);
}


void line_clear(struct line_t *line)
{
    line_clear_word(line);

    while (line->first) {
        struct word_item *tmp = line->first;
        line->first = tmp->next;
        free(tmp->word);
        free(tmp);
    }
    line_set_default(line);
}


void line_del(struct line_t *line)
{
    line_clear(line);
    str_del(&line->current_word);
}
