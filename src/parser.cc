#include "parser.h"

#define CHECK_ERROR  error);      \
  if (*error) return NULL;        \
  ((void)0


Parser::Parser(const char* filename)
{
  scanner = new Scanner(filename);
  scope = global = new GlobalScope();
  operators = new stack<Token::Value>();
  operands = new stack<Ast::Expression *>();

  operators->push(SENTINEL);
}

Parser::~Parser()
{
  // TODO: make sure these are empty
  //operators->clear();
  //operands->clear();

  delete scanner;
  delete scope;
  // delete global;
  delete operators;
  delete operands;
}

Token::Value Parser::peek()
{
  return scanner->peek();
}

/**
* Consume a token from the scanner and return its value
*/
Token::Value Parser::next()
{
  Token::Value tok = scanner->scan();
  return tok;
}

/**
* Consume a token and check whether it's the same
* as the expected token.
*/
void Parser::expect(Token::Value token, bool* error)
{
  Token::Value current = next();

  if(current != token)
  {
    cout << "unexpected token: " << Token::String(current) << "\n";
    *error = true;
  }
}

/**
* Create an new scope within the current scope.
*/
void Parser::openScope()
{
  scope = scope->createChildScope();
}

/**
* Close a by setting the current scope to be the current
* scope's parent.
*/
void Parser::closeScope()
{
  scope = scope->parent;
}

// start parsing from the global scope
void Parser::parse(Ast::List<Ast::Statement *> *statements)
{
  // a program is just a list of statements
  Ast::Statement* statement;
  bool error = false;

  while(peek() != Token::EOS && !error)
  {
    if((statement = parseStatement(&error)) != NULL && !error)
    {
      statements->push_back(statement);
    }
  }
}

// used for finding our way back to a valid
// statement after encountering an error
void Parser::syncStatement()
{
  next();
  while(1) {

    switch(peek()) {
      case Token::BREAK:
      case Token::CONST:
      case Token::CONTINUE:
      case Token::FOR:
      case Token::GOTO:
      case Token::IF:
      case Token::RETURN:
      case Token::SWITCH:
        return;
      default:
        break;
    }
    next();
  }
}

// union    struct  char     int    float    double  unsigned signed  long    long    short
// 0        0       0       0       0        0       0        0       0       0       0
#define _SHORT_     (1 << 0)
#define _LONG_      (1 << 1)
#define _LONGLONG_  (1 << 2)
#define _SIGNED_    (1 << 3)
#define _UNSIGNED_  (1 << 4)
#define _DOUBLE_    (1 << 5)
#define _FLOAT_     (1 << 6)
#define _INT_       (1 << 7)
#define _CHAR_      (1 << 8)
#define _STRUCT_    (1 << 9)
#define _UNION_     (1 << 10)
#define DATA_TYPE (_DOUBLE_ | _INT_ | _FLOAT_ | _CHAR_)
#define TYPE_MODIFIER (_SHORT_ | _LONG_ | _SIGNED_ | _UNSIGNED_)
#define IS_BOTH(A, T1, T2) ((A) & (T1) && (A) & (T2))

unsigned short Parser::parseType(bool* error)
{
  Token::Value tok;

  unsigned short type = 0;
  unsigned short specifier = 0;

  while(Token::IsTypeSpecifier(tok = peek()))
  {
    switch(tok)
    {
      case Token::SHORT:
        specifier = _SHORT_;
        break;
      case Token::LONG:
        if(type & _LONG_) {
          specifier = _LONGLONG_;
        }
        else specifier = _LONG_;
        break;
      case Token::SIGNED:
        specifier = _SIGNED_;
        break;
      case Token::UNSIGNED:
        specifier = _UNSIGNED_;
        break;
      case Token::FLOAT:
        specifier = _FLOAT_;
        break;
      case Token::DOUBLE:
        specifier = _DOUBLE_;
        break;
      case Token::INT:
        specifier = _INT_;
        break;
      case Token::CHAR:
        specifier = _CHAR_;
        break;
    }

    if(IS_BOTH(DATA_TYPE, type, specifier))
    {
      cout << "two or more data types in declaration specifiers\n";
    }
    if(IS_BOTH(type | specifier, _SHORT_, _LONG_))
    {
      cout << "cannot have both long and short in declaration specifiers\n";
    }
    if(IS_BOTH(type | specifier, TYPE_MODIFIER, _FLOAT_))
    {
      cout << "cannot have both float and " << Token::String(tok) << " in declaration specifiers\n";
    }
    if(IS_BOTH(type | specifier, _DOUBLE_, _SHORT_)) {
      cout << "cannot have both double and short in declaration specifiers\n";
    }
    if(IS_BOTH(type | specifier, _DOUBLE_, _SIGNED_)) {
      cout << "cannot have both double and signed in declaration specifiers\n";
    }
    if(IS_BOTH(type | specifier, _DOUBLE_, _UNSIGNED_)) {
      cout << "cannot have both double and unsigned in declaration specifiers\n";
    }

    // add the specifer to the type
    type |= specifier;

    next();
  }
  return type;
}

/**
* Because both variables and function declarations
* begin with type specifiers, we don't know which we're
* dealing with until the type has been parsed.
*/
Ast::Statement *Parser::parseFuncOrVar(bool* error)
{
  Ast::Statement *statement;
  int type = parseType(CHECK_ERROR);

  expect(Token::IDENTIFIER, CHECK_ERROR);

  if(peek() == Token::LPAREN) {
    statement = parseFunctionDecl(type, CHECK_ERROR);
  }
  else {
    statement = parseVariableDeclList(type, CHECK_ERROR);
  }

  return statement;
}

/**
* Default parse expression with all operators
*/
Ast::Expression *Parser::parseExpression(bool* error)
{
  return parseExpression(0, error);
}

/**
* Parse an expression, allowing only operators greater than
* a certain precedence to allow us to parse things like function
* invocation expressions which are seperated by commas.
*/
Ast::Expression *Parser::parseExpression(int prec, bool* error)
{
  Ast::Expression *result;
  Ast::Expression *expr;

  getOperand(CHECK_ERROR);

  if(Token::IsAssignmentOp(peek()) && !Token::IsUpdateOp(peek()))
  {
    Token::Value assign_op = next();
    Ast::Assignable *operand = (Ast::Assignable *)operands->top();
    operands->pop();
    expr = parseExpression(CHECK_ERROR);
    result = new Ast::AssignExpression(assign_op, operand, expr);
  }
  else {
    for(Token::Value tok = peek();
      ( Token::IsBinaryOp(tok) ||
        Token::IsCompareOp(tok)) && Token::Precedence(tok) >= prec;
      tok = peek())
    {
      pushOperator(tok);
      next();
      getOperand(CHECK_ERROR);
    }
    while(operators->top() != SENTINEL)
    {
      popOperator();
    }
    result = operands->top();
    operands->pop();
  }

  return result;
}

void *Parser::getOperand(bool* error)
{
  Token::Value tok = peek();
  if(tok == Token::IDENTIFIER)
  {
    expect(Token::IDENTIFIER, CHECK_ERROR);
    if(peek() == Token::LPAREN)
    {
      Ast::FunctionInvocation *expr = parseFunctionInvocation(CHECK_ERROR);
      operands->push(expr);
    }
    else
    {
      string name = scanner->currentVal();

      Ast::Type *type = scope->lookupVar(name);

      if(type)
      {
        Ast::Expression *expr = new Ast::Variable(name, type);

        while(peek() == Token::DOT)
        {
          expect(Token::DOT, CHECK_ERROR);
          expect(Token::IDENTIFIER, CHECK_ERROR);
          expr = new Ast::MemberExpression(expr, scanner->currentVal());
        }

        // We need to check for a postfix operation here.
        if(Token::IsUpdateOp(peek()))
        {
          expr = new Ast::UpdateExpression(next(), expr, false);
        }

        operands->push(expr);
      }
      else
      {
        cout << "This variable is not defined in the current scope: " << name << "\n";
        *error = true;
      }
    }
  }
  else if(Token::IsIdentifierOrLiteral(tok))
  {
    next();

    if(tok == Token::NUMBER)
    {
      operands->push(new Ast::NumberLiteral(scanner->currentVal(), scanner->currentNumber()));
    }
    else
    {
      operands->push(new Ast::Literal(scanner->currentVal()));
    }
  }
  else if(tok == Token::LPAREN)
  {
    operators->push(SENTINEL);
    expect(Token::LPAREN, CHECK_ERROR);

    Ast::Expression *expr = parseExpression(CHECK_ERROR);
    operands->push(expr);

    expect(Token::RPAREN, CHECK_ERROR);
    operators->pop();
  }
  else if(Token::IsUnaryOp(tok))
  {
    next();
    getOperand(CHECK_ERROR);
    Ast::Expression *expr = operands->top();
    operands->pop();

    operands->push(new Ast::UnaryExpression(tok, expr));
  }
  else if(Token::IsUpdateOp(tok))
  {
    next();
    getOperand(CHECK_ERROR);
    Ast::Expression *expr = operands->top();
    operands->pop();

    operands->push(new Ast::UpdateExpression(tok, expr, true));
  }
  else
  {
    cout << "error parsing operand" << Token::String(tok) << "\n";
    *error = true;
  }

  return NULL;
}

void Parser::popOperator()
{
  Token::Value tok = operators->top();
  operators->pop();

  if(Token::IsBinaryOp(tok) || Token::IsCompareOp(tok))
  {
    Ast::Expression *right = operands->top();
    operands->pop();
    Ast::Expression *left = operands->top();
    operands->pop();

    operands->push(new Ast::BinaryExpression(tok, left, right));
  }
  else if(Token::IsUpdateOp(tok))
  {
    Ast::Expression *top = operands->top();
    operands->pop();
    operands->push(new Ast::UpdateExpression(tok, top, true));
  }
  else
  {
    Ast::Expression *top = operands->top();
    operands->pop();
    operands->push(new Ast::UnaryExpression(tok, top));
  }

}
void Parser::pushOperator(Token::Value op)
{
  Token::Value top = operators->top();

  bool unary = Token::IsUnaryOp(op) || Token::IsUpdateOp(op);

  while(Token::Precedence(top) > Token::Precedence(op) && !(unary && Token::IsBinaryOp(top)))
  {
    popOperator();
    top = operators->top();
  }
  operators->push(op);
}

/**
* Parse an expression statement which is made up of an
* expression and a semicolon.
*/
Ast::ExpressionStatement *Parser::parseExpressionStatement(bool* error)
{
  Ast::Expression *expression = NULL;

  // can have an empty statement
  if(peek() != Token::SEMICOLON) {
    expression = parseExpression(CHECK_ERROR);
  }
  expect(Token::SEMICOLON, CHECK_ERROR);
  return new Ast::ExpressionStatement(
      expression
    );
}

/**
* Decide what sort of statement is coming next and
* select a function to parse it.
*/
Ast::Statement *Parser::parseStatement(bool *error)
{
  Ast::Statement *statement = NULL;

  Token::Value tok = peek();

  if(tok == Token::IDENTIFIER || tok == Token::NUMBER ||
       Token::IsUpdateOp(tok) || Token::IsUnaryOp(tok))
  {
    statement = parseExpressionStatement(error);
  }
  else if(Token::IsStorageSpecifier(tok) || Token::IsTypeSpecifier(tok))
  {
    statement = parseFuncOrVar(error);
  }
  else
  {
    switch(peek())
    {
      case Token::RETURN:
        statement = parseReturn(error);
        break;
      case Token::LBRACE:
        statement = parseBlock(error);
        break;
      case Token::FOR:
        statement = parseFor(error);
        break;
      case Token::WHILE:
        statement = parseWhile(error);
        break;
      case Token::IF:
        statement = parseIf(error);
        break;
      case Token::STRUCT:
        statement = parseStruct(error);
        break;
      case Token::SWITCH:
        statement = parseSwitch(error);
        break;
      case Token::SEMICOLON:
        statement = parseExpressionStatement(error);
        break;
      case Token::BREAK:
        statement = new Ast::BreakStatement();
        expect(Token::BREAK, error);
        expect(Token::SEMICOLON, error);
        break;
      case Token::CONTINUE:
        statement = new Ast::ContinueStatement();
        expect(Token::CONTINUE, error);
        expect(Token::SEMICOLON, error);
        break;
      case Token::ILLEGAL:
      default:
        // If we found a statement we don't recognize, find one
        // that we do recognize.
        cout << "got unrecognized statement\n";
        *error = true;
        break;
    }
  }

  return statement;
}

/**
*
*/
Ast::Statement *Parser::parseFunctionDecl(unsigned short type, bool* error)
{
  string name = scanner->currentVal();
  Ast::FunctionPrototype *proto = global->lookupFunc(name);
  Ast::List<Ast::Argument *> *arguments = proto ? proto->arguments : NULL;
  Ast::BlockStatement *body = NULL;
  Ast::Type *returnType = proto ? proto->returnType : new Ast::Type(type);

  expect(Token::LPAREN, CHECK_ERROR);
  arguments = parseArgumentList(arguments, CHECK_ERROR);
  expect(Token::RPAREN, CHECK_ERROR);

  if(!proto)
  {
    proto = new Ast::FunctionPrototype(
        returnType,
        name,
        arguments
      );
    global->declareFunc(name, proto);
  }

  // check if it's a prototype or definition.
  if(peek() == Token::LBRACE)
  {
    openScope();

    // we must define each of the arguments in the current
    // scope
    for(Ast::List<Ast::Argument *>::iterator it = arguments->begin(); it != arguments->end(); ++it)
    {
      scope->declareVar((*it)->name, (*it)->type);
    }

    body = parseBlock(false, CHECK_ERROR);

    return new Ast::FunctionDeclaration(
      proto,
      body
    );
  }
  else {
    expect(Token::SEMICOLON, CHECK_ERROR);
    return proto;
  }

}

Ast::Argument *Parser::parseArgument(Ast::Argument *argument, bool *error)
{
  unsigned short type = parseType(CHECK_ERROR);
  string name = "";

  if(peek() == Token::IDENTIFIER)
  {
    expect(Token::IDENTIFIER, CHECK_ERROR);
    name = scanner->currentVal();
  }

  if(argument)
  {
    if(argument->type->type != type)
    {
      cout << "type mismatch\n";
      *error = true;
    }
    else
    {
      argument->name = name;
    }
  }
  else
  {
    argument = new Ast::Argument(
        new Ast::Type(type),
        name
      );
  }

  return argument;
}

Ast::List<Ast::Argument *> *Parser::parseArgumentList(Ast::List<Ast::Argument *> *arguments, bool* error)
{
  if(!arguments) return parseArgumentList(CHECK_ERROR);
  Ast::List<Ast::Argument *>::iterator it = arguments->begin();

  if(peek() != Token::RPAREN)
  {
    if(it <= arguments->end()) {
      parseArgument(*it++, CHECK_ERROR);

      while(peek() == Token::COMMA && it < arguments->end())
      {
        expect(Token::COMMA, CHECK_ERROR);
        parseArgument(*it, CHECK_ERROR);
        it++;
      }
    }
    else {
      *error = true;
      cout << "type mismatch\n";
    }
  }

  return arguments;
}

Ast::List<Ast::Argument *> *Parser::parseArgumentList(bool* error)
{
  Ast::List<Ast::Argument *> *arguments = new Ast::List<Ast::Argument *>();

  if(peek() != Token::RPAREN)
  {
    Ast::Argument *argument = parseArgument(NULL, CHECK_ERROR);
    arguments->push_back(argument);

    while(peek() == Token::COMMA)
    {
      expect(Token::COMMA, CHECK_ERROR);
      argument = parseArgument(NULL, CHECK_ERROR);
      arguments->push_back(argument);
    }
  }

  return arguments;
}


Ast::FunctionInvocation *Parser::parseFunctionInvocation(bool *error)
{
  // name has already been scanned at this stage
  string name = scanner->currentVal();
  Ast::List<Ast::Expression *> *arguments = new Ast::List<Ast::Expression *>();
  Ast::Expression *argument = NULL;

  expect(Token::LPAREN, CHECK_ERROR);

  // we have at least one argument
  if(peek() != Token::RPAREN) {

    argument = parseExpression(4, CHECK_ERROR);
    arguments->push_back(argument);

    while(peek() == Token::COMMA) {
      expect(Token::COMMA, CHECK_ERROR);
      argument = parseExpression(4, CHECK_ERROR);
      arguments->push_back(argument);
    }
  }

  Ast::FunctionPrototype *function = global->lookupFunc(name);

  if(!function)
  {
    cout << "this function has not been declared: " << name << "\n";
  }
  else
  {
    if(function->arguments->size() != arguments->size())
    {
      cout << "wrong number of arguments\n";
    }
  }


  expect(Token::RPAREN, CHECK_ERROR);

  return new Ast::FunctionInvocation(
      function,
      arguments
    );
};

Ast::Declaration *Parser::parseVariableDeclList(unsigned short type, bool *error)
{
  Ast::List<Ast::Declarator *> *declarations = new Ast::List<Ast::Declarator *>();
  Ast::Declarator *declaration = parseVariableDecl(true, type, CHECK_ERROR);
  // always has at least one variable declared
  declarations->push_back(declaration);


  while(peek() == Token::COMMA)
  {
    expect(Token::COMMA, CHECK_ERROR);
    declaration = parseVariableDecl(false, type, CHECK_ERROR);
    declarations->push_back(declaration);
  }

  expect(Token::SEMICOLON, CHECK_ERROR);

  return new Ast::Declaration(
      new Ast::Type(type),
      declarations
    );
}

Ast::Declarator *Parser::parseVariableDecl(bool hasName, short unsigned type, bool* error)
{
  string name;
  Ast::Expression *init = NULL;

  if(!hasName) {
    expect(Token::IDENTIFIER, CHECK_ERROR);
  }

  name = scanner->currentVal();

  scope->declareVar(name, new Ast::Type(type));

  if(peek() == Token::ASSIGN) {
    expect(Token::ASSIGN, CHECK_ERROR);
    init = parseExpression(4, CHECK_ERROR);
  }
  return new Ast::Declarator(
      name,
      init
    );
}

Ast::ReturnStatement *Parser::parseReturn(bool* error)
{
  // return keyword
  expect(Token::RETURN, CHECK_ERROR);

  // followed by expression
  Ast::Expression *expression = parseExpression(CHECK_ERROR);

  // followed by semicolon
  expect(Token::SEMICOLON, CHECK_ERROR);

  return new Ast::ReturnStatement(
      expression
    );
}

Ast::IfStatement *Parser::parseIf(bool* error)
{
  Ast::Expression *condition = NULL;
  Ast::Statement *body = NULL;
  Ast::Statement *elseBody = NULL;

  expect(Token::IF, CHECK_ERROR);
  expect(Token::LPAREN, CHECK_ERROR);

  condition = parseExpression(CHECK_ERROR);

  expect(Token::RPAREN, CHECK_ERROR);

  body = parseStatement(CHECK_ERROR);

  if(peek() == Token::ELSE) {
    expect(Token::ELSE, CHECK_ERROR);
    elseBody = parseStatement(CHECK_ERROR);
  }

  return new Ast::IfStatement(
      condition,
      body,
      elseBody
    );
}

Ast::SwitchStatement *Parser::parseSwitch(bool* error)
{
  Ast::Expression *variable = NULL;

  expect(Token::SWITCH, CHECK_ERROR);

  expect(Token::LPAREN, CHECK_ERROR);

  variable = parseExpression(CHECK_ERROR);

  expect(Token::RPAREN, CHECK_ERROR);

  expect(Token::LBRACE, CHECK_ERROR);

  // TODO: parse the body of the switch statement

  expect(Token::RBRACE, CHECK_ERROR);

  return new Ast::SwitchStatement(
      variable
    );
}


Ast::ForStatement *Parser::parseFor(bool* error)
{
  Ast::Expression *init = NULL;
  Ast::Expression *condition = NULL;
  Ast::Expression *loop = NULL;
  Ast::Statement *body = NULL;

  expect(Token::FOR, CHECK_ERROR);
  expect(Token::LPAREN, CHECK_ERROR);

  // expressions are optional
  if(peek() != Token::SEMICOLON) {
    // TODO: allow for declatations in init
    init = parseExpression(CHECK_ERROR);
  }
  expect(Token::SEMICOLON, CHECK_ERROR);

  // expressions are optional
  if(peek() != Token::SEMICOLON) {
    condition = parseExpression(CHECK_ERROR);
  }
  expect(Token::SEMICOLON, CHECK_ERROR);

  // expressions are optional
  if(peek() != Token::RPAREN) {
    loop = parseExpression(CHECK_ERROR);
  }
  expect(Token::RPAREN, CHECK_ERROR);

  // open new "wrapper" scope to contain
  // variables defined in the intialisation
  // of the for loop
  openScope();

  // parse any statement including a block
  // statement
  body = parseStatement(CHECK_ERROR);

  // close the "wrapper" scope
  closeScope();

  return new Ast::ForStatement(
      init,
      condition,
      loop,
      body
    );
}

Ast::WhileStatement *Parser::parseWhile(bool* error)
{
  Ast::Expression *condition = NULL;
  Ast::Statement *body = NULL;

  expect(Token::WHILE, CHECK_ERROR);
  expect(Token::LPAREN, CHECK_ERROR);

  condition = parseExpression(CHECK_ERROR);

  expect(Token::RPAREN, CHECK_ERROR);

  body = parseStatement(CHECK_ERROR);

  return new Ast::WhileStatement(
      condition,
      body
    );
}

Ast::StructStatement *Parser::parseStruct(bool* error)
{
  Ast::List<Ast::Declaration *> *members = new Ast::List<Ast::Declaration *>();
  Ast::Declaration *decl;
  string name;

  expect(Token::STRUCT, CHECK_ERROR);

  expect(Token::IDENTIFIER, CHECK_ERROR);
  name = scanner->currentVal();

  // definition of struct
  if(peek() == Token::LBRACE)
  {
    expect(Token::LBRACE, CHECK_ERROR);

    while(peek() != Token::RBRACE)
    {
      decl = (Ast::Declaration *)parseFuncOrVar(CHECK_ERROR);
      members->push_back(decl);
    }

    expect(Token::RBRACE, CHECK_ERROR);
  }

  // declaring instance of struct
  if(peek() == Token::IDENTIFIER)
  {
    expect(Token::IDENTIFIER, CHECK_ERROR);
  }

  expect(Token::SEMICOLON, CHECK_ERROR);

  return new Ast::StructStatement(
      name,
      members
    );
}

Ast::List<Ast::Expression *> *Parser::parseArrayExpression(bool *error)
{
  Ast::List<Ast::Expression *> *elements = new Ast::List<Ast::Expression *>();
  Ast::Expression *expr;

  expect(Token::LBRACE, CHECK_ERROR);

  while(peek() != Token::RBRACE)
  {
    expr = parseExpression(4, CHECK_ERROR);

    elements->push_back(expr);
  }

  expect(Token::RBRACE, CHECK_ERROR);

  return elements;
}

Ast::BlockStatement *Parser::parseBlock(bool* error)
{
  // open a scope for every block by default
  return parseBlock(true, error);
}

// overloaded to allow defining whether or not
// a new scope should be opened. e.g the scope is
// opened early in a function declaration.
Ast::BlockStatement *Parser::parseBlock(bool openNewScope, bool* error)
{
  Ast::List<Ast::Statement *> *statements = new Ast::List<Ast::Statement *>();
  Ast::Statement* statement;

  if(openNewScope)
  {
    openScope();
  }

  expect(Token::LBRACE, CHECK_ERROR);

  while(peek() != Token::RBRACE && peek() != Token::EOS)
  {
    statement = parseStatement(CHECK_ERROR);
    statements->push_back(statement);
  }

  // the scope will always be closed at the end of the
  expect(Token::RBRACE, error);

  closeScope();

  return new Ast::BlockStatement(
      statements
    );
}
