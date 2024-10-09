#include <stdio.h>

#include "cmd.h"
#include "error.h"
#include "message.h"


static void line_exec(struct line_t *line)
{
    struct cmd_t cmd;

    if (wordlist_is_empty(&line->wordlist)) {
        return;
    }

    cmd_init_from_line(&cmd, line);

    cmd_exec(&cmd);
    if (cmd.state == cmd_state_launched) {
        cmd_wait(&cmd);
    }

    cmd_del(&cmd);
}


int main()
{
    int c;
    struct line_t line;

    line_init(&line);
    printf("%s ", msg_prompt);
    while ((c = getchar()) != EOF) {
        line_process_char(&line, c);
        if (!line.is_finished) {
            continue;
        }

        if (line.errno) {
            print_error(line_strerror(&line));
        } else {
            line_exec(&line);
        }
        line_clear(&line);
        printf("%s ", msg_prompt);
    }
    line_del(&line);
    return 0;
}
