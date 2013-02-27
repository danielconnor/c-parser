#include "scanner.h"

Scanner::Scanner(const char* filename)
{
  file.open(filename);
  lineOffset = 0;
  colOffset = 0;
  offset = 0;

  _next = new Buffer();
  _current = new Buffer();

  next();
  // load the buffers so that there is a value
  // ready for peek.
  scan();
}

Scanner::~Scanner()
{
  delete _next;
  delete _current;
}

char Scanner::next()
{
  current = file.get();
  offset++;
  colOffset++;
  if(current == '\n') {
    colOffset = 0;
    lineOffset++;
  }
  return current;
}

inline Token::Value Scanner::select(Token::Value token, char nChar, Token::Value nToken)
{
  if(current == nChar) {
    token = nToken;
    next();
  }
  return token;
}
inline Token::Value Scanner::select(Token::Value token, char nChar, Token::Value nToken, char nnChar, Token::Value nnToken)
{
  if(current == nChar) {
    token = nToken;
    next();
    if(current == nnChar) {
      token = nnToken;
      next();
    }
  }
  return token;
}

inline Token::Value Scanner::select2(Token::Value token, char nChar, Token::Value nToken, char nnChar, Token::Value nnToken)
{
  if(current == nChar) {
    token = nToken;
    next();
  }
  else if(current == nnChar) {
    token = nnToken;
    next();
  }
  return token;
}

inline Token::Value Scanner::select3(Token::Value token, char nChar, Token::Value nToken, char nnChar, Token::Value nnToken, char nnnChar, Token::Value nnnToken)
{
  if(current == nChar) {
    token = nToken;
    next();
  }
  else if(current == nnChar) {
    token = nnToken;
    next();
  }
  else if(current == nnnChar) {
    token = nnnToken;
    next();
  }
  return token;
}

Token::Value Scanner::peek()
{
  return _next->token;
}

string Scanner::peekVal()
{
  return _next->toString();
}

string Scanner::currentVal()
{
  return _current->toString();
}

number Scanner::currentNumber()
{
  return _current->numberValue;
}


// swap the buffers and discard what was previously
// in the current buffer.
void Scanner::swapBuffers()
{
  Buffer *temp = _next;
  _next = _current;

  // current becomes the previous "peek" value
  _current = temp;

  // the current value is discarded and it's position
  // is reset to the current position in the file
  // because this is where we start reading the next token.
  _next->reset(lineOffset, colOffset);
}

Token::Value Scanner::scan()
{
  // go to the first character of the next token
  skipWhitespace();

  swapBuffers();

  // the line after the whitespace has been skipped
  // and before the token has been scanned
  char ch = current;

  Token::Value token = Token::EOS;

  if(isalpha(current) || current == '_') {
    token = scanIdentifierOrKeyword();
  }
  else if(isdigit(current)) {
    token = scanNumber();
  }
  else {
    next();
    switch(ch) {
      case '/':
        if(current == '/' || current == '*') {
          token = scanComment();
        }
        else {
          token = select(Token::DIV, '=', Token::DIV_ASSIGN);
        }
        break;
      case '"':
        token = scanString();
        break;
      case '!':
        token = Token::NOT;
        break;
      case '\'':
        token = scanChar();
        break;
      case ';':
        token = Token::SEMICOLON;
        break;
      case '(':
        token = Token::LPAREN;
        break;
      case ')':
        token = Token::RPAREN;
        break;
      case '{':
        token = Token::LBRACE;
        break;
      case '}':
        token = Token::RBRACE;
        break;
      case '.':
        token = Token::DOT;
        break;
      case ',':
        token = Token::COMMA;
        break;
      case '~':
        token = Token::BIT_NOT;
        break;
      case '=': // =, ==
        token = select(Token::ASSIGN, '=', Token::EQ);
        break;
      case '-': // -, --, -=, ->
        token = select3(Token::SUB, '-', Token::DEC, '=', Token::SUB_ASSIGN, '>', Token::DEREF);
        break;
      case '+': // +, ++, +=
        token = select2(Token::ADD, '+', Token::INC, '=', Token::ADD_ASSIGN);
        break;
      case '*': // *, *=
        token = select(Token::MUL, '=', Token::MUL_ASSIGN);
        break;
      case '|': // |, ||
        token = select(Token::BIT_OR, '=', Token::OR);
        break;
      case '&': // &, &&, &=
        token = select2(Token::BIT_AND, '&', Token::AND, '=', Token::AND_ASSIGN);
        break;
      case '>': // >, >>, >>=
        token = select(Token::LT, '>', Token::SHR, '=', Token::RSHIFT_ASSIGN);
        break;
      case '%': // %, %=
        token = select(Token::MOD, '=', Token::MOD_ASSIGN);
        break;
      case '<': // <, <<, <<=
        token = select(Token::LT, '<', Token::SHL, '=', Token::LSHIFT_ASSIGN);
        break;
    }
  }

  // the parser doesn't need comments
  // so we get the next character
  if(token == Token::COMMENT) {
    swapBuffers();
    return scan();
  }

  _next->token = token;
  return _current->token;
}


void Scanner::skipWhitespace()
{
  while(current == ' ' || current == '\n' || current == '\t' || current == '\r') {
    next();
  }
}

#define KEYWORDS(GROUP, KEYWORD)                                           \
  GROUP('a')                                                               \
  KEYWORD(AUTO, "auto")                                                    \
  GROUP('b')                                                               \
  KEYWORD(BREAK, "break")                                                  \
  GROUP('c')                                                               \
  KEYWORD(CASE, "case")                                                    \
  KEYWORD(CHAR, "char")                                                    \
  KEYWORD(CONST, "const")                                                  \
  KEYWORD(CONTINUE, "continue")                                            \
  GROUP('d')                                                               \
  KEYWORD(DEFAULT, "default")                                              \
  KEYWORD(DO, "do")                                                        \
  KEYWORD(DOUBLE, "double")                                                \
  GROUP('e')                                                               \
  KEYWORD(ELSE, "else")                                                    \
  KEYWORD(ENUM, "enum")                                                    \
  KEYWORD(EXTERN, "extern")                                                \
  GROUP('f')                                                               \
  KEYWORD(FLOAT, "float")                                                  \
  KEYWORD(FOR, "for")                                                      \
  GROUP('g')                                                               \
  KEYWORD(GOTO, "goto")                                                    \
  GROUP('i')                                                               \
  KEYWORD(IF, "if")                                                        \
  KEYWORD(INT, "int")                                                      \
  GROUP('l')                                                               \
  KEYWORD(LONG, "long")                                                    \
  GROUP('r')                                                               \
  KEYWORD(REGISTER, "register")                                            \
  KEYWORD(RETURN, "return")                                                \
  GROUP('s')                                                               \
  KEYWORD(SHORT, "short")                                                  \
  KEYWORD(SIGNED, "signed")                                                \
  KEYWORD(SIZEOF, "sizeof")                                                \
  KEYWORD(STATIC, "static")                                                \
  KEYWORD(STRUCT, "struct")                                                \
  KEYWORD(SWITCH, "switch")                                                \
  GROUP('t')                                                               \
  KEYWORD(TYPEDEF, "typedef")                                              \
  GROUP('u')                                                               \
  KEYWORD(UNION, "union")                                                  \
  KEYWORD(UNSIGNED, "unsigned")                                            \
  KEYWORD(VOID, "void")                                                    \
  GROUP('v')                                                               \
  KEYWORD(VOLATILE, "volatile")                                            \
  GROUP('w')                                                               \
  KEYWORD(WHILE, "while")


Token::Value Scanner::IdentifyKeyword(char *string, int strLen)
{
  switch (string[0]) {
    default:
#define GROUP_CASE(ch)                                        \
      break;                                                  \
    case ch:
#define KEYWORD(token, keyword)                               \
    {                                                         \
      /* 'keyword' is a char array, so sizeof(keyword) is */  \
      /* strlen(keyword) plus 1 for the NULL char. */         \
      const int len = sizeof(keyword) - 1;                    \
      if (strLen == len && string[1] == keyword[1] &&         \
          (len <= 2 || string[2] == keyword[2]) &&            \
          (len <= 3 || string[3] == keyword[3]) &&            \
          (len <= 4 || string[4] == keyword[4]) &&            \
          (len <= 5 || string[5] == keyword[5]) &&            \
          (len <= 6 || string[6] == keyword[6]) &&            \
          (len <= 7 || string[7] == keyword[7]) &&            \
          (len <= 8 || string[8] == keyword[8]) &&            \
          (len <= 9 || string[9] == keyword[9])) {            \
        return Token::token;                                  \
      }                                                       \
    }
    KEYWORDS(GROUP_CASE, KEYWORD)
  }

  return Token::IDENTIFIER;
}

Token::Value Scanner::scanIdentifierOrKeyword()
{
  do
  {
    _next->push(current);
    next();
  } while(isalnum(current) || current == '_');

  return IdentifyKeyword(_next->buffer, _next->len);
}

Token::Value Scanner::scanNumber()
{
  int value = 0;

  while(isdigit(current))
  {
    _next->push(current);

    value = (value * 10) + (current - '0');

    next();
  }
  _next->numberValue.intVal = value;


  if(current == '.')
  {
    double offset = 10;
    double mantissa = 0;

    _next->push(current);
    next();

    while(isdigit(current))
    {
      _next->push(current);

      mantissa += (current - '0') / offset;
      offset += 10;
      next();
    }

    cout << "value: " << value << "\n";
    cout << "mantissa: " << mantissa << "\n";
    _next->numberValue.doubleVal = value + mantissa;
  }

  return Token::NUMBER;
}

//TODO: handle escapes properly
void Scanner::scanEscape()
{
  next();
  switch (current) {
    case 'a':
    case 'b':
    case 'f':
    case 'n':
    case 'r':
    case 't':
    case 'v':
    case '\\':
      _next->push(current);
      return;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
      break;
    case 'x':
      break;
    case 'u':
      break;
    case 'U':
    default:
      return;
  }
}

Token::Value Scanner::scanString()
{
  while(current != '"' && current >= 0) {
    _next->push(current);
    if(current == '\\') {
      scanEscape();
    }
    next();
  }
  next();

  return Token::STRING;
}

Token::Value Scanner::scanChar()
{
  while(current != '\'' && current >= 0) {
    _next->push(current);
    if(current == '\\') {
      scanEscape();
    }
    next();
  }
  next();

  return Token::CHARACTER;
}

Token::Value Scanner::scanComment()
{
  char ch;

  _next->push('/');

  // multiline comment
  if(current == '*') {

    while(current >= 0) {
      ch = current;
      _next->push(ch);
      next();
      if(ch == '*' && current == '/') {
        _next->push(current);
        next();
        break;
      }
    }
  }
  //single line comment
  else if(current == '/') {
    while(current != '\n' && current >= 0) {
      _next->push(current);
      next();
    }
  }

  return Token::COMMENT;
}
