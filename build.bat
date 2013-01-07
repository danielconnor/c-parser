@echo off

cls

REM cl /ZI /Od main.cc token.cc scanner.cc parser.cc scope.cc buffer.cc ast.cc
g++ main.cc token.cc scanner.cc parser.cc scope.cc buffer.cc ast.cc