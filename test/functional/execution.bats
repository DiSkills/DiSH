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


@test "Enter an empty line" {
    run dish <<EOF

EOF
    assert_success
    assert_output "$w $w "
}


@test "Exit by Ctrl-D" {
    run dish <<EOF
EOF
    assert_success
    assert_output "$w "
}


@test "Print an error message for unmatched quotes" {
    run dish <<EOF
$PRINT_ARGS "Hello, ""World!
EOF
    assert_success
    assert_output "$w $msg_line_error_quotes""$newline""$w "
}


@test "Print an error message for an unsupported escape sequence" {
    run dish <<EOF
$PRINT_ARGS Hello \\World!
EOF
    assert_success
    assert_output "$w $msg_line_error_escape""$newline""$w "
}


@test "Check string processing" {
    run dish <<EOF
$PRINT_ARGS "Hello, World!" "" My name"" ""is \\"DiSkills\\" \\\\
EOF
    assert_success
    assert_output "$w [Hello, World!]
[]
[My]
[name]
[is]
[\"DiSkills\"]
[\\]
$w "
}


@test "Check cd" {
    run dish <<EOF
cd test
pwd
EOF
    assert_success
    assert_output "$w $w "`cd test && pwd`"$newline""$w "
}


@test "Check error handling for cd" {
    run dish <<EOF
cd baddir
EOF
    assert_success
    assert_output "$w baddir: No such file or directory""$newline""$w "
}


@test "Check error for cd by number of arguments" {
    run dish <<EOF
cd dir1 dir2
EOF
    assert_success
    assert_output "$w cd: too many arguments""$newline""$w "
}
