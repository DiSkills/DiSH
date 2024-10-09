#ifndef WORDLIST_H
#define WORDLIST_H

struct wordlist_item_t {
    char *word;
    struct wordlist_item_t *next;
};

struct wordlist_t {
    struct wordlist_item_t *first,
                           *last;
};

void wordlist_init(struct wordlist_t *wordlist);
void wordlist_del(struct wordlist_t *wordlist);
void wordlist_push_back(struct wordlist_t *wordlist, const char *word);

int wordlist_length(const struct wordlist_t *wordlist);
int wordlist_is_empty(const struct wordlist_t *wordlist);

#endif
