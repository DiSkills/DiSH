#include <stdio.h>

#include "cmd.h"
#include "message.h"


#if 0
static void line_print(const struct line_t *line)
{
    const struct wordlist_item *p;
    for (p = line->wordlist.first; p; p = p->next) {
        printf("[%s]\n", p->word);
    }
}
#endif


static void line_exec(struct line_t *line)
{
    struct cmd_t cmd;
    cmd_init_from_line(&cmd, line);

    cmd_exec(&cmd);
    if (cmd.state == state_launched) {
        cmd_wait(&cmd);
    }

    cmd_del(&cmd);
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
            line_exec(&line);
        }
        line_clear(&line);
        printf("%s ", msg_welcome);
    }
    line_del(&line);
    return 0;
}
