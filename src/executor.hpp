#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "lexer.hpp"

class Node {
public:
    virtual ~Node() {}
    virtual void Execute() const = 0;
};

class SimpleCommand : public Node {   
public:
    struct Argument {
        Word *arg;
        Argument *next;
    };
private:
    Word *name;
    Argument *args;
    unsigned int len;
public:
    SimpleCommand(Word *name, Argument *args, unsigned int len)
        : name(name), args(args), len(len) {}
    virtual ~SimpleCommand();

    virtual void Execute() const;
};

#endif
