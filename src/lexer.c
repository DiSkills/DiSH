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
            str_append(&lexer->buffer, c);
            lexer_add_token(lexer, token_type_stdin_redirection);
            return;

        case ';':
            str_append(&lexer->buffer, c);
            lexer_add_token(lexer, token_type_sequential_execution);
            return;

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
            str_append(&lexer->buffer, c);
            return;

        case '&':
            lexer->state = lexer_state_ampersand;
            str_append(&lexer->buffer, c);
            return;

        case '>':
            lexer->state = lexer_state_greater;
            str_append(&lexer->buffer, c);
            return;

        default:
            lexer->state = lexer_state_reading_word;
            str_append(&lexer->buffer, c);
            return;
    }
}


static void lexer_pipe(struct lexer_t *lexer, char c)
{
    if (c == '|') {
        lexer->state = lexer_state_double_pipe;
        str_append(&lexer->buffer, c);
    } else {
        lexer->state = lexer_state_initial;
        lexer_add_token(lexer, token_type_pipeline);
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
        lexer_add_token(lexer, token_type_or);
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
        lexer_add_token(lexer, token_type_background);
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
        lexer_add_token(lexer, token_type_and);
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
        lexer_add_token(lexer, token_type_stdout_redirection);
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
        lexer_add_token(lexer, token_type_stdout_redirection_append);
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


static void lexer_reading_word(struct lexer_t *lexer, char c)
{
    switch (c) {
        case '\\':
            lexer->state = lexer_state_escaping_in_word;
            return;

        case '"':
            lexer->state = lexer_state_reading_string;
            return;

        case '\n':
        case ' ':
        case '\t':
        case '<':
        case ';':
        case '(':
        case ')':
        case '|':
        case '&':
        case '>':
            lexer->state = lexer_state_initial;
            lexer_add_token(lexer, token_type_word);
            lexer_process_char(lexer, c);
            return;

        default:
            str_append(&lexer->buffer, c);
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


static void lexer_reading_string(struct lexer_t *lexer, char c)
{
    switch (c) {
        case '\\':
            lexer->state = lexer_state_escaping_in_string;
            return;

        case '"':
            lexer->state = lexer_state_reading_word;
            return;

        case '\n':
            lexer->state = lexer_state_error;
            lexer->errno = lexer_error_quotes;
            return;

        default:
            str_append(&lexer->buffer, c);
    }
}


void lexer_process_char(struct lexer_t *lexer, char c)
{
    switch (lexer->state) {
        case lexer_state_initial:
            lexer_initial(lexer, c);
            return;

        case lexer_state_error:
            return;
        case lexer_state_completed:
            return;

        case lexer_state_reading_word:
            lexer_reading_word(lexer, c);
            return;
        case lexer_state_escaping_in_word:
            lexer_escaping_in_word(lexer, c);
            return;

        case lexer_state_reading_string:
            lexer_reading_string(lexer, c);
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
