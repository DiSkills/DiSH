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
