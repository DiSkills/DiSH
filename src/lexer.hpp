#ifndef LEXER_HPP
#define LEXER_HPP

#include <stdio.h>

#include "string_buffer.hpp"

enum {
    BACKGROUND = '&',
    SEQUENCE = ';',

    INPUT_REDIRECTION = '<',
    OUTPUT_REDIRECTION = '>',
    APPEND_REDIRECTION = 256,

    PIPE = '|',

    LBRACKET = '(',
    RBRACKET = ')',

    AND = 257,
    OR = 258,

    WORD = 259,
};

class Token {
    int tag;
public:
    Token(int tag) : tag(tag) {}
    virtual ~Token() {}
    int GetTag() const { return tag; }
};

class Word : public Token {
    char *lexeme;
public:
    Word(char *lexeme) : Token(WORD), lexeme(lexeme) {}
    virtual ~Word() { delete[] lexeme; }
    const char *GetLexeme() const { return lexeme; }
private:
    Word(const Word &);
    void operator=(const Word &);
};

class Lexer {
    int peek;
    StringBuffer lexeme;
public:
    Lexer() : peek(' ') {}
    Token *Scan();
private:
    void GetChar() { peek = getchar(); }
    bool GetChar(char c)
        { GetChar(); if (peek != c) return false; ResetPeek(); return true; }
    void ResetPeek() { peek = ' '; }

    Token *GetOperatorToken() const;
    void SkipSpaces() { while (IsSpace(peek)) GetChar(); }
    Token *ScanLexeme();
    Token *ScanOperator();

    static bool IsSpace(char c) { return c == ' ' || c == '\t'; }
    static bool IsSpecialChar(char c);
private:
    Lexer(const Lexer &);
    void operator=(const Lexer &);
};

#endif
