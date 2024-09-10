#!/bin/sh
cc src/* -Iinclude -o out -DDEBUG  -Wall -Werror -fwrapv -fno-strict-aliasing -fsanitize=undefined,address -DHOMEWORK -g
