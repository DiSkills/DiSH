#include "parser.hpp"

int main()
{
    Lexer lexer;
    printf(">>> ");
    Parser parser(lexer);
    do {
        Pipeline *root = parser.Run();
        if (root) {
            root->Execute();
            root->Wait();
            printf(">>> ");
            delete root;
        } else if (!parser.IsFinished()) {
            fprintf(stderr, "Syntax error\n");
            parser.SkipRestOfLine();
        }
    } while (!parser.IsFinished());
    return 0;
}
