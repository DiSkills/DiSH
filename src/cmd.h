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
    char *name;

    int argc;
    char **argv;

    enum cmd_states state;

    int pid,
        code;
};

void cmd_init_from_line(struct cmd_t *cmd, struct line_t *line);

#endif
