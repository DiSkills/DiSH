#ifndef LEXER_HPP
#define LEXER_HPP

#include <stdio.h>

#include "string_buffer.hpp"

namespace Tokens {

enum Tag {
    BACKGROUND, SEQUENCE,
    INPUT_REDIRECTION, OUTPUT_REDIRECTION, APPEND_REDIRECTION,
    PIPE,
    LBRACKET, RBRACKET,
    AND, OR,
    WORD,
    NEWLINE, END,
};

class Token {
    Tag tag;
public:
    explicit Token(Tag tag) : tag(tag) {}
    virtual ~Token() {}

    Tag GetTag() const { return tag; }
};

class Word : public Token {
    char *lexeme;
public:
    explicit Word(char *lexeme) : Token(WORD), lexeme(lexeme) {}
    virtual ~Word() { delete[] lexeme; }

    char *GetLexeme() const { return lexeme; }
private:
    Word(const Word &);
    void operator=(const Word &);
};

};

class Lexer {
    int peek;
    StringBuffer lexeme;
public:
    Lexer() : peek(' ') {}
    Tokens::Token *Scan();
private:
    void GetChar() { peek = getchar(); }
    bool GetChar(char c)
        { GetChar(); if (peek != c) return false; ResetPeek(); return true; }
    void ResetPeek() { peek = ' '; }

    Tokens::Token *GetOperatorToken() const;
    void SkipSpaces() { while (IsSpace(peek)) GetChar(); }
    Tokens::Word *ScanLexeme();
    Tokens::Token *ScanOperator();

    static bool IsSpace(char c) { return c == ' ' || c == '\t'; }
    static bool IsSpecialChar(char c);
private:
    Lexer(const Lexer &);
    void operator=(const Lexer &);
};

#endif
