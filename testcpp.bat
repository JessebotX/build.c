@echo off

if not exist out ( mkdir out )

clang++ -std=c++11 -Wno-deprecated demo/build.c -o out/buildcpp.exe
