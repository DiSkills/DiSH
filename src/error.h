#ifndef ERROR_H
#define ERROR_H

extern const char error_quotes[];
extern const char error_escape[];

extern const char error_cd_home[];
extern const char error_cd_many_args[];

extern const char error_env_many_args[];
extern const char error_env_novariable[];

void print_error(const char *error);

#endif
