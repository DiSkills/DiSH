#include <stdlib.h>

#include "cmd.h"


static void callback_move_to_argv(struct wordlist_item *p, void *userdata)
{
    char ***pargv = userdata;

    **pargv = p->word;
    p->word = NULL;
    (*pargv)++;
}


void cmd_init_from_line(struct cmd_t *cmd, struct line_t *line)
{
    char **argv;

    cmd->argc = wordlist_length(&line->wordlist);
    cmd->argv = malloc((cmd->argc + 1) * sizeof(*cmd->argv));

    /* fill in argv */
    argv = cmd->argv;
    wordlist_traverse(&line->wordlist, callback_move_to_argv, &argv);
    cmd->argv[cmd->argc] = NULL;

    /* fill in additional fields */
    cmd->name = cmd->argv[0];
    cmd->state = state_not_launched;

    line_clear(line);
}
