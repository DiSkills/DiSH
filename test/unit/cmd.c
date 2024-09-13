#include <unity.h>

#include "line_assert.h"

#include "src/cmd.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init_from_line()
{
    int i;
    struct cmd_t cmd;
    struct line_t line;
    const char s[] = "ls -l -a /\n";

    line_init(&line);
    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }

    cmd_init_from_line(&cmd, &line);
    TEST_ASSERT_EQUAL_STRING("ls", cmd.name);

    TEST_ASSERT_EQUAL(4, cmd.argc);
    TEST_ASSERT_EQUAL_STRING("ls", cmd.argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", cmd.argv[1]);
    TEST_ASSERT_EQUAL_STRING("-a", cmd.argv[2]);
    TEST_ASSERT_EQUAL_STRING("/", cmd.argv[3]);
    TEST_ASSERT_NULL(cmd.argv[4]);

    TEST_ASSERT_EQUAL_PTR(cmd.argv[0], cmd.name);

    TEST_ASSERT_EQUAL(state_not_launched, cmd.state);

    TEST_ASSERT_LINE(line, 0, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);
    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
    /* TODO cmd_del(&cmd); */
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init_from_line);

    return UNITY_END();
}
