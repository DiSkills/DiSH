#include <stdio.h>

#include "line.h"


static const char msg_welcome[] = ">";

static const char msg_error[] = "Error:";
static const char msg_error_quotes[] = "unmatched quotes";
static const char msg_error_escape[] = "unsupported escape sequence";


static void line_print(struct line_t *line)
{
    struct wordlist_item *p;
    for (p = line->wordlist.first; p; p = p->next) {
        printf("[%s]\n", p->word);
    }
}


static void line_print_error(struct line_t *line)
{
    const char *error;
    switch (line->errno) {
        case error_quotes:
            error = msg_error_quotes;
            break;
        case error_escape:
            error = msg_error_escape;
            break;
        case noerror:
            return;
    }
    fprintf(stderr, "%s %s\n", msg_error, error);
}


int main()
{
    int c;
    struct line_t line;

    line_init(&line);
    printf("%s ", msg_welcome);
    while ((c = getchar()) != EOF) {
        line_process_char(&line, c);

        if (!line.is_finished) {
            continue;
        }

        if (line.errno != noerror) {
            line_print_error(&line);
        } else {
            line_print(&line);
        }
        line_clear(&line);
        printf("%s ", msg_welcome);
    }
    line_del(&line);
    return 0;
}
