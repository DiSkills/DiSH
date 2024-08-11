#include <stdlib.h>
#include <string.h>

#include "str.h"


enum {
    min_str_size = 16
};


void str_init(struct str_t *str)
{
    str->data = malloc(min_str_size);
    str->max_size = min_str_size;
    str_clear(str);
}


void str_del(struct str_t *str)
{
    free(str->data);
    str->size = 0;
    str->max_size = 0;
    str->data = NULL;
}


void str_clear(struct str_t *str)
{
    str->data[0] = '\0';
    str->size = 0;
}


static int str_is_full(struct str_t *str)
{
    return str->size + 1 == str->max_size;
}


static void str_resize(struct str_t *str)
{
    char *tmp = str->data;

    str->max_size *= 2;
    str->data = malloc(str->max_size);
    strcpy(str->data, tmp);

    free(tmp);
}


void str_append(char c, struct str_t *str)
{
    if (str_is_full(str)) {
        str_resize(str);
    }
    str->data[str->size] = c;
    str->size++;
    str->data[str->size] = '\0';
}
