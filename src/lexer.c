#include "lexer.h"


void lexer_init(struct lexer_t *lexer)
{
    lexer->state = lexer_state_initial;
    lexer->errno = lexer_error_noerror;

    str_init(&lexer->buffer);
    token_list_init(&lexer->tokens);
}


void lexer_clear(struct lexer_t *lexer)
{
    lexer->state = lexer_state_initial;
    lexer->errno = lexer_error_noerror;

    str_clear(&lexer->buffer);
    token_list_del(&lexer->tokens);
}


void lexer_del(struct lexer_t *lexer)
{
    lexer->state = lexer_state_initial;
    lexer->errno = lexer_error_noerror;

    str_del(&lexer->buffer);
    token_list_del(&lexer->tokens);
}
