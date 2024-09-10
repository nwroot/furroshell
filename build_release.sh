#!/bin/sh
cc src/* -Iinclude -o out_rel -Wall -Werror -fwrapv -fno-strict-aliasing -DHOMEWORK -O3
