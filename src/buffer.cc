#include "buffer.h"

Buffer::Buffer() {
  len = 0;
}

void Buffer::push(char c) {
  if(len < BUF_SIZE) {
    buffer[len++] = c;
  }
}

void Buffer::reset(int lineOffset, int colOffset) {
  len = 0;
  line = lineOffset;
  col = colOffset;
}

string Buffer::toString() {
  return string(buffer, len);
}
