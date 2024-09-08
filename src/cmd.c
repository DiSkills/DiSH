#include <stdlib.h>

#include "cmd.h"


static void cmd_fill_by_default(struct cmd_t *cmd)
{
    cmd->name = NULL;

    cmd->argc = 0;
    cmd->argv = NULL;

    cmd->state = state_not_launched;

    cmd->pid = 0;
    cmd->code = 0;
}


void cmd_init(struct cmd_t *cmd)
{
    cmd_fill_by_default(cmd);
}


void cmd_del(struct cmd_t *cmd)
{
    int i;
    for (i = 0; i < cmd->argc; i++) {
        free((char*)cmd->argv[i]);
    }
    free(cmd->argv);

    cmd_fill_by_default(cmd);
}


static void wordlist_callback_move_word(struct wordlist_item *item,
        void *userdata)
{
    const char ***pargv = userdata;

    **pargv = item->word;
    item->word = NULL;
    (*pargv)++;
}


void line_to_cmd(struct line_t *line, struct cmd_t *cmd)
{
    const char **argv;

    cmd->argc = wordlist_get_length(&line->wordlist);
    cmd->argv = malloc((cmd->argc + 1) * sizeof(*cmd->argv));

    /* wordlist to array */
    argv = cmd->argv;
    wordlist_traverse(&line->wordlist, wordlist_callback_move_word, &argv);
    cmd->argv[cmd->argc] = NULL;

    cmd->name = cmd->argv[0];
    cmd->state = state_not_launched;

    line_clear(line);
}
