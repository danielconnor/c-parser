#ifndef _SCANNER
#define _SCANNER

#include <istream>
#include <string>
#include <iostream>
#include <ctype.h>

#include "token.h"
#include "buffer.h"


class Scanner {

private:
  std::istream& file;

public:
  Buffer *_current;
  Buffer *_next;
  void swapBuffers();

  Scanner(istream&);
  ~Scanner();

  // overall offset
  int offset;
  // line number
  int lineOffset;
  // column offset for the current line
  int colOffset;

  char current;
  char next();

  Token::Value scanComment();
  Token::Value scanIdentifierOrKeyword();
  Token::Value scanString();
  Token::Value scanChar();
  Token::Value scanNumber();
  void scanEscape();

  inline Token::Value select(Token::Value, char, Token::Value);
  inline Token::Value select(Token::Value, char, Token::Value, char, Token::Value);

  inline Token::Value select2(Token::Value, char, Token::Value, char, Token::Value);
  inline Token::Value select3(Token::Value, char, Token::Value, char, Token::Value, char, Token::Value);

  static Token::Value IdentifyKeyword(char *, int);

  void skipWhitespace();

  Token::Value scan();
  Token::Value peek();
  string currentVal();
  number currentNumber();
  string peekVal();
};

#endif