#include <assert.h>
#include <string.h>

#include "../src/wordlist.h"


typedef void (*test_func)();


static void test_init()
{
    struct wordlist_t wordlist;

    wordlist_init(&wordlist);
    assert(wordlist.first == NULL);
    assert(wordlist.last == NULL);

    wordlist_del(&wordlist);
}


static void test_push_back_first_item()
{
    const char word[] = "word";
    struct wordlist_t wordlist;
    wordlist_init(&wordlist);

    wordlist_push_back(&wordlist, word);
    assert(wordlist.first != NULL);
    assert(wordlist.first == wordlist.last);

    assert(wordlist.first->word != NULL);
    assert(wordlist.first->word != word);
    assert(strcmp(wordlist.first->word, word) == 0);
    assert(wordlist.first->next == NULL);

    wordlist_del(&wordlist);
}


static void test_push_back_two_items()
{
    const char first[] = "The first",
          second[] = "The second";
    struct wordlist_t wordlist;
    wordlist_init(&wordlist);
    wordlist_push_back(&wordlist, first);

    wordlist_push_back(&wordlist, second);
    assert(wordlist.first != wordlist.last);
    assert(wordlist.first->next == wordlist.last);
    assert(wordlist.last->word != second);
    assert(strcmp(wordlist.last->word, second) == 0);
    assert(wordlist.last->next == NULL);

    wordlist_del(&wordlist);
}


static void test_del()
{
    const char first[] = "The first",
          second[] = "The second";
    struct wordlist_t wordlist;
    wordlist_init(&wordlist);
    wordlist_push_back(&wordlist, first);
    wordlist_push_back(&wordlist, second);

    wordlist_del(&wordlist);
    assert(wordlist.first == NULL);
    assert(wordlist.last == NULL);
}


int main()
{
    int i;
    test_func tests[] = {
        test_init,
        test_push_back_first_item,
        test_push_back_two_items,
        test_del
    };
    for (i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
        tests[i]();
    }
    return 0;
}
