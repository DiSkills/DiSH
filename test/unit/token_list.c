#include <unity/unity.h>

#include "src/token_list.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init()
{
    struct token_list_t tknlst;

    token_list_init(&tknlst);
    TEST_ASSERT_NULL(tknlst.head);
    TEST_ASSERT_NULL(tknlst.tail);
    TEST_ASSERT_EQUAL_INT(0, tknlst.length);

    token_list_del(&tknlst);
}


static void test_del()
{
    struct token_list_t tknlst;
    const char word[] = "word",
          delimiter[] = "&";

    token_list_init(&tknlst);
    token_list_push(&tknlst, word, token_type_word);
    token_list_push(&tknlst, delimiter, token_type_delimiter);

    token_list_del(&tknlst);
    TEST_ASSERT_NULL(tknlst.head);
    TEST_ASSERT_NULL(tknlst.tail);
    TEST_ASSERT_EQUAL_INT(0, tknlst.length);
}


static void test_push()
{
    struct token_list_t tknlst;
    const struct token_list_item_t *p;
    const char word[] = "word",
          delimiter[] = "&";

    token_list_init(&tknlst);

    /* The first item */
    token_list_push(&tknlst, word, token_type_word);
    TEST_ASSERT_NOT_NULL(tknlst.head);
    TEST_ASSERT_EQUAL_PTR(tknlst.head, tknlst.tail);
    TEST_ASSERT_EQUAL_INT(1, tknlst.length);

    p = tknlst.tail;
    TEST_ASSERT_FALSE(p->token.content == word);
    TEST_ASSERT_EQUAL_STRING(word, p->token.content);
    TEST_ASSERT_EQUAL_INT(token_type_word, p->token.type);
    TEST_ASSERT_NULL(p->next);

    /* The second item */
    token_list_push(&tknlst, delimiter, token_type_delimiter);
    TEST_ASSERT_FALSE(tknlst.head == tknlst.tail);
    TEST_ASSERT_EQUAL_PTR(tknlst.head->next, tknlst.tail);
    TEST_ASSERT_EQUAL_INT(2, tknlst.length);

    p = tknlst.tail;
    TEST_ASSERT_FALSE(p->token.content == delimiter);
    TEST_ASSERT_EQUAL_STRING(delimiter, p->token.content);
    TEST_ASSERT_EQUAL_INT(token_type_delimiter, p->token.type);
    TEST_ASSERT_NULL(p->next);

    token_list_del(&tknlst);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_del);
    RUN_TEST(test_push);

    UNITY_END();
}
