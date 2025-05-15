#!/bin/bash
find src include \( -name '*.cpp' -o -name '*.hpp' \) | xargs -n 1 clang-tidy -header-filter=.* -p build
