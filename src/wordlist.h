#ifndef WORDLIST_H
#define WORDLIST_H

struct wordlist_item {
    char *word;
    struct wordlist_item *next;
};

struct wordlist_t {
    struct wordlist_item *first,
                         *last;
};

void wordlist_init(struct wordlist_t *wordlist);
void wordlist_del(struct wordlist_t *wordlist);

void wordlist_push_back(struct wordlist_t *wordlist, const char *word);

int wordlist_len(const struct wordlist_t *wordlist);

#endif
