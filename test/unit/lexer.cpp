#include <stdio.h>

#include <gtest/gtest.h>

#include "lexer.hpp"

using namespace Tokens;

class ExpectedWord : public Token {
    const char *lexeme;
public:
    explicit ExpectedWord(const char *lexeme)
        : Token(WORD), lexeme(lexeme) {}
    virtual ~ExpectedWord() {};

    const char *GetLexeme() const { return lexeme; }
private:
    ExpectedWord(const ExpectedWord &);
    void operator=(const ExpectedWord &);
};

class TestParam {
    const char *line;
    std::vector<Token*> tokens;
public:
    TestParam(const char *line, std::vector<Token*> tokens)
        : line(line), tokens(tokens) {}
    const char *GetLine() const { return line; }
    const std::vector<Token*> &GetTokens() const { return tokens; }
};

class LexerTest : public testing::TestWithParam<TestParam> {
    FILE *save_stdin;
protected:
    virtual void SetUp();
    virtual void TearDown();
};

void LexerTest::SetUp()
{
    save_stdin = stdin;
    const char *line = GetParam().GetLine();
    FILE *stream = fmemopen(const_cast<char*>(line), strlen(line), "r");
    stdin = stream;
}

void LexerTest::TearDown()
{
    fclose(stdin);
    stdin = save_stdin;
}

TEST_P(LexerTest, Scan)
{
    Lexer lexer;
    for (Token *expected: GetParam().GetTokens()) {
        Token *token = lexer.Scan();

        EXPECT_EQ(token->GetTag(), expected->GetTag());
        if (token->GetTag() == WORD) {
            Word *word = static_cast<Word*>(token);
            ExpectedWord *expected_word = static_cast<ExpectedWord*>(expected);
            EXPECT_STREQ(word->GetLexeme(), expected_word->GetLexeme());
        }

        delete token;
        delete expected;
    }
}

INSTANTIATE_TEST_SUITE_P(
    LexerTestCase,
    LexerTest,
    testing::Values(
        TestParam("ls -la\n", {
            new ExpectedWord("ls"), new ExpectedWord("-la"),
            new Token(NEWLINE), new Token(END)
        }),
        TestParam("\nls\n-la&&echo\t123\n\t\t\t\n\t", {
            new Token(NEWLINE), new ExpectedWord("ls"),
            new Token(NEWLINE), new ExpectedWord("-la"),
            new Token(AND), new ExpectedWord("echo"),
            new ExpectedWord("123"), new Token(NEWLINE),
            new Token(NEWLINE), new Token(END)
        }),
        TestParam("\n||&&;&><>,test.h", {
            new Token(NEWLINE), new Token(OR), new Token(PIPE),
            new Token(AND), new Token(SEQUENCE), new Token(BACKGROUND),
            new Token(OUTPUT_REDIRECTION), new Token(INPUT_REDIRECTION),
            new Token(OUTPUT_REDIRECTION), new ExpectedWord(",test.h")
        })
    )
);
