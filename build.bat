@echo off

cls

REM cl /ZI /Od main.cc token.cc scanner.cc parser.cc scope.cc buffer.cc ast.cc
g++ -o src/parser.exe src/main.cc src/token.cc src/scanner.cc src/parser.cc src/scope.cc src/buffer.cc src/ast.cc