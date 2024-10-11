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


static void lexer_pipe(struct lexer_t *lexer, char c)
{
    if (c == '|') {
        lexer->state = lexer_state_double_pipe;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_double_pipe(struct lexer_t *lexer, char c)
{
    if (c == '|') {
        lexer->state = lexer_state_error;
        lexer->errno = lexer_error_delimiter;
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_ampersand(struct lexer_t *lexer, char c)
{
    if (c == '&') {
        lexer->state = lexer_state_double_ampersand;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_double_ampersand(struct lexer_t *lexer, char c)
{
    if (c == '&') {
        lexer->state = lexer_state_error;
        lexer->errno = lexer_error_delimiter;
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_greater(struct lexer_t *lexer, char c)
{
    if (c == '>') {
        lexer->state = lexer_state_double_greater;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_double_greater(struct lexer_t *lexer, char c)
{
    if (c == '>') {
        lexer->state = lexer_state_error;
        lexer->errno = lexer_error_delimiter;
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_delimiter);
        lexer_process_char(lexer, c);
    }
}


static void lexer_escaping_in_word(struct lexer_t *lexer, char c)
{
    if (c == '\\' || c == '"') {
        lexer->state = lexer_state_reading_word;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_error;
        lexer->errno = lexer_error_escape;
    }
}


static void lexer_escaping_in_string(struct lexer_t *lexer, char c)
{
    if (c == '\\' || c == '"') {
        lexer->state = lexer_state_reading_string;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_error;
        lexer->errno = lexer_error_escape;
    }
}


void lexer_process_char(struct lexer_t *lexer, char c)
{
    switch (lexer->state) {
        case lexer_state_initial:
            lexer_initial(lexer, c);
            return;

        case lexer_state_error:
            /* TODO */
            return;
        case lexer_state_completed:
            /* TODO */
            return;

        case lexer_state_reading_word:
            /* TODO */
            return;
        case lexer_state_escaping_in_word:
            lexer_escaping_in_word(lexer, c);
            return;

        case lexer_state_reading_string:
            /* TODO */
            return;
        case lexer_state_escaping_in_string:
            lexer_escaping_in_string(lexer, c);
            return;

        case lexer_state_pipe:
            lexer_pipe(lexer, c);
            return;
        case lexer_state_double_pipe:
            lexer_double_pipe(lexer, c);
            return;

        case lexer_state_ampersand:
            lexer_ampersand(lexer, c);
            return;
        case lexer_state_double_ampersand:
            lexer_double_ampersand(lexer, c);
            return;

        case lexer_state_greater:
            lexer_greater(lexer, c);
            return;
        case lexer_state_double_greater:
            lexer_double_greater(lexer, c);
            return;
    }
}
