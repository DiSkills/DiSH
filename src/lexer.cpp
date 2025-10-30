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

Token *Lexer::GetOperatorToken() const
{
    switch (peek) {
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

Token *Lexer::ScanOperator()
{
    switch (peek) {
    case '&':
        if (GetChar('&'))
            return new Token(AND);
        return new Token(BACKGROUND);
    case '>':
        if (GetChar('>'))
            return new Token(APPEND_REDIRECTION);
        return new Token(OUTPUT_REDIRECTION);
    case '|':
        if (GetChar('|'))
            return new Token(OR);
        return new Token(PIPE);
    }
    Token *token = GetOperatorToken();
    if (token) {
        ResetPeek();
    }
    return token;
}

Token *Lexer::ScanLexeme()
{
    lexeme.Clear();
    do {
        lexeme.Append(peek);
        GetChar();
    } while (!IsSpace(peek) && !IsSpecialChar(peek));
    return new Word(lexeme.GetString());
}

Token *Lexer::Scan()
{
    SkipSpaces();
    Token *token = ScanOperator();
    if (token) {
        return token;
    }
    return ScanLexeme();
}
