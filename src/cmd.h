#ifndef CMD_H
#define CMD_H

#include "line.h"

enum cmd_states {
    state_not_launched = 0,
    state_launched = 1,
    state_exited = 2,
    state_terminated = 3
};

struct cmd_t {
    const char *name;

    int argc;
    const char **argv;

    enum cmd_states state;

    int pid,
        code;
};

void cmd_init(struct cmd_t *cmd);
void cmd_del(struct cmd_t *cmd);
void line_to_cmd(struct line_t *line, struct cmd_t *cmd);
void cmd_execute(struct cmd_t *cmd);

#endif
