#ifndef _PARSER
#define _PARSER

#include <stack>

#include "scanner.h"
#include "scope.h"
#include "ast.h"

using namespace std;

#define SENTINEL Token::EOS

class Parser {

public:
  Scanner *scanner;
  GlobalScope *global;
  Scope *scope;

  string lastError;

  Scope *objectScope;

  stack<Token::Value> *operators;
  stack<Ast::Expression *> *operands;

  Parser(const char *);
  ~Parser();

  Token::Value next();
  Token::Value peek();
  Token::Value current();

  void expect(Token::Value, bool*);

  void openScope();
  void closeScope();

  void *getOperand(bool*);
  void popOperator();
  void pushOperator(Token::Value);

  void syncStatement();

  Ast::List<Ast::Statement *> *parse();

  unsigned short parseType(bool*);
  Ast::Statement *parseFuncOrVar(bool*);
  Ast::Statement *parseFunctionDecl(unsigned short, bool*);
  Ast::Argument *parseArgument(Ast::Argument *, bool *);
  Ast::List<Ast::Argument *> *parseArgumentList(bool*);
  Ast::List<Ast::Argument *> *parseArgumentList(Ast::List<Ast::Argument *> *, bool*);

  Ast::Declaration *parseVariableDeclList(unsigned short, bool*);
  Ast::Declarator *parseVariableDecl(bool, unsigned short, bool*);
  Ast::FunctionInvocation *parseFunctionInvocation(bool*);

  Ast::BlockStatement *parseBlock(bool*);
  Ast::BlockStatement *parseBlock(bool, bool*);
  Ast::Statement *parseStatement(bool*);

  Ast::ReturnStatement *parseReturn(bool*);

  Ast::Expression *parseExpression(int prec, bool*);
  Ast::Expression *parseExpression(bool*);
  Ast::ExpressionStatement *parseExpressionStatement(bool*);

  Ast::IfStatement *parseIf(bool*);
  Ast::SwitchStatement *parseSwitch(bool*);
  Ast::ForStatement *parseFor(bool*);
  Ast::WhileStatement *parseWhile(bool*);

  Ast::StructStatement *parseStruct(bool*);
  Ast::EnumStatement *parseEnum(bool*);


  Ast::List<Ast::Expression *> *parseArrayExpression(bool *);

};

#endif
