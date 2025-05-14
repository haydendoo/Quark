#!/bin/bash
find src includes \( -name '*.cpp' -o -name '*.hpp' \) | xargs -n 1 clang-tidy -header-filter=.* -p build
