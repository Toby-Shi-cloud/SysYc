# SysYc

## Overview

SysY is a subset of C language, which is designed for the compiler course in many universities in China.

This project is a compiler for SysY language, which is implemented in SysY itself.

## SysY

The grammar of SysY this compiler supports is defined in [`SysY.g4`](SysY.g4).

SysYc supports the following features:
- Basic data types: `int` and `char`
- Arrays, arrays as function parameters
- Control structures: `if`, `while`, `for`, `break`, `continue`
- `&&` and `||` operators with short-circuit evaluation
- Library functions: `getint`, `getch`, `putint`, `putch`, `printf` 

> Note: `printf` will only support `%d`, `%s` and `%c` format specifiers.

> Note: Preprocessor directives (lines start with a `#`) will be ignored.
