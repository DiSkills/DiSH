#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "token_list.h"


void token_list_init(struct token_list_t *tknlst)
{
    tknlst->length = 0;

    tknlst->head = NULL;
    tknlst->tail = NULL;
}


void token_list_del(struct token_list_t *tknlst)
{
    while (tknlst->head) {
        struct token_list_item_t *tmp;

        tmp = tknlst->head;
        tknlst->head = tmp->next;

        free(tmp->token.content);
        free(tmp);
    }

    tknlst->tail = NULL;
    tknlst->length = 0;
}


void token_list_push(struct token_list_t *tknlst,
        const char *s, enum token_type_t type)
{
    struct token_list_item_t *tmp;

    tmp = malloc(sizeof(*tmp));
    tmp->token.content = strdup(s);
    tmp->token.type = type;
    tmp->next = NULL;

    if (tknlst->tail) {
        tknlst->tail->next = tmp;
    } else {
        tknlst->head = tmp;
    }
    tknlst->tail = tmp;
    tknlst->length++;
}
