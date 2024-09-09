#!/bin/sh
cc src/* -Iinclude -o out -DDEBUG -O3 -Wall -Werror -fwrapv -fno-strict-aliasing -fsanitize=undefined,address -DHOMEWORK -g
