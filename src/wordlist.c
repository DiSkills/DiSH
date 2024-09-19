#include <stdlib.h>
#include <string.h>

#include "wordlist.h"


void wordlist_init(struct wordlist_t *wordlist)
{
    wordlist->first = NULL;
    wordlist->last = NULL;
}


void wordlist_del(struct wordlist_t *wordlist)
{
    while (wordlist->first) {
        struct wordlist_item *tmp;

        tmp = wordlist->first;
        wordlist->first = tmp->next;

        if (tmp->word) {
            free(tmp->word);
        }
        free(tmp);
    }
    wordlist->last = NULL;
}


void wordlist_push_back(struct wordlist_t *wordlist, const char *word)
{
    struct wordlist_item *tmp;

    tmp = malloc(sizeof(*tmp));
    tmp->word = malloc(strlen(word) + 1);
    strcpy(tmp->word, word);
    tmp->next = NULL;

    if (wordlist->last) {
        wordlist->last->next = tmp;
    } else {
        wordlist->first = tmp;
    }
    wordlist->last = tmp;
}


void wordlist_traverse(struct wordlist_t *wordlist,
        wordlist_callback callback, void *userdata)
{
    struct wordlist_item *p;

    for (p = wordlist->first; p; p = p->next) {
        callback(p, userdata);
    }
}


int wordlist_length(const struct wordlist_t *wordlist)
{
    int length = 0;
    const struct wordlist_item *p;

    for (p = wordlist->first; p; p = p->next) {
        length++;
    }

    return length;
}
