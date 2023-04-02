#!/usr/bin/bash
clang_format_bin=$1
compile_commands_dir=$2
current_path=$(pwd)
files=$(find ${current_path}/../src/*.cpp)
files="${files} $(find ${current_path}/../test/*.cpp)"

for file in ${files}; do
    ${clang_format_bin} --format-style=file -p ${compile_commands_dir} ${file} &
done
wait
