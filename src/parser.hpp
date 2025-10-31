#ifndef PARSER_HPP
#define PARSER_HPP

#include "executor.hpp"

class Parser {
    Lexer &lexer;
    Token *look;
public:
    Parser(Lexer &lexer) : lexer(lexer) { Move(); }
    ~Parser() { delete look; }

    void SkipRestOfLine()
        { while (look->GetTag() != EOF && look->GetTag() != '\n') Skip(); }

    bool IsFinished() const { return look->GetTag() == EOF; }
    Node *Run();
private:
    void Skip() { delete look; Move(); }
    void SkipEmptyLines() { while (look->GetTag() == '\n') Skip(); }

    void Move() { look = lexer.Scan(); }
#if 0
    bool Match(int tag)
        { if (look->GetTag() == tag) { Skip(); return true; } return false; }
#endif

    SimpleCommand *ParseSimpleCommand();
};

#endif
