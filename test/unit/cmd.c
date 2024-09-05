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


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    
    return UNITY_END();
}
