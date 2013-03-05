#include "ast.h"


const char *Ast::Type::types[] = {
  "short",
  "long",
  "long",
  "signed",
  "unsigned",
  "double",
  "float",
  "int",
  "char"
};

uint32_t Ast::Printable::ref_count = 0;
