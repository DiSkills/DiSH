#include <stdio.h>

#include "line.h"
#include "message.h"


static void line_print(const struct line_t *line)
{
    const struct wordlist_item *p;
    for (p = line->wordlist.first; p; p = p->next) {
        printf("[%s]\n", p->word);
    }
}


static void line_print_error(const struct line_t *line)
{
    fprintf(stderr, "%s: %s\n", msg_error, line_get_error_msg(line));
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
