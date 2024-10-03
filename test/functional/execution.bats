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
    expected="$w "
    expected+="$w "

    run dish <<EOF

EOF
    assert_success
    assert_output "$expected"
}


@test "exit by Ctrl-D" {
    expected="$w "

    run dish <<EOF
EOF
    assert_success
    assert_output "$expected"
}


@test "cd: invalid number of arguments" {
    cmd="cd dir1 dir2"

    expected="$w "
    expected+="cd: too many arguments""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "cd: error from chdir" {
    cmd="cd baddir"

    expected="$w "
    expected+="baddir: No such file or directory""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "cd: works correctly" {
    cmd1="cd test"
    cmd2="pwd"

    expected="$w "
    expected+="$w "
    expected+=`$cmd1 && $cmd2`"$newline"
    expected+="$w "

    run dish <<<"$cmd1""$newline""$cmd2"
    assert_success
    assert_output "$expected"
}


@test "cd: home directory undefined" {
    unset HOME

    cmd="cd"

    expected="$w "
    expected+="HOME: unknown path to home directory""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "cd: home directory defined" {
    cmd1="cd"
    cmd2="pwd"

    expected="$w "
    expected+="$w "
    expected+=`$cmd1 && $cmd2`"$newline"
    expected+="$w "

    run dish <<<"$cmd1""$newline""$cmd2"
    assert_success
    assert_output "$expected"
}


@test "execute program: invalid program name" {
    cmd="badcmd"

    expected="$w "
    expected+="$cmd: No such file or directory""$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}


@test "execute program: works correctly" {
    cmd="ls -lh -a"

    expected="$w "
    expected+=`$cmd`"$newline"
    expected+="$w "

    run dish <<<"$cmd"
    assert_success
    assert_output "$expected"
}
