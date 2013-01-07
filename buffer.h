#ifndef _BUFFER
#define _BUFFER

#include <string>
#include "token.h"

#define BUF_SIZE 256

class Buffer {
public:
  char buffer[BUF_SIZE];

  unsigned long int len;
  unsigned long int line;
  unsigned long int col;

  Token::Value token;

  number numberValue;

  Buffer();

  void push(char);
  // reset the buffer to a position in the file
  void reset(int, int);

  string toString();
};

#endif