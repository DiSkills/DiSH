#!/usr/bin/env bats

w='>'
newline=$'\n'

msg_line_error_quotes="Error: unmatched quotes"
msg_line_error_escape="Error: unsupported escape sequence"


setup() {
    load './test_helper/bats-support/load'
    load './test_helper/bats-assert/load'

    dish() {
        stdbuf -i0 -oL -eL $TARGET
    }
}


@test "string processing: unmatched quotes error" {
    cmd="$PRINT_ARGS \"Hello, \"\"World!"

    expected="$w "
    expected+="$msg_line_error_quotes""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "string processing: escape sequence error" {
    cmd="$PRINT_ARGS Hello \\World!"

    expected="$w "
    expected+="$msg_line_error_escape""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "string processing: works correctly" {
    cmd="$PRINT_ARGS \"Hello, World!\" \"\" "
    cmd+="My name\"\" \"\"is \\\"DiSkills\\\" \\\\"

    expected="$w "
    expected+="[Hello, World!]""$newline"
    expected+="[]""$newline"
    expected+="[My]""$newline"
    expected+="[name]""$newline"
    expected+="[is]""$newline"
    expected+="[\"DiSkills\"]""$newline"
    expected+="[\\]""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}
