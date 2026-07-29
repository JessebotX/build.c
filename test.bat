@echo off

if not exist out ( mkdir out )

clang -std=c89 -O0 -g3 -ffreestanding -pedantic -Wall -Wextra -Weverything -Werror -Wno-unused-function -Wno-unused-parameter -Wno-unused-variable -Wno-unsafe-buffer-usage -Wno-sign-conversion -Wno-undef demo/build.c -o out/build.exe
