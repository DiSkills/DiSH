#define TEST_ASSERT_LINE_WORD(LINE, LEN, SIZE, STR) \
    TEST_ASSERT_EQUAL_INT(LEN, LINE.current_word.len); \
    TEST_ASSERT_EQUAL_INT(SIZE, LINE.current_word.size); \
    TEST_ASSERT_EQUAL_STRING(STR, LINE.current_word.data);

#define TEST_ASSERT_LINE_WORD_IS_DEFAULT(LINE) \
    TEST_ASSERT_LINE_WORD(LINE, 0, 16, "");

#define TEST_ASSERT_LINE_WORDLIST_IS_EMPTY(LINE) \
    TEST_ASSERT_NULL(LINE.wordlist.first); \
    TEST_ASSERT_NULL(LINE.wordlist.last);

#define TEST_ASSERT_LINE(LINE, IS_FINISHED, IS_ESCAPED, \
        WORD_IS_EMPTY, ERROR, MODE) \
    TEST_ASSERT_EQUAL_INT(IS_FINISHED, LINE.is_finished); \
    TEST_ASSERT_EQUAL_INT(IS_ESCAPED, LINE.is_escaped); \
    TEST_ASSERT_EQUAL_INT(WORD_IS_EMPTY, LINE.word_is_empty); \
    TEST_ASSERT_EQUAL_INT(ERROR, LINE.errno); \
    TEST_ASSERT_EQUAL_INT(MODE, LINE.mode);

#define FILL_LINE(LINE, IS_FINISHED, IS_ESCAPED, \
        WORD_IS_EMPTY, ERROR, MODE) \
    LINE.is_finished = IS_FINISHED; \
    LINE.is_escaped = IS_ESCAPED; \
    LINE.word_is_empty = WORD_IS_EMPTY; \
    LINE.errno = ERROR; \
    LINE.mode = MODE;
