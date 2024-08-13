#include <stdlib.h>
#include <string.h>

#include "line.h"


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
