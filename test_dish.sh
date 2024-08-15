#!/bin/bash

filename=$1
test_dir=$2
input_dir=$test_dir/input
output_dir=$test_dir/output

while read test_name; do
    result=`stdbuf -i0 -oL -eL ./$filename < $input_dir/$test_name.txt |& cat`
    expected=`cat $output_dir/$test_name.txt`

    if [ x"$expected" != x"$result" ]; then
        echo "TEST $test_name FAILED: expected \"$expected\", got \"$result\""
    fi
done <<END
    test1_only_1_cmd
    test2_only_1_cmd
    test3_only_1_cmd
    test_many_cmds
END
