#include <stdlib.h>
#include <string.h>

#include "word.h"


enum {
    min_word_size = 16
};


void word_init(struct word_t *word)
{
    word->data = malloc(min_word_size * sizeof(char));
    word->max_size = min_word_size;
    word_clear(word);
}


void word_del(struct word_t *word)
{
    free(word->data);
    word->size = 0;
    word->max_size = 0;
    word->data = NULL;
}


void word_clear(struct word_t *word)
{
    word->data[0] = '\0';
    word->size = 0;
}


static int word_is_full(struct word_t *word)
{
    return word->size + 1 == word->max_size;
}


static void word_resize(struct word_t *word)
{
    char *tmp = word->data;

    word->max_size *= 2;
    word->data = malloc(word->max_size * sizeof(char));
    strcpy(word->data, tmp);

    free(tmp);
}


void word_append(char c, struct word_t *word)
{
    if (word_is_full(word)) {
        word_resize(word);
    }
    word->data[word->size] = c;
    word->size++;
    word->data[word->size] = '\0';
}
