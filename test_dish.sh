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
    split_mode
    nosplit_mode
    many
    error_unmatched_quotes
END
