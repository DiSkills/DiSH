#include <unity/unity.h>

#include "line_assert.h"

#include "src/line.h"


static struct line_t line;
static const struct wordlist_item *item;


void setUp()
{
    line_init(&line);
}


void tearDown()
{
    line_del(&line);
}


static void test_correctly()
{   /* abra sch"wa"bra" "kadabra    foo"    b"ar */
    const char s[] = "abra sch\"wa\"bra\" \"kadabra\tfoo\"\tb\"ar";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_noerror, line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 0, 32, "");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("abra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("schwabra kadabra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("foo\tbar", item->word);
    TEST_ASSERT_NULL(item->next);
}


static void test_escape_sequence_correctly()
{   /* It\"s the \\correct\\ "\"escape\" sequence" */
    const char s[] =
        "It\\\"s the \\\\correct\\\\ \"\\\"escape\\\" sequence\"";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_noerror, line_split_mode_split);
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
}


static void test_empty_word_correctly()
{   /* abra"" "" ""\\ schw""abra """"  ""kadabra "" */
    const char s[] =
        "abra\"\" \"\" \"\"\\\\ schw\"\"abra \"\"\"\"\t\"\"kadabra \"\"";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_noerror, line_split_mode_split);
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
}


static void test_error_unmatched_quotes()
{   /* abra "schwabra kadabra"  "foo */
    const char s[] = "abra \"schwabra kadabra\"\t\"foo";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 0, 0, line_error_quotes, line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 4, 32, "foo\n");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("abra", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("schwabra kadabra", item->word);
    TEST_ASSERT_NULL(item->next);
}


static void test_error_unsupported_escape_sequence()
{   /* It's \"an\" unsupported\? escape sequence */
    const char s[] = "It's \\\"an\\\" unsupported\\? escape sequence";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 1, 0, line_error_escape, line_split_mode_split);
    TEST_ASSERT_LINE_WORD(line, 11, 16, "unsupported");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("It's", item->word);

    item = item->next;
    TEST_ASSERT_EQUAL_STRING("\"an\"", item->word);
    TEST_ASSERT_NULL(item->next);
}


static void test_check_to_save_first_error()
{   /* check "to save\? the first error */
    const char s[] = "check \"to save\\? the first error";

    line_from_chars(&line, s);
    TEST_ASSERT_LINE(line, 1, 1, 0, line_error_escape, line_split_mode_nosplit);
    TEST_ASSERT_LINE_WORD(line, 7, 16, "to save");

    item = line.wordlist.first;
    TEST_ASSERT_EQUAL_STRING("check", item->word);
    TEST_ASSERT_NULL(item->next);
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
