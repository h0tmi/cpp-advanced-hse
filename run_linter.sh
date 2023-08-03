#!/bin/bash

# Run from build directory

if [ "$#" -lt 1 ]; then
    echo "Usage: $0 task-name"
    exit 1
fi

TASK_PATH=../tasks/$1
CLANG_PATH=../run-clang-format.py
CLANG_TIDY=clang-tidy-14

if [ ! -f compile_commands.json ]; then
    echo "Run this script from the build directory"
    exit 1
fi

if [ "$#" -ge 2 ]; then
    TASK_PATH=../../tasks/$1
    CLANG_PATH=../../run-clang-format.py
    CLANG_TIDY="hse-clang-tidy --extra-arg=-I/usr/lib/clang/11/include/"
fi


set -e
set -u
set -o pipefail
set -x

# clang-format all sources
jq -r '.allow_change | if type=="array" then .[] else . end' --raw-output $TASK_PATH/.tester.json | sed "s|^|$TASK_PATH\/|g" | egrep '\.c$|\.cpp$|\.h$|\.hpp$' | xargs -t -I@ sh -c "ls @" | xargs -t $CLANG_PATH

# clang-tidy all TUs
jq -r '.allow_change | if type=="array" then .[] else . end' --raw-output $TASK_PATH/.tester.json | sed "s|^|$TASK_PATH\/|g" | egrep '\.c$|\.cpp$|\.h$|\.hpp$' | xargs -t -I@ sh -c "ls @" | xargs -t $CLANG_TIDY

# additional checks
if [ "$#" -eq 3 ]; then
    jq -r '.allow_change | if type=="array" then .[] else . end' --raw-output $TASK_PATH/.tester.json | sed "s|^|$TASK_PATH\/|g" | egrep '\.c$|\.cpp$|\.h$|\.hpp$' | xargs -t -I@ sh -c "ls @" | xargs -t $CLANG_TIDY --config="$3"
fi

$CLANG_TIDY $TASK_PATH/*.cpp
