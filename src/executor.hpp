#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "lexer.hpp"

class SimpleCommand {   
public:
    struct Argument {
        Word *arg;
        Argument *next;
    public:
        Argument(Word *arg, Argument *next = 0) : arg(arg), next(next) {}
        ~Argument() { delete arg; }
    };
private:
    Word *name;
    Argument *args;
    unsigned int len;
public:
    SimpleCommand(Word *name, Argument *args, unsigned int len)
        : name(name), args(args), len(len) {}
    ~SimpleCommand();
    void Execute() const;
};

class Pipeline {
public:
    struct Item {
        SimpleCommand *cmd;
        Item *next;
    public:
        Item(SimpleCommand *cmd, Item *next = 0) : cmd(cmd), next(next) {}
        ~Item() { delete cmd; }
    };
private:
    Item *commands;
    unsigned int len;
public:
    Pipeline(Item *commands, unsigned int len)
        : commands(commands), len(len) {}
    ~Pipeline();
    int Execute() {
        if (len == 1)
            return ExecuteSimplestCase();
        return ExecuteCompositeCase();
    }
    int ExecuteSimplestCase() const;
    int ExecuteCompositeCase() const;

    int ExecuteFirstProcess(SimpleCommand *cmd, int fdin, int fdout) const;
    int ExecuteMiddleProcess(SimpleCommand *cmd, int fd,
            int fdin, int fdout) const;
    int ExecuteLastProcess(SimpleCommand *cmd, int fdin) const;

    void Wait() const;
};

#endif
