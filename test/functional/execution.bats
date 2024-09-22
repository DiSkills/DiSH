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
