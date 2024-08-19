#define assert_word_is_default(LINE) \
    assert(LINE.current_word.len == 0); \
    assert(LINE.current_word.size == 16); \
    assert(LINE.current_word.data != NULL); \
    assert(strlen(LINE.current_word.data) == 0);

#define assert_wordlist_is_empty(LINE) \
    assert(LINE.wordlist.first == NULL); \
    assert(LINE.wordlist.last == NULL);

#define assert_line(LINE, IS_FINISHED, IS_ESCAPED, \
        WORD_IS_EMPTY, ERROR, MODE) \
    assert(LINE.is_finished == (IS_FINISHED)); \
    assert(LINE.is_escaped == (IS_ESCAPED)); \
    assert(LINE.word_is_empty == (WORD_IS_EMPTY)); \
    assert(LINE.errno == ERROR); \
    assert(LINE.mode == MODE);
