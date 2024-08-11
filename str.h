#ifndef STR_H
#define STR_H

struct str_t {
    unsigned size,
             max_size;
    char *data;
};


void str_init(struct str_t *str);
void str_del(struct str_t *str);
void str_clear(struct str_t *str);
void str_append(char c, struct str_t *str);

#endif
