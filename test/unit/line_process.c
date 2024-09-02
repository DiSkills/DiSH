#include <unity.h>

#include "line_assert.h"

#include "src/line.h"


void setUp()
{
}


void tearDown()
{
}


static void test_correctly()
{   /* abra sch"wa"bra" "kadabra    foo"    b"ar */
    int i;
    struct line_t line;
    const struct wordlist_item *item;
    const char s[] = "abra sch\"wa\"bra\" \"kadabra\tfoo\"\tb\"ar\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 0, 32, "");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("abra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("schwabra kadabra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("foo\tbar", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


static void test_escape_sequence_correctly()
{   /* It\"s the \\correct\\ "\"escape\" sequence" */
    int i;
    struct line_t line;
    struct wordlist_item *item;
    const char s[] =
        "It\\\"s the \\\\correct\\\\ \"\\\"escape\\\" sequence\"\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD(line, 0, 32, "");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("It\"s", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("the", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("\\correct\\", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("\"escape\" sequence", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


static void test_empty_word_correctly()
{   /* abra"" "" ""\\ schw""abra """"  ""kadabra "" */
    int i;
    struct line_t line;
    struct wordlist_item *item;
    const char s[] =
        "abra\"\" \"\" \"\"\\\\ schw\"\"abra \"\"\"\"\t\"\"kadabra \"\"\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 0, 0, noerror, mode_split);
    TEST_ASSERT_LINE_WORD_IS_DEFAULT(line);

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("abra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("\\", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("schwabra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("kadabra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


static void test_error_unmatched_quotes()
{   /* abra "schwabra kadabra"  "foo */
    int i;
    struct line_t line;
    struct wordlist_item *item;
    const char s[] = "abra \"schwabra kadabra\"\t\"foo\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 0, 0, error_quotes, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 4, 32, "foo\n");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("abra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("schwabra kadabra", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


static void test_error_unsupported_escape_sequence()
{   /* It's \"an\" unsupported\? escape sequence */
    int i;
    struct line_t line;
    struct wordlist_item *item;
    const char s[] = "It's \\\"an\\\" unsupported\\? escape sequence\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 1, 0, error_escape, mode_split);
    TEST_ASSERT_LINE_WORD(line, 11, 16, "unsupported");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("It's", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("\"an\"", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


static void test_check_to_save_first_error()
{   /* check "to save\? the first error */
    int i;
    struct line_t line;
    struct wordlist_item *item;
    const char s[] = "check \"to save\\? the first error\n";
    line_init(&line);

    for (i = 0; s[i]; i++) {
        line_process_char(&line, s[i]);
    }
    TEST_ASSERT_LINE(line, 1, 1, 0, error_escape, mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 7, 16, "to save");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("check", item->word);
    TEST_ASSERT_NULL(item->next);

    line_del(&line);
}


int main()
{
    UNITY_BEGIN();

    RUN_TEST(test_correctly);
    RUN_TEST(test_escape_sequence_correctly);
    RUN_TEST(test_empty_word_correctly);
    RUN_TEST(test_error_unmatched_quotes);
    RUN_TEST(test_error_unsupported_escape_sequence);
    RUN_TEST(test_check_to_save_first_error);

    return UNITY_END();
}
