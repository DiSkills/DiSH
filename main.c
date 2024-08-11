#include <stdio.h>

#include "line.h"


static const char welcome_msg[] = "> ";

static const char error_msg[] = "Error:";
static const char error_msg_quotes[] = "unmatched quotes";


static void line_print(struct line_t *line)
{
    struct word_item *p;
    for (p = line->first; p; p = p->next) {
        printf("[%s]\n", p->word);
    }
}


static void line_print_error(struct line_t *line)
{
    const char *error;
    if (line->state == state_unmatched_quotes) {
        error = error_msg_quotes;
    }
    fprintf(stderr, "%s %s\n", error_msg, error);
}


int main()
{
    int c;
    struct line_t line;

    line_init(&line);
    printf("%s", welcome_msg);
    while ((c = getchar()) != EOF) {
        line_process_char(c, &line);
        if (line.state == state_ok) {
            continue;
        }
        if (line.state == state_finished) {
            line_print(&line);
        } else {
            line_print_error(&line);
        }

        line_clear(&line);
        printf("%s", welcome_msg);
    }
    line_del(&line);
    return 0;
}
