#!/bin/bash

set -ex

S=$(pwd)
B=$S/../../../build/RelWithDebInfo

$B/check-names $S/tests/no-dict/*.cpp $S/check_names.cpp -p $B >/tmp/no-dict-result.txt 2>/dev/null
$B/check-names $S/tests/dict/*.cpp -p $B -dict $S/tests/dict/dict.txt >/tmp/dict-result.txt 2>/dev/null

diff $S/tests/no-dict/result.txt /tmp/no-dict-result.txt
diff $S/tests/dict/result.txt /tmp/dict-result.txt
