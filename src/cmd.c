#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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


void cmd_del(struct cmd_t *cmd)
{
    for (; cmd->argc; cmd->argc--) {
        free(cmd->argv[cmd->argc - 1]);
    }
    free(cmd->argv);

    cmd->argv = NULL;
    cmd->name = NULL;
    cmd->state = state_not_launched;
}


static void cd(struct cmd_t *cmd)
{
    int cdr;

    cmd->pid = 0;
    cmd->code = 1;
    cmd->state = state_exited;

    if (cmd->argc != 2) {
        fprintf(stderr, "cd: too many arguments\n");
        return;
    }

    cdr = chdir(cmd->argv[1]);
    if (cdr == -1) {
        perror(cmd->argv[1]);
        return;
    }
    cmd->code = 0;
}
