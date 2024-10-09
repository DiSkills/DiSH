#include <stdio.h>

#include "error.h"
#include "message.h"


const char error_quotes[] = "unmatched quotes";
const char error_escape[] = "unsupported escape sequence";

const char error_cd_home[] = "cd: unknown path to home directory";
const char error_cd_many_args[] = "cd: too many arguments";


void print_error(const char *error)
{
    fprintf(stderr, "%s: %s\n", msg_error, error);
}
