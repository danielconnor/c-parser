#include "ast.h"


const char* const Ast::FundamentalType::types[] = {
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

const char* const Ast::NodeTypeName[] = {
  #define P_NAME(E, S) S,
  STATEMENT_TYPES(P_NAME)
  #undef P_NAME
};

uint32_t Ast::Printable::ref_count = 0;
