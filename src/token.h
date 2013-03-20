#ifndef _TOKEN
#define _TOKEN

#include <string>
#include <cassert>

using namespace std;

#define TOKEN_LIST(T, K)                                                \
  /* End of source indicator. */                                        \
  T(EOS, "EOS", 0)                                                      \
  T(ILLEGAL, "ILLEGAL", 0)                                              \
                                                                        \
  T(LPAREN, "(", 0)                                                     \
  T(RPAREN, ")", 0)                                                     \
  T(LBRACK, "[", 0)                                                     \
  T(RBRACK, "]", 0)                                                     \
  T(LBRACE, "{", 0)                                                     \
  T(RBRACE, "}", 0)                                                     \
  T(COLON, ":", 0)                                                      \
  T(SEMICOLON, ";", 0)                                                  \
  T(DOT, ".", 0)                                                        \
  T(DEREF, "->", 0)                                                     \
  T(CONDITIONAL, "?", 3)                                                \
                                                                        \
  /* Assignment operators. */                                           \
  /* IsAssignmentOp() and Assignment::is_compound() relies on */        \
  /* this block of enum values being contiguous and sorted in the */    \
  /* same order! */                                                     \
  T(ASSIGN, "=", 2)                                                     \
  T(OR_ASSIGN, "|=", 2)                                                 \
  T(XOR_ASSIGN, "^=", 2)                                                \
  T(AND_ASSIGN, "&=", 2)                                                \
  T(LSHIFT_ASSIGN, "<<=", 2)                                            \
  T(RSHIFT_ASSIGN, ">>=", 2)                                            \
  T(ADD_ASSIGN, "+=", 2)                                                \
  T(SUB_ASSIGN, "-=", 2)                                                \
  T(MUL_ASSIGN, "*=", 2)                                                \
  T(INC, "++", 0)                                                       \
  T(DEC, "--", 0)                                                       \
  T(DIV_ASSIGN, "/=", 2)                                                \
  T(MOD_ASSIGN, "%=", 2)                                                \
                                                                        \
  /* Binary operators sorted by precedence. */                          \
  /* IsBinaryOp() relies on this block of enum values */                \
  /* being contiguous and sorted in the same order! */                  \
  T(COMMA, ",", 1)                                                      \
  T(OR, "||", 4)                                                        \
  T(AND, "&&", 5)                                                       \
  T(BIT_OR, "|", 6)                                                     \
  T(BIT_XOR, "^", 7)                                                    \
  T(BIT_AND, "&", 8)                                                    \
  T(SHL, "<<", 11)                                                      \
  T(SHR, ">>", 11)                                                      \
  T(ADD, "+", 12)                                                       \
  T(SUB, "-", 12)                                                       \
  T(MUL, "*", 13)                                                       \
  T(DIV, "/", 13)                                                       \
  T(MOD, "%", 13)                                                       \
                                                                        \
  /* Compare operators sorted by precedence. */                         \
  /* IsCompareOp() relies on this block of enum values */               \
  /* being contiguous and sorted in the same order! */                  \
  T(EQ, "==", 9)                                                        \
  T(NE, "!=", 9)                                                        \
  T(LT, "<", 10)                                                        \
  T(GT, ">", 10)                                                        \
  T(LTE, "<=", 10)                                                      \
  T(GTE, ">=", 10)                                                      \
                                                                        \
  /* Unary operators. */                                                \
  /* IsUnaryOp() relies on this block of enum values */                 \
  /* being contiguous and sorted in the same order! */                  \
  T(NOT, "!", 0)                                                        \
  T(BIT_NOT, "~", 0)                                                    \
                                                                        \
  /* keywords */                                                        \
  K(BREAK, "break", 0)                                                  \
  K(CASE, "case", 0)                                                    \
  K(CONTINUE, "continue", 0)                                            \
  K(DEFAULT, "default", 0)                                              \
  K(DO, "do", 0)                                                        \
  K(ELSE, "else", 0)                                                    \
  K(FOR, "for", 0)                                                      \
  K(GOTO, "goto", 0)                                                    \
  K(IF, "if", 0)                                                        \
  K(RETURN, "return", 0)                                                \
  K(SIZEOF, "sizeof", 0)                                                \
  K(SWITCH, "switch", 0)                                                \
  K(TYPEDEF, "typedef", 0)                                              \
  K(WHILE, "while", 0)                                                  \
  K(MORE, "...", 0)                                                     \
                                                                        \
  /* Type specifiers */                                                 \
  K(CHAR, "char", 0)                                                    \
  K(DOUBLE, "double", 0)                                                \
  K(FLOAT, "float", 0)                                                  \
  K(INT, "int", 0)                                                      \
  K(LONG, "long", 0)                                                    \
  K(SHORT, "short", 0)                                                  \
  K(UNSIGNED, "unsigned", 0)                                            \
  K(SIGNED, "signed", 0)                                                \
  K(STRUCT, "struct", 0)                                                \
  K(UNION, "union", 0)                                                  \
  K(ENUM, "enum", 0)                                                    \
  K(VOID, "void", 0)                                                    \
                                                                        \
  /* Storage class specifiers */                                        \
  K(AUTO, "auto", 0)                                                    \
  K(CONST, "const", 0)                                                  \
  K(EXTERN, "extern", 0)                                                \
  K(REGISTER, "register", 0)                                            \
  K(STATIC, "static", 0)                                                \
  K(INLINE, "inline", 0)                                                \
  K(VOLATILE, "volatile", 0)                                            \
                                                                        \
  T(COMMENT, NULL, 0)                                                   \
                                                                        \
  T(NUMBER, NULL, 0)                                                    \
  T(STRING, NULL, 0)                                                    \
  T(CHARACTER, NULL, 0)                                                 \
  T(IDENTIFIER, NULL, 0)                                                \
                                                                        \
  /* Non standard tokens */                                             \
  K(_INLINE, "__inline__", 0)                                           \
  K(_ATTRIBUTE, "__attribute__", 0)





typedef union _number {
  signed int intVal;
  double doubleVal;
} number;


class Token {

public:

#define T(name, string, precedence) name,
  enum Value {
    TOKEN_LIST(T, T)
    NUM_TOKENS
  };
#undef T


  // Returns a string corresponding to the C++ token name
  // (e.g. "LT" for the token LT).
  static const char* Name(Value tok)
  {
    assert(tok < NUM_TOKENS);  // tok is unsigned
    return name_[tok];
  }

  // Predicates
  static bool IsKeyword(Value tok)
  {
    return token_type[tok] == 'K';
  }

  static bool IsIdentifierOrLiteral(Value tok)
  {
    return tok >= NUMBER && tok <= IDENTIFIER;
  }

  static bool IsBinaryOp(Value op)
  {
    return COMMA <= op && op <= MOD;
  }

  static bool IsAssignmentOp(Value op)
  {
    return ASSIGN <= op && op <= MOD_ASSIGN;
  }

  static bool IsCompareOp(Value op)
  {
    return EQ <= op && op <= GTE;
  }

  static bool IsBitOp(Value op)
  {
    return (BIT_OR <= op && op <= SHR) || op == BIT_NOT;
  }

  static bool IsUnaryOp(Value op)
  {
    return (NOT <= op && op <= BIT_NOT) || op == ADD || op == SUB;
  }

  static bool IsUpdateOp(Value op)
  {
    return op == INC || op == DEC;
  }

  static bool IsShiftOp(Value op)
  {
    return (SHL <= op) && (op <= SHR);
  }

  static bool IsMemberOp(Value op)
  {
    return op == DOT || op == DEREF;
  }

  static bool IsStorageSpecifier(Value op)
  {
    return (AUTO <= op) && (op <= VOLATILE);
  }

  static bool IsTypeSpecifier(Value op)
  {
    return (CHAR <= op) && (op <= VOID);
  }

  static bool IsNonStandard(Value op)
  {
    return (_INLINE <= op) && (op <= _ATTRIBUTE);
  }

  // Returns a string corresponding to the JS token string
  // (.e., "<" for the token LT) or NULL if the token doesn't
  // have a (unique) string (e.g. an IDENTIFIER).
  static const char* String(Value tok)
  {
    assert(tok < NUM_TOKENS);  // tok is unsigned.
    return string_[tok];
  }

  // Returns the precedence > 0 for binary and compare
  // operators; returns 0 otherwise.
  static int Precedence(Value tok)
  {
    if(tok > NUM_TOKENS) {
      return 0;
    }
    assert(tok < NUM_TOKENS);  // tok is unsigned.
    return precedence_[tok];
  }


private:
  static const char* const name_[NUM_TOKENS];
  static const char* const string_[NUM_TOKENS];
  static const int precedence_[NUM_TOKENS];
  static const char token_type[NUM_TOKENS];

};

#endif