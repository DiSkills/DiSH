#include <stdio.h>

#include "error.h"
#include "message.h"


const char error_quotes[] = "unmatched quotes";
const char error_escape[] = "unsupported escape sequence";

const char error_cd_home[] = "cd: unknown path to home directory";
const char error_cd_many_args[] = "cd: too many arguments";

const char error_env_args[] = "env: incorrect number of arguments";
const char error_env_novariable[] = "env: no variable";

const char error_is_loader[] = "l: incorrect number of arguments";


void print_error(const char *error)
{
    fprintf(stderr, "%s: %s\n", msg_error, error);
}
