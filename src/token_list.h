#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H

enum token_type_t {
    token_type_word,
    token_type_delimiter
};

struct token_t {
    char *content;
    enum token_type_t type;
};

struct token_list_item_t {
    struct token_t token;

    struct token_list_item_t *next;
};

struct token_list_t {
    int length;

    struct token_list_item_t *head,
                             *tail;
};

void token_list_init(struct token_list_t *tknlst);
void token_list_del(struct token_list_t *tknlst);
void token_list_push(struct token_list_t *tknlst,
        const char *s, enum token_type_t type);

#endif
