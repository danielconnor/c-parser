#include "parser.h"

#define CHECK_ERROR error);  \
  if (*error) return;        \
  ((void)0

#define CHECK_ERROR_NULL  error);  \
  if (*error) return NULL;         \
  ((void)0


Parser::Parser(istream& input) : scanner(input)
{
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
  delete scope;
  // delete global;
  delete operators;
  delete operands;
}

Token::Value Parser::peek()
{
  return scanner.peek();
}

/**
* Consume a token from the scanner and return its value
*/
Token::Value Parser::next()
{
  Token::Value tok = scanner.scan();
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
    cerr << "line:" << scanner._current->line << " col: " << scanner._current->col << " - unexpected token: " << Token::String(current) << "\n";
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
  scope = scope->closeScope();
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

Ast::Type* Parser::parseType(bool consumePointer, bool* error)
{
  Ast::Type* type;
  Token::Value tok;
  unsigned short typeField = 0;
  unsigned short specifier = 0;

  // Look for a type defined with typedef
  Ast::Node* node;

  while(Token::IsStorageSpecifier(tok = peek())) {
      // TODO: actually handle the storage specifier
      next();
  }

  while(Token::IsNonStandard(peek())) {
    consumeNonStandard(CHECK_ERROR_NULL);
  }

  if(global->lookupIdentifier(scanner.peekVal(), node) == Ast::TYPEDEF) {
    next();
    type = ((Ast::Typedef*)node)->type;
  }
  else {
    if(tok == Token::STRUCT) {
      type = parseStruct(CHECK_ERROR_NULL);
    }
    else if(tok == Token::UNION) {
      cerr << "union not yet implemented";
      //type = parseUnion();
    }
    else {
      while(Token::IsTypeSpecifier(tok = peek()))
      {
        switch(tok)
        {
          case Token::SHORT:
            specifier = _SHORT_;
            break;
          case Token::LONG:
            if(typeField & _LONG_) {
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

        if(IS_BOTH(DATA_TYPE, typeField, specifier))
        {
          cout << "two or more data types in declaration specifiers\n";
        }
        if(IS_BOTH(typeField | specifier, _SHORT_, _LONG_))
        {
          cout << "cannot have both long and short in declaration specifiers\n";
        }
        if(IS_BOTH(typeField | specifier, TYPE_MODIFIER, _FLOAT_))
        {
          cout << "cannot have both float and " << Token::String(tok) << " in declaration specifiers\n";
        }
        if(IS_BOTH(typeField | specifier, _DOUBLE_, _SHORT_)) {
          cout << "cannot have both double and short in declaration specifiers\n";
        }
        if(IS_BOTH(typeField | specifier, _DOUBLE_, _SIGNED_)) {
          cout << "cannot have both double and signed in declaration specifiers\n";
        }
        if(IS_BOTH(typeField | specifier, _DOUBLE_, _UNSIGNED_)) {
          cout << "cannot have both double and unsigned in declaration specifiers\n";
        }
        // add the specifer to the type
        typeField |= specifier;

        next();

        while(Token::IsNonStandard(peek())) {
          consumeNonStandard(CHECK_ERROR_NULL);
        }

      }
      type = new Ast::FundamentalType(typeField);
    }
  }

  if(consumePointer) {
    while(checkPointer())
    {
      type = new Ast::Pointer(type);
    }
  }

  return type;
}


/**
 * Consume non-standard tokens and discard them.
 * @param error Whether or not an error has occured.
 */
void Parser::consumeNonStandard(bool *error)
{
  switch(peek()) {
    // add other non-standard tokens to be skipped here:
    case Token::_INLINE:
      next();
      break;
    case Token::_ATTRIBUTE:
      next();
      expect(Token::LPAREN, CHECK_ERROR);
      expect(Token::LPAREN, CHECK_ERROR);
      // just consume any constants
      next();
      expect(Token::RPAREN, CHECK_ERROR);
      expect(Token::RPAREN, CHECK_ERROR);
      break;
    default:
      *error = true;
      break;
  }
}

/**
* Because both variables and function declarations
* begin with type specifiers, we don't know which we're
* dealing with until the type has been parsed.
*/
Ast::Statement *Parser::parseFuncOrVar(bool* error)
{
  Ast::Statement *statement;
  Ast::Type* type = parseType(false, CHECK_ERROR_NULL);
  Ast::Type* pointerType = type;

  while(checkPointer()) {
    pointerType = new Ast::Pointer(type);
  }

  while(Token::IsNonStandard(peek())) {
    consumeNonStandard(CHECK_ERROR_NULL);
  }

  expect(Token::IDENTIFIER, CHECK_ERROR_NULL);

  if(peek() == Token::LPAREN) {
    statement = parseFunctionDecl(pointerType, CHECK_ERROR_NULL);
  }
  else {
    statement = parseVariableDeclList(type, pointerType, CHECK_ERROR_NULL);
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

  getOperand(CHECK_ERROR_NULL);

  if(Token::IsAssignmentOp(peek()) && !Token::IsUpdateOp(peek()))
  {
    Token::Value assign_op = next();
    Ast::Assignable *operand = (Ast::Assignable *)operands->top();
    operands->pop();
    expr = parseExpression(CHECK_ERROR_NULL);
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
      getOperand(CHECK_ERROR_NULL);
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

void Parser::getOperand(bool* error)
{
  Token::Value tok = peek();
  Ast::Expression* expr = NULL;

  if(tok == Token::IDENTIFIER)
  {
    expect(Token::IDENTIFIER, CHECK_ERROR);
    if(peek() == Token::LPAREN)
    {
      expr = parseFunctionInvocation(CHECK_ERROR);
    }
    else
    {
      string name = scanner.currentVal();

      Ast::Node *node;
      Ast::NodeType nodeType = scope->lookupIdentifier(name, node);
      if(nodeType == Ast::VARIABLE || nodeType == Ast::ARGUMENT)
      {
        expr = (Ast::Variable*)node;

        // We need to check for a postfix operation here.
        if(Token::IsUpdateOp(peek()))
        {
          expr = new Ast::UpdateExpression(next(), expr, false);
        }
      }
      else
      {
        cout << scanner._current->line << ": This variable is not defined in the current scope: " << name << "\n";
        *error = true;
      }
    }
  }
  else if(Token::IsIdentifierOrLiteral(tok))
  {
    next();
    if(tok == Token::NUMBER)
    {
      expr = new Ast::NumberLiteral(scanner.currentVal(), scanner.currentNumber());
    }
    else
    {
      expr = new Ast::Literal(scanner.currentVal());
    }
  }
  else if(tok == Token::LPAREN)
  {
    operators->push(SENTINEL);
    expect(Token::LPAREN, CHECK_ERROR);

    expr = parseExpression(CHECK_ERROR);

    expect(Token::RPAREN, CHECK_ERROR);
    operators->pop();
  }
  else if(Token::IsUnaryOp(tok))
  {
    next();
    Ast::Expression* operand = parseExpression(CHECK_ERROR);
    expr = new Ast::UnaryExpression(tok, operand);
  }
  else if(Token::IsUpdateOp(tok))
  {
    next();
    Ast::Expression *operand = parseExpression(CHECK_ERROR);
    expr = new Ast::UpdateExpression(tok, expr, true);
  }
  else
  {
    cout << "error parsing operand" << Token::String(tok) << "\n";
    *error = true;
  }

  while(peek() == Token::DOT || peek() == Token::LBRACK || peek() == Token::DEREF)
  {
    switch(next())
    {
      case Token::DEREF:
      case Token::DOT:
        expect(Token::IDENTIFIER, CHECK_ERROR);
        expr = new Ast::MemberExpression(expr, scanner.currentVal());
        break;
      case Token::LBRACK:
        Ast::Expression* index = parseExpression(CHECK_ERROR);
        expect(Token::RBRACK, CHECK_ERROR);
        expr = new Ast::ArrayAccess(expr, index);
        break;
    }
  }

  operands->push(expr);
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
    expression = parseExpression(CHECK_ERROR_NULL);
  }
  expect(Token::SEMICOLON, CHECK_ERROR_NULL);
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

  if(Token::IsStorageSpecifier(tok) || Token::IsTypeSpecifier(tok) ||
    scope->lookupIdentifier(scanner.peekVal()) == Ast::TYPEDEF)
  {
    statement = parseFuncOrVar(error);
  }
  else if(tok == Token::LPAREN || tok == Token::IDENTIFIER || tok == Token::NUMBER ||
       Token::IsUpdateOp(tok) || Token::IsUnaryOp(tok))
  {
    statement = parseExpressionStatement(error);
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
      case Token::SWITCH:
        statement = parseSwitch(error);
        break;
      case Token::SEMICOLON:
        statement = parseExpressionStatement(error);
        break;
      case Token::TYPEDEF:
        statement = parseTypedef(error);
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

Ast::Statement *Parser::parseFunctionDecl(Ast::Type* type, bool* error)
{
  string name = scanner.currentVal();
  Ast::FunctionPrototype *proto = NULL;
  Ast::ArgumentList* arguments = NULL;
  Ast::BlockStatement *body = NULL;

  expect(Token::LPAREN, CHECK_ERROR_NULL);
  arguments = parseArgumentList(CHECK_ERROR_NULL);
  expect(Token::RPAREN, CHECK_ERROR_NULL);


  proto = new Ast::FunctionPrototype(
      type,
      name,
      arguments
    );

  Ast::Node* node;
  Ast::NodeType nodeType = global->lookupIdentifier(name, node);

  if(nodeType == Ast::FUNCTION_PROTOTYPE)
  {
    if(!proto->matches((Ast::FunctionPrototype*)node))
    {
      cerr << "type mismatch error\n";
    }
  }
  else if(nodeType == Ast::NONE)
  {
    global->declareIdentifier(name, proto);
  }
  else
  {
    cerr << name << " has already been defined as another type";
  }

  // check if it's a prototype or definition.
  if(peek() == Token::LBRACE)
  {
    openScope();
    // We must define each of the arguments in the current
    // scope so they are available when parsing the block
    // statement.
    for(Ast::ArgumentList::iterator it = arguments->begin(); it != arguments->end(); ++it)
    {
      scope->declareIdentifier((*it)->name, (*it));
    }

    body = parseBlock(false, CHECK_ERROR_NULL);

    return new Ast::FunctionDeclaration(
      proto,
      body
    );
  }
  else {
    expect(Token::SEMICOLON, CHECK_ERROR_NULL);
    return proto;
  }
}

Ast::Argument *Parser::parseArgument(bool *error)
{
  Ast::Type* type = parseType(true, CHECK_ERROR_NULL);

  string name;

  if(peek() == Token::IDENTIFIER)
  {
    next();
    name = scanner.currentVal();
  }
  if(peek() == Token::LBRACK)
  {
    type = parseArraySize(type, CHECK_ERROR_NULL);
  }
  return new Ast::Argument(
        name,
        type
      );
}

Ast::ArgumentList *Parser::parseArgumentList(bool* error)
{
  Ast::ArgumentList* arguments = new Ast::ArgumentList(false);

  if(peek() != Token::RPAREN)
  {
    Ast::Argument *argument = parseArgument(CHECK_ERROR_NULL);
    arguments->push_back(argument);

    while(peek() == Token::COMMA)
    {
      expect(Token::COMMA, CHECK_ERROR_NULL);
      if(peek() == Token::DOT) {
        expect(Token::DOT, CHECK_ERROR_NULL);
        expect(Token::DOT, CHECK_ERROR_NULL);
        expect(Token::DOT, CHECK_ERROR_NULL);

        arguments->variable = true;
        break;
      }
      argument = parseArgument(CHECK_ERROR_NULL);
      arguments->push_back(argument);
    }
  }

  return arguments;
}


Ast::FunctionInvocation *Parser::parseFunctionInvocation(bool *error)
{
  // name has already been scanned at this stage
  string name = scanner.currentVal();
  Ast::List<Ast::Expression *> *arguments = new Ast::List<Ast::Expression *>();
  Ast::Expression *argument = NULL;

  expect(Token::LPAREN, CHECK_ERROR_NULL);

  // we have at least one argument
  if(peek() != Token::RPAREN)
  {
    argument = parseExpression(4, CHECK_ERROR_NULL);
    arguments->push_back(argument);

    while(peek() == Token::COMMA)
    {
      expect(Token::COMMA, CHECK_ERROR_NULL);
      argument = parseExpression(4, CHECK_ERROR_NULL);
      arguments->push_back(argument);
    }
  }
  Ast::Node* node;
  Ast::FunctionPrototype *function = NULL;
  if(global->lookupIdentifier(name, node) == Ast::FUNCTION_PROTOTYPE)
  {
    function = (Ast::FunctionPrototype*)node;
  }

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
  expect(Token::RPAREN, CHECK_ERROR_NULL);

  return new Ast::FunctionInvocation(
      function,
      arguments
    );
};
Ast::Declaration *Parser::parseVariableDeclList(Ast::Type* type, Ast::Type* pointerType, bool *error)
{
  Ast::List<Ast::Declarator *> *declarations = new Ast::List<Ast::Declarator *>();
  Ast::Declarator *declaration = parseVariableDecl(pointerType, true, CHECK_ERROR_NULL);
  // always has at least one variable declared
  declarations->push_back(declaration);

  while(peek() == Token::COMMA)
  {
    expect(Token::COMMA, CHECK_ERROR_NULL);
    declaration = parseVariableDecl(type, false, CHECK_ERROR_NULL);
    declarations->push_back(declaration);
  }

  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  return new Ast::Declaration(
      declarations
    );
}

/**
 * Parse a variable declaration. The type has already been parsed
 * by "parseFuncOrVar" so we just need to get the name, check whether
 * it's a pointer and assign an initial value if one exists.
 * @param  type  The type information that has already been parsed.
 * @param  hasName Whether the name token has been consumed or not.
 * @param  error Whether an error occured.
 * @return       A Declarator Ast Object.
 */
Ast::Declarator *Parser::parseVariableDecl(Ast::Type *type, bool hasName, bool* error)
{
  Ast::Expression* init = NULL;
  Ast::Variable* variable = parseVariableDef(type, hasName, CHECK_ERROR_NULL);

  if(peek() == Token::ASSIGN) {
    expect(Token::ASSIGN, CHECK_ERROR_NULL);
    init = parseExpression(4, CHECK_ERROR_NULL);
  }
  return new Ast::Declarator(
      variable,
      init
    );
}

Ast::List<Ast::Variable*>* Parser::parseStructDeclList(bool* error)
{
  Ast::List<Ast::Variable*>* declList = new Ast::List<Ast::Variable*>;
  Ast::Type* type = parseType(false, CHECK_ERROR_NULL);
  Ast::Variable* variable = parseVariableDef(type, false, CHECK_ERROR_NULL);
  declList->push_back(variable);

  while(peek() == Token::COMMA)
  {
    expect(Token::COMMA, CHECK_ERROR_NULL);
    variable = parseVariableDef(type, false, CHECK_ERROR_NULL);
    declList->push_back(variable);
  }

  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  return declList;
}

Ast::Variable* Parser::parseVariableDef(Ast::Type* type, bool hasName, bool* error)
{
  if(!hasName)
  {
    // If we already have the name then we can't check
    // whether it's a pointer or not.
    while(checkPointer())
    {
      type = new Ast::Pointer(type);
    }
    expect(Token::IDENTIFIER, CHECK_ERROR_NULL);
  }
  string name = scanner.currentVal();

  if(peek() == Token::LBRACK)
  {
    type = parseArraySize(type, CHECK_ERROR_NULL);
  }

  Ast::Variable* var = new Ast::Variable(name, type);
  if(!scope->declareIdentifier(name, var)) {
    *error = true;
    return NULL;
  }

  return var;
}

Ast::Type* Parser::parseArraySize(Ast::Type* type, bool* error)
{
  expect(Token::LBRACK, CHECK_ERROR_NULL);
  // the size is optional in most cases
  // TODO: handle cases where it's required
  Ast::Expression* size = peek() == Token::RBRACK ? NULL : parseExpression(CHECK_ERROR_NULL);
  expect(Token::RBRACK, CHECK_ERROR_NULL);

  if(peek() == Token::LBRACK)
  {
    type = parseArraySize(type, CHECK_ERROR_NULL);
  }

  return new Ast::Array(type, size);
}

inline bool Parser::checkPointer()
{
  return peek() == Token::MUL && (next(), true);
}

Ast::Typedef *Parser::parseTypedef(bool* error)
{
  Ast::Type* type;
  string name;
  Ast::Typedef* def;

  expect(Token::TYPEDEF, CHECK_ERROR_NULL);

  type = parseType(true, CHECK_ERROR_NULL);

  expect(Token::IDENTIFIER, CHECK_ERROR_NULL);
  name = scanner.currentVal();
  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  def = new Ast::Typedef(type, name);

  if(!global->declareIdentifier(name, def))
  {
    *error = true;
    return NULL;
  }

  return def;
}

Ast::ReturnStatement *Parser::parseReturn(bool* error)
{
  // return keyword
  expect(Token::RETURN, CHECK_ERROR_NULL);

  // followed by expression
  Ast::Expression *expression = parseExpression(CHECK_ERROR_NULL);

  // followed by semicolon
  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  return new Ast::ReturnStatement(
      expression
    );
}

Ast::IfStatement *Parser::parseIf(bool* error)
{
  Ast::Expression *condition = NULL;
  Ast::Statement *body = NULL;
  Ast::Statement *elseBody = NULL;

  expect(Token::IF, CHECK_ERROR_NULL);
  expect(Token::LPAREN, CHECK_ERROR_NULL);

  condition = parseExpression(CHECK_ERROR_NULL);

  expect(Token::RPAREN, CHECK_ERROR_NULL);

  body = parseStatement(CHECK_ERROR_NULL);

  if(peek() == Token::ELSE) {
    expect(Token::ELSE, CHECK_ERROR_NULL);
    elseBody = parseStatement(CHECK_ERROR_NULL);
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

  expect(Token::SWITCH, CHECK_ERROR_NULL);

  expect(Token::LPAREN, CHECK_ERROR_NULL);

  variable = parseExpression(CHECK_ERROR_NULL);

  expect(Token::RPAREN, CHECK_ERROR_NULL);

  expect(Token::LBRACE, CHECK_ERROR_NULL);

  // TODO: parse the body of the switch statement

  expect(Token::RBRACE, CHECK_ERROR_NULL);

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

  expect(Token::FOR, CHECK_ERROR_NULL);
  expect(Token::LPAREN, CHECK_ERROR_NULL);

  // expressions are optional
  if(peek() != Token::SEMICOLON) {
    // TODO: allow for declatations in init
    init = parseExpression(CHECK_ERROR_NULL);
  }
  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  // expressions are optional
  if(peek() != Token::SEMICOLON) {
    condition = parseExpression(CHECK_ERROR_NULL);
  }
  expect(Token::SEMICOLON, CHECK_ERROR_NULL);

  // expressions are optional
  if(peek() != Token::RPAREN) {
    loop = parseExpression(CHECK_ERROR_NULL);
  }
  expect(Token::RPAREN, CHECK_ERROR_NULL);

  // open new "wrapper" scope to contain
  // variables defined in the intialisation
  // of the for loop
  openScope();

  // parse any statement including a block
  // statement
  body = parseStatement(CHECK_ERROR_NULL);

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

  expect(Token::WHILE, CHECK_ERROR_NULL);
  expect(Token::LPAREN, CHECK_ERROR_NULL);

  condition = parseExpression(CHECK_ERROR_NULL);

  expect(Token::RPAREN, CHECK_ERROR_NULL);

  body = parseStatement(CHECK_ERROR_NULL);

  return new Ast::WhileStatement(
      condition,
      body
    );
}

Ast::Struct* Parser::parseStruct(bool* error)
{
  Ast::List<Ast::Variable *> *members;
  Ast::Struct* structDecl = NULL;
  string name;

  expect(Token::STRUCT, CHECK_ERROR_NULL);

  if(peek() == Token::IDENTIFIER) {
    expect(Token::IDENTIFIER, CHECK_ERROR_NULL);
    name = scanner.currentVal();

    Ast::Node* node;
    Ast::NodeType type = scope->lookupIdentifier(name, node);
    if(type == Ast::STRUCT)
    {
      structDecl = (Ast::Struct*)node;
      if(peek() == Token::LBRACE)
      {
        cerr << "redefinition of struct\n";
        *error = true;
        return NULL;
      }
    }
    else if(type != Ast::NONE)
    {
      cerr << "error defining struct: identifier already used for another type\n";
      *error = true;
      return NULL;
    }
  }

  // definition of struct
  if(peek() == Token::LBRACE)
  {
    openScope();
    expect(Token::LBRACE, CHECK_ERROR_NULL);
    members = new Ast::List<Ast::Variable *>();

    while(peek() != Token::RBRACE)
    {
      members = parseStructDeclList(CHECK_ERROR_NULL);
      //members->push_back(decl);
    }

    expect(Token::RBRACE, CHECK_ERROR_NULL);
    closeScope();

    structDecl = new Ast::Struct(name, members);
    scope->declareIdentifier(name, structDecl);
  }


  return structDecl;
}

Ast::List<Ast::Expression *> *Parser::parseArrayExpression(bool *error)
{
  Ast::List<Ast::Expression *> *elements = new Ast::List<Ast::Expression *>();
  Ast::Expression *expr;

  expect(Token::LBRACE, CHECK_ERROR_NULL);

  while(peek() != Token::RBRACE)
  {
    expr = parseExpression(4, CHECK_ERROR_NULL);

    elements->push_back(expr);
  }

  expect(Token::RBRACE, CHECK_ERROR_NULL);

  return elements;
}

Ast::BlockStatement *Parser::parseBlock(bool* error)
{
  // open a scope for every block by default
  return parseBlock(true, error);
}

// Overloaded to allow defining whether or not
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

  expect(Token::LBRACE, CHECK_ERROR_NULL);

  while(peek() != Token::RBRACE && peek() != Token::EOS)
  {
    statement = parseStatement(CHECK_ERROR_NULL);
    statements->push_back(statement);
  }

  // the scope will always be closed at the end of the
  expect(Token::RBRACE, error);

  closeScope();

  return new Ast::BlockStatement(
      statements
    );
}
