#include "executor.hpp"

SimpleCommand::~SimpleCommand()
{
    delete name;
    while (args) {
        Argument *tmp = args;
        args = args->next;
        delete tmp;
    }
}

void SimpleCommand::Execute() const
{
    printf("[%s]", name->GetLexeme());
    for (Argument *p = args; p; p = p->next) {
        printf(" [%s]", p->arg->GetLexeme());
    }
}

Pipeline::~Pipeline()
{
    while (commands) {
        Item *tmp = commands;
        commands = commands->next;
        delete tmp;
    }
}

void Pipeline::Execute() const
{
    commands->cmd->Execute();
    for (Item *p = commands->next; p; p = p->next) {
        printf(" | ");
        p->cmd->Execute();
    }
}
