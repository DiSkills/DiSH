#ifndef LEXER_H
#define LEXER_H

#include "str.h"
#include "token_list.h"

enum lexer_state_t {
    lexer_state_initial,

    lexer_state_error,
    lexer_state_completed,

    lexer_state_reading_word,
    lexer_state_escaping_in_word,

    lexer_state_reading_string,
    lexer_state_escaping_in_string,

    lexer_state_pipe,
    lexer_state_double_pipe,

    lexer_state_ampersand,
    lexer_state_double_ampersand,
    
    lexer_state_greater,
    lexer_state_double_greater
};

enum lexer_error_t {
    lexer_error_noerror = 0,
    lexer_error_delimiter,
    lexer_error_escape,
    lexer_error_quotes
};

struct lexer_t {
    enum lexer_state_t state;
    enum lexer_error_t errno;

    struct str_t buffer;
    struct token_list_t tokens;
};

void lexer_init(struct lexer_t *lexer);
void lexer_clear(struct lexer_t *lexer);
void lexer_del(struct lexer_t *lexer);

void lexer_process_char(struct lexer_t *lexer, char c);

#endif
