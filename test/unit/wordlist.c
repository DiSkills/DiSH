#include <unity.h>

#include "src/wordlist.h"


void setUp()
{
}


void tearDown()
{
}


static void test_init()
{
    struct wordlist_t wordlist;

    wordlist_init(&wordlist);
    TEST_ASSERT_NULL(wordlist.first);
    TEST_ASSERT_NULL(wordlist.last);

    wordlist_del(&wordlist);
}


static void test_push_back_first_item()
{
    struct wordlist_t wordlist;
    const char word[] = "word";
    wordlist_init(&wordlist);

    wordlist_push_back(&wordlist, word);
    TEST_ASSERT_NOT_NULL(wordlist.first);
    TEST_ASSERT_EQUAL_PTR(wordlist.first, wordlist.last);

    TEST_ASSERT_NOT_NULL(wordlist.first->word);
    TEST_ASSERT_FALSE(wordlist.first->word == word);
    TEST_ASSERT_EQUAL_STRING(word, wordlist.first->word);
    TEST_ASSERT_NULL(wordlist.first->next);

    wordlist_del(&wordlist);
}


static void test_push_back_two_items()
{
    struct wordlist_t wordlist;
    const char first[] = "The first",
          second[] = "The second";
    wordlist_init(&wordlist);
    wordlist_push_back(&wordlist, first);

    wordlist_push_back(&wordlist, second);
    TEST_ASSERT_FALSE(wordlist.first == wordlist.last);
    TEST_ASSERT_EQUAL_PTR(wordlist.first->next, wordlist.last);
    TEST_ASSERT_FALSE(wordlist.last->word == second);
    TEST_ASSERT_EQUAL_STRING(second, wordlist.last->word);
    TEST_ASSERT_NULL(wordlist.last->next);

    wordlist_del(&wordlist);
}


static void test_del()
{
    struct wordlist_t wordlist;
    const char first[] = "The first",
          second[] = "The second";
    wordlist_init(&wordlist);
    wordlist_push_back(&wordlist, first);
    wordlist_push_back(&wordlist, second);

    wordlist_del(&wordlist);
    TEST_ASSERT_NULL(wordlist.first);
    TEST_ASSERT_NULL(wordlist.last);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_init);
    RUN_TEST(test_push_back_first_item);
    RUN_TEST(test_push_back_two_items);
    RUN_TEST(test_del);

    return UNITY_END();
}
