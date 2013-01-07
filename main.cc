#include "parser.h"

int main()
{

  Parser p("parser_test.c");

  Ast::List<Ast::Statement *> *ast = p.parse();

  ast->printVar("ast.js", "ast");

  delete ast;

  return 0;
}
