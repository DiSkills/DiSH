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


@test "ignore empty line" {
    run dish <<EOF

EOF
    assert_success
    assert_output "$w $w "
}


@test "exit by Ctrl-D" {
    run dish <<EOF
EOF
    assert_success
    assert_output "$w "
}


@test "string processing: unmatched quotes error" {
    run dish <<EOF
$PRINT_ARGS "Hello, ""World!
EOF
    assert_success
    assert_output "$w $msg_line_error_quotes""$newline""$w "
}


@test "string processing: escape sequence error" {
    run dish <<EOF
$PRINT_ARGS Hello \\World!
EOF
    assert_success
    assert_output "$w $msg_line_error_escape""$newline""$w "
}


@test "string processing: works correctly" {
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


@test "cd: invalid number of arguments" {
    run dish <<EOF
cd dir1 dir2
EOF
    assert_success
    assert_output "$w cd: too many arguments""$newline""$w "
}


@test "cd: error from chdir" {
    run dish <<EOF
cd baddir
EOF
    assert_success
    assert_output "$w baddir: No such file or directory""$newline""$w "
}


@test "cd: works correctly" {
    run dish <<EOF
cd test
pwd
EOF
    assert_success
    assert_output "$w $w "`cd test && pwd`"$newline""$w "
}
