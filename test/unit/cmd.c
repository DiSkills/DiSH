#include <unity.h>

#include "line_assert.h"

#include "src/cmd.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init()
{
    struct cmd_t cmd;

    cmd_init(&cmd);
    TEST_ASSERT_NULL(cmd.name);
    TEST_ASSERT_EQUAL_INT(0, cmd.argc);
    TEST_ASSERT_NULL(cmd.argv);
    TEST_ASSERT_EQUAL_INT(state_not_launched, cmd.state);
    TEST_ASSERT_EQUAL_INT(0, cmd.pid);
    TEST_ASSERT_EQUAL_INT(0, cmd.code);

    cmd_del(&cmd);
}


static void test_line_to_cmd()
{
    int i;
    struct cmd_t cmd;
    struct line_t line;
    const char s[] = "ls -l -a /\n";

    cmd_init(&cmd);
    line_init(&line);
    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }

    line_to_cmd(&line, &cmd);
    TEST_ASSERT_EQUAL_STRING("ls", cmd.name);
    TEST_ASSERT_EQUAL_INT(4, cmd.argc);
    TEST_ASSERT_EQUAL_INT(state_not_launched, cmd.state);

    TEST_ASSERT_EQUAL_STRING("ls", cmd.argv[0]);
    TEST_ASSERT_EQUAL_STRING("-l", cmd.argv[1]);
    TEST_ASSERT_EQUAL_STRING("-a", cmd.argv[2]);
    TEST_ASSERT_EQUAL_STRING("/", cmd.argv[3]);

    TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(line);

    line_del(&line);
    cmd_del(&cmd);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_line_to_cmd);
    
    return UNITY_END();
}
