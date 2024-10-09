#ifndef CMD_H
#define CMD_H

#include "line.h"

enum cmd_state_t {
    cmd_state_not_launched = 0,
    cmd_state_launched = 1,
    cmd_state_exited = 2,
    cmd_state_terminated = 3
};

struct cmd_t {
    char *name;

    int argc;
    char **argv;

    enum cmd_state_t state;

    int pid,
        code;
};

void cmd_init_from_line(struct cmd_t *cmd, struct line_t *line);
void cmd_del(struct cmd_t *cmd);
void cmd_exec(struct cmd_t *cmd);
void cmd_wait(struct cmd_t *cmd);

#endif
