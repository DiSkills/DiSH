#ifndef WORD_H
#define WORD_H

struct word_t {
    unsigned size,
             max_size;
    char *data;
};


void word_init(struct word_t *word);
void word_del(struct word_t *word);
void word_clear(struct word_t *word);
void word_append(char c, struct word_t *word);

#endif
