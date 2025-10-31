#include "executor.hpp"

SimpleCommand::~SimpleCommand()
{
    delete name;
    while (args) {
        Argument *tmp = args;
        args = args->next;
        delete tmp->arg;
        delete tmp;
    }
}

void SimpleCommand::Execute() const
{
    printf("[%s]", name->GetLexeme());
    for (Argument *p = args; p; p = p->next) {
        printf(" [%s]", p->arg->GetLexeme());
    }
    putchar('\n');
}
