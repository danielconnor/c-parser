#ifndef _PARSER
#define _PARSER

#include <stack>
#include <stdint.h>

#include "scanner.h"
#include "scope.h"
#include "ast.h"

using namespace std;

#define SENTINEL Token::EOS

class Parser {

public:
  Scanner scanner;
  GlobalScope* global;
  Scope* scope;

  string lastError;

  Scope* objectScope;

  stack<Token::Value>* operators;
  stack<Ast::Expression*>* operands;

  Parser(istream&);
  ~Parser();

  void addError(uint32_t, uint32_t, std::string);

  Token::Value next();
  Token::Value peek();
  Token::Value current();

  void expect(Token::Value, bool*);

  void openScope();
  void closeScope();

  void getOperand(bool*);
  void popOperator();
  void pushOperator(Token::Value);

  void syncStatement();

  void parse(Ast::List<Ast::Statement*>* );
  void consumeNonStandard(bool*);

  Ast::Type* parseType(bool, bool*);
  inline bool checkPointer();

  Ast::Statement* parseFuncOrVar(bool*);
  Ast::Statement* parseFunctionDecl(Ast::Type*, bool*);
  Ast::Argument* parseArgument(bool*);
  Ast::ArgumentList* parseArgumentList(bool*);

  Ast::Declaration* parseVariableDeclList(Ast::Type*, Ast::Type*, bool*);
  Ast::Declarator* parseVariableDecl(Ast::Type*, bool, bool*);

  Ast::FunctionInvocation* parseFunctionInvocation(bool*);

  Ast::Variable* parseVariableDef(Ast::Type*, bool, bool*);
  Ast::Type* parseArraySize(Ast::Type*, bool*);
  Ast::List<Ast::Variable*>* parseStructDeclList(bool*);

  Ast::BlockStatement* parseBlock(bool*);
  Ast::BlockStatement* parseBlock(bool, bool*);
  Ast::Statement* parseStatement(bool*);

  Ast::Typedef* parseTypedef(bool* );

  Ast::ReturnStatement* parseReturn(bool*);

  Ast::Expression* parseExpression(int prec, bool*);
  Ast::Expression* parseExpression(bool*);
  Ast::ExpressionStatement* parseExpressionStatement(bool*);

  Ast::IfStatement* parseIf(bool*);
  Ast::SwitchStatement* parseSwitch(bool*);
  Ast::ForStatement* parseFor(bool*);
  Ast::WhileStatement* parseWhile(bool*);

  Ast::Struct* parseStruct(bool*);
  Ast::Struct* parseUnion(bool*);
  Ast::EnumStatement* parseEnum(bool*);

  Ast::List<Ast::Expression* >* parseArrayExpression(bool* );

};

#endif
