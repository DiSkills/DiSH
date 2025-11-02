#include "parser.hpp"

Pipeline *Parser::Run()
{
    SkipEmptyLines();
    Pipeline *node = ParsePipeline();
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
    if (look->GetTag() != WORD) {
        return 0;
    }
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

Pipeline *Parser::ParsePipeline()
{
    SimpleCommand *cmd = ParseSimpleCommand();
    if (!cmd) {
        return 0;
    }
    Pipeline::Item *head = new Pipeline::Item(cmd);
    Pipeline::Item *tail = head;
    unsigned int len = 1;
    while (Match(PIPE)) {
        cmd = ParseSimpleCommand();
        if (!cmd) {
            goto error;
        }
        Pipeline::Item *tmp = new Pipeline::Item(cmd);
        tail->next = tmp;
        tail = tmp;
        len++;
    }
    return new Pipeline(head, len);
error:
    while (head) {
        Pipeline::Item *tmp = head;
        head = head->next;
        delete tmp;
    }
    return 0;
}
