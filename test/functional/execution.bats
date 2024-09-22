#!/usr/bin/env bats

w='>'
newline=$'\n'


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


@test "execute program: invalid program name" {
    run dish <<EOF
badname
EOF
    assert_success
    assert_output "$w badname: No such file or directory""$newline""$w "
}


@test "execute program: works correctly" {
    run dish <<EOF
ls -lh -a
EOF
    assert_success
    assert_output "$w ""`ls -lh -a`""$newline""$w "
}
