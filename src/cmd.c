#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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


static void exec(struct cmd_t *cmd)
{
    cmd->pid = fork();
    if (cmd->pid == -1) {
        perror("fork");
        return;
    }

    if (cmd->pid == 0) {
        execvp(cmd->name, cmd->argv);
        perror(cmd->name);
        exit(1);
    }
    cmd->state = state_launched;
}


void cmd_exec(struct cmd_t *cmd)
{
    if (!cmd->name) {
        return;
    }

    if (strcmp(cmd->name, "cd") == 0) {
        cd(cmd);
    } else {
        exec(cmd);
    }
}


void cmd_wait(struct cmd_t *cmd)
{
    int wr;

    wr = waitpid(cmd->pid, &cmd->code, 0);
    if (wr == -1) {
        perror("wait");
        return;
    }

    if (WIFEXITED(cmd->code)) {
        cmd->code = WEXITSTATUS(cmd->code);
        cmd->state = state_exited;
    } else {
        cmd->code = WTERMSIG(cmd->code);
        cmd->state = state_terminated;
    }
}
