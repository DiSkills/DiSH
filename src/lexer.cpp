#include "lexer.hpp"

bool Lexer::IsSpecialChar(char c)
{
    switch (c) {
    case '&':
    case '>':
    case '|':
    case ';':
    case '<':
    case '(':
    case ')':
        return true;
    }
    return false;
}

Token *Lexer::ScanOperator()
{
    switch (peek) {
    case '&':
        if (GetChar('&')) {
            return new Token(AND);
        }
        return new Token(BACKGROUND);
    case '>':
        if (GetChar('>')) {
            return new Token(APPEND_REDIRECTION);
        }
        return new Token(OUTPUT_REDIRECTION);
    case '|':
        if (GetChar('|')) {
            return new Token(OR);
        }
        return new Token(PIPE);
    case ';':
        return new Token(SEQUENCE);
    case '<':
        return new Token(INPUT_REDIRECTION);
    case '(':
        return new Token(LBRACKET);
    case ')':
        return new Token(RBRACKET);
    }
    return 0;
}

#if 0
Token *Lexer::ScanLexeme()
{
    do {
        /* putchar(peek); */
        GetChar();
    } while (!IsSpace(peek) && !IsSpecialChar(peek));
    /* return 0; */
}
#endif

Token *Lexer::Scan()
{
    SkipSpaces();
    Token *token = ScanOperator();
    if (token) {
        return token;
    }
    return ScanLexeme();
}
