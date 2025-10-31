#include "parser.hpp"

Node *Parser::Run()
{
    SkipEmptyLines();
    Node *node = ParseSimpleCommand();
    if (look->GetTag() == '\n') {
        return node;
    }
    if (node) {
        delete node;
    }
    return 0;
}

SimpleCommand *Parser::ParseSimpleCommand()
{
    if (look->GetTag() == WORD) {
        Word *name = static_cast<Word *>(look);

        unsigned int len = 0;
        SimpleCommand::Argument *head = 0, *tail = 0;

        for (Move(); look->GetTag() == WORD; Move()) {
            SimpleCommand::Argument *tmp = new SimpleCommand::Argument(
                static_cast<Word *>(look)
            );
            if (tail) {
                tail->next = tmp;
            } else {
                head = tmp;
            }
            tail = tmp;

            len++;
        }
        return new SimpleCommand(name, head, len);
    }
    return 0;
}
