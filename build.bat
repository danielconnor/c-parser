clang++ -g src/main.cc src/token.cc src/scanner.cc src/parser.cc src/scope.cc src/buffer.cc src/ast.cc `llvm-config --cppflags --ldflags --libs core` -o parser
