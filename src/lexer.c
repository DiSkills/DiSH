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


static void lexer_add_token(struct lexer_t *lexer, enum token_type_t type)
{
    token_list_push(&lexer->tokens, lexer->buffer.data, type);
    str_clear(&lexer->buffer);
}


static void lexer_initial(struct lexer_t *lexer, char c)
{
    switch (c) {
        case ' ':
        case '\t':
            return;

        case '<':
        case ';':
        case '(':
        case ')':
            str_append(&lexer->buffer, c);
            lexer_add_token(lexer, token_type_delimiter);
            return;

        case '\n':
            lexer->state = lexer_state_completed;
            return;

        case '\\':
            lexer->state = lexer_state_escaping_in_word;
            return;

        case '"':
            lexer->state = lexer_state_reading_string;
            return;

        case '|':
            lexer->state = lexer_state_pipe;
            break;
        case '&':
            lexer->state = lexer_state_ampersand;
            break;
        case '>':
            lexer->state = lexer_state_greater;
            break;

        default:
            lexer->state = lexer_state_reading_word;
    }

    str_append(&lexer->buffer, c);
}


void lexer_process_char(struct lexer_t *lexer, char c)
{
    switch (lexer->state) {
        case lexer_state_initial:
            lexer_initial(lexer, c);
            return;
    }
}
