#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "cmd.h"
#include "error.h"
#include "message.h"


static void wordlist_to_argv(char **argv, struct wordlist_t *wordlist)
{
    while (wordlist->first) {
        struct wordlist_item_t *tmp;

        tmp = wordlist->first;
        wordlist->first = tmp->next;

        *argv = tmp->word;
        argv++;

        free(tmp);
    }
    wordlist->last = NULL;
    *argv = NULL;
}


void cmd_init_from_line(struct cmd_t *cmd, struct line_t *line)
{
    cmd->argc = wordlist_length(&line->wordlist);
    cmd->argv = malloc((cmd->argc + 1) * sizeof(*cmd->argv));
    wordlist_to_argv(cmd->argv, &line->wordlist);

    cmd->name = cmd->argv[0];
    cmd->state = cmd_state_not_launched;

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
    cmd->state = cmd_state_not_launched;
}


static void cd(struct cmd_t *cmd)
{
    int cdr;
    char *path;

    cmd->pid = 0;
    cmd->code = 1;
    cmd->state = cmd_state_exited;

    if (cmd->argc > 2) {
        print_error(error_cd_many_args);
        return;
    } else if (cmd->argc == 2) {
        path = cmd->argv[1];
    } else {
        path = getenv("HOME");
        if (!path) {
            print_error(error_cd_home);
            return;
        }
    }

    cdr = chdir(path);
    if (cdr == -1) {
        perror(path);
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
    cmd->state = cmd_state_launched;
}


static void print_env(struct cmd_t *cmd)
{
    const char *env;
    
    cmd->pid = 0;
    cmd->code = 1;
    cmd->state = cmd_state_exited;

    if (cmd->argc != 2) {
        print_error(error_env_args);
        return;
    }

    env = getenv(cmd->argv[1]);
    if (!env) {
        print_error(error_env_novariable);
        return;
    }

    printf("%s\n", env);
    cmd->code = 0;
}


static void is_loader(struct cmd_t *cmd)
{
    int fd;
    const char *s;
    unsigned short fs;

    if (cmd->argc != 2) {
        print_error(error_is_loader);
        return;
    }

    fd = open(cmd->argv[1], O_RDONLY);
    if (fd == -1) {
        perror(cmd->argv[1]);
        return;
    }

    lseek(fd, 510, SEEK_SET);
    read(fd, &fs, sizeof(fs));
    close(fd);

    if (fs == 0xaa55) {
        s = msg_is_loader;
    } else {
        s = msg_isn_loader;
    }
    printf("%s\n", s);
}


void cmd_exec(struct cmd_t *cmd)
{
    if (!cmd->name) {
        return;
    }

    if (strcmp(cmd->name, "cd") == 0) {
        cd(cmd);
    } else if (strcmp(cmd->name, ":q") == 0 || strcmp(cmd->name, "exit") == 0) {
        exit(0);
    } else if (strcmp(cmd->name, ":e") == 0) {
        print_env(cmd);
    } else if (strcmp(cmd->name, ":l") == 0) {
        is_loader(cmd);
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
        cmd->state = cmd_state_exited;
    } else {
        cmd->code = WTERMSIG(cmd->code);
        cmd->state = cmd_state_terminated;
    }
}
