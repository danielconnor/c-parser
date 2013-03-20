#ifndef AST_
#define AST_


#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>
#include <stdint.h>

#include "token.h"
#include "statements.h"

using namespace std;

class Ast {

public:
  static const char* const NodeTypeName[];
  #define P_ENUM(E, S) E,
  enum NodeType {
    STATEMENT_TYPES(P_ENUM)
    NUM_STATEMENTS
  };
  #undef P_ENUM

  class Node;
  class Expression;
  class Statement;
  class Variable;
  class Type;

  /**
  * Super class for all ast types to allow them to
  * be printed to JSON
  */
  class Printable {
  public:
    int c;

    static uint32_t ref_count;

    Printable()
    {
      ref_count++;
    }
    virtual ~Printable()
    {
      ref_count--;
    }

    void indent(ostream &out, int indentation)
    {
      for(int i = 0; i < indentation; i++)
      {
        out << "    ";
      }
    }

    // print opening brace
    virtual void printStart(ostream &out)
    {
      out << '{';
    }

    virtual void printContents(ostream &out, int indentation)
    {
    }

    // print closing brace
    virtual void printEnd(ostream &out) {
      out << '}';
    }

    void printName(ostream &out, string name, int indentation)
    {
      if(c) {
        out << ",\n";
      }
      indent(out, indentation);
      out << '"' << name << '"' << ": ";
      c++;
    }

    void printMember(ostream &out, string name, Printable *member, int indentation)
    {
      printName(out, name, indentation);
      if(member)
      {
        member->print(out, indentation);
      }
      else {
        out << "null";
        c++;
      }
    }

    void printMember(ostream &out, string name, double value, int indentation)
    {
      printName(out, name, indentation);
      out << value;
    }

    void printMember(ostream &out, string name, int value, int indentation)
    {
      printName(out, name, indentation);
      out << value;
    }

    void printMember(ostream &out, string name, string value, int indentation)
    {
      printName(out, name, indentation);
      out << '"' << value << '"';
    }

    void printMember(ostream &out, string name, char *value, int indentation)
    {
      printName(out, name, indentation);
      out << '"' << value << '"';
    }

    virtual void printType(ostream &out, int indentation)
    {
      printMember(out, "typeName", getTypeString(), indentation);
    }

    string getTypeString()
    {
      return NodeTypeName[getType()];
    }
    virtual inline NodeType getType() const
    {
      return NONE;
    }

    void print(const char *filename)
    {
      print(filename, 0);
    }

    void print(const char *filename, int indentation)
    {
      ofstream out;
      out.open(filename);
      print(out, indentation);
      out.close();
    }

    void print()
    {
      print(cout);
    }

    void print(ostream &out)
    {
      print(out, 0);
    }

    void print(ostream &out, int indentation)
    {
      c = 0;
      printStart(out);
      out << "\n";
      printType(out, indentation + 1);
      printContents(out, indentation + 1);
      if(c) out << "\n";
      indent(out, indentation);
      printEnd(out);
    }

    void printVar(ostream &out, string var)
    {
      out << "var " << var << " = ";
      print(out);
      out << ";";
    }

    void printVar(const char *filename, string var)
    {
      ofstream out;
      out.open(filename);

      printVar(out, var);

      out.close();
    }
  };

  template <typename T> class List : public vector<T>, public Printable {
  public:
    void printStart(ostream &out)
    {
      out << '[';
    }
    void printEnd(ostream &out)
    {
      out << ']';
    }

    virtual void printContents(ostream &out, int indentation)
    {
      for(vector<T>::size_type i = 0; i < this->size(); i++)
      {
        if(c)
        {
          out << ",\n";
        }

        T item = (*this)[c];

        if(item) {
          indent(out, indentation);
          item->print(out, indentation);
          c++;
        }
      }
    }

    virtual void printType(ostream &out, int indentation) {}

    virtual ~List()
    {
      for(vector<T>::size_type i = 0; i < this->size(); i++)
      {
        delete this->at(i);
      }
      this->clear();
    }
  };

  class Type : public Printable
  {
  public:
    virtual uint64_t getSize() = 0;
    virtual uint8_t getAlign() = 0;
    virtual bool matches(Type*) = 0;
  };

  typedef uint16_t Typemask;

  class FundamentalType : public Type
  {
  public:
    Typemask type;
    const static char* const types[];

    FundamentalType(Typemask type) : type(type)
    {}

    virtual void printContents(ostream &out, int i)
    {
      string name = "";
      Typemask mask = 1;

      for(int s = 0; s < 10; s++)
      {
        if(type & mask)
        {
          if(name.length())
          {
            name.append(" ");
          }
          name.append(types[s]);
        }
        mask <<= 1;
      }
      printMember(out, "name", name, i);
    }

    virtual uint64_t getSize()
    {
      return 0;
    }
    virtual uint8_t getAlign()
    {
      return 0;
    }
    virtual inline NodeType getType() const
    {
      return FUNDAMENTAL_TYPE;
    }
    virtual bool matches(Type* other)
    {
      return getType() == other->getType() && type == ((FundamentalType*)other)->type;
    }
  };

  class ComplexType : public Type {
  public:
    List<Variable*>* members;

    ComplexType(List<Variable*>* members) : members(members)
    {}

    virtual ~ComplexType()
    {
      delete members;
    }
    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "members", members, i);
    }
  };

  class AnonymousComplexType : public ComplexType
  {};

  class NamedComplexType : public ComplexType {
  public:
    string name;

    NamedComplexType(string name, List<Variable *>* members) : name(name), ComplexType(members)
    {}

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "name", name, i);
      ComplexType::printContents(out, i);
    }
  };

  class Struct : public NamedComplexType
  {
  public:
  Struct(string name, List<Variable *>* members) : NamedComplexType(name, members)
  {}

    virtual uint64_t getSize()
    {
      //TODO
      return 0;
    }
    virtual uint8_t getAlign()
    {
      //TODO
      return 0;
    }
    virtual inline NodeType getType() const
    {
      return STRUCT;
    }
    virtual bool matches(Type* other)
    {
      return true;
    }
  };

  class Union : public NamedComplexType
  {
  public:
    virtual uint64_t getSize()
    {
      //TODO
      return 0;
    }
    virtual uint8_t getAlign()
    {
      //TODO
      return 0;
    }
    virtual inline NodeType getType() const
    {
      return UNION;
    }
    virtual bool matches(Type* other)
    {
      return true;
    }
  };

  class Pointer : public Type
  {
  public:
    Type* type;

    Pointer(Type* type): type(type)
    {}

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "type", type, i);
    }

    virtual inline NodeType getType() const
    {
      return POINTER;
    }

    virtual uint64_t getSize()
    {
      //TODO
      return 32;
    }
    virtual uint8_t getAlign()
    {
      //TODO
      return 32;
    }

    virtual bool matches(Type* other)
    {
      return (POINTER == other->getType() ||
              ARRAY == other->getType()) && type->matches(((Pointer*)other)->type);
    }
  };

  class Array : public Pointer
  {
  public:
    Expression* size;
    Array(Type* type, Expression* size) : Pointer(type), size(size)
    {}
    virtual inline NodeType getType() const
    {
      return ARRAY;
    }
    virtual void printContents(ostream &out, int i)
    {
      Pointer::printContents(out, i);
      printMember(out, "size", size, i);
    }
  };

  class Position : public Printable
  {
    int line;
    int col;

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "line", line, i);
      printMember(out, "col", col, i);
    }
  };


  class Node : public Printable
  {
  //   Position start;
  //   Position end;

  // public:
  //   Node(Position start, Position end)
  //   {}

  //   virtual void printContents(ostream &out, int i) {
  //     printMember(out, "start", start, i);
  //     printMember(out, "end", end, i);
  //   }
  };

  class Expression : public Node
  {
  };

  class Literal : public Expression
  {
  public:
    string raw;

    Literal(string raw)
    {
      this->raw = raw;
    }

    virtual inline NodeType getType() const
    {
      return LITERAL;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "raw", raw, i);
    }
  };


  class NumberLiteral : public Literal
  {
  public:
    number value;

    NumberLiteral(string raw, number value) : Literal(raw)
    {
      this->value = value;
    }

    virtual inline NodeType getType() const
    {
      return NUMBER_LITERAL;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "raw", raw, i);
      printMember(out, "intVal", value.intVal, i);
      printMember(out, "doubleVal", value.doubleVal, i);
    }
  };


  class Assignable : public Expression {};

  class Variable : public Assignable
  {
  public:
    string name;
    Type *type;

    Variable(string name, Type *type): name(name), type(type)
    {
      this->name = name;
      this->type = type;
    }
    virtual ~Variable()
    {
      // Because types are stored in the scope
      // and re-used for each of the declarators
      // they cannot be deleted by the variable
      // so they must be deleted in the declarator
      // where it was created.
      // delete type;
    }

    virtual inline NodeType getType() const
    {
      return VARIABLE;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "name", name, i);
      printMember(out, "type", type, i);
    }
  };

  class OpExpression : public Expression
  {
  public:
    Token::Value op;
    OpExpression(Token::Value op)
    {
      this->op = op;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "op", Token::String(op), i);
      printMember(out, "opName", Token::Name(op), i);
    }
  };

  class AssignExpression : public OpExpression
  {
  public:
    Assignable *left;
    Expression *right;

    AssignExpression(Token::Value op, Assignable *left, Expression *right) : OpExpression(op)
    {
      this->left = left;
      this->right = right;
    }
    virtual ~AssignExpression()
    {
      delete left;
      delete right;
    }

    virtual inline NodeType getType() const
    {
      return ASSIGN_EXPRESSION;
    }

    virtual void printContents(ostream &out, int i)
    {
      OpExpression::printContents(out, i);
      printMember(out, "left", left, i);
      printMember(out, "right", right, i);
    }
  };

  class UnaryExpression : public OpExpression
  {
  public:
    Expression *operand;

    UnaryExpression(Token::Value op, Expression *operand) : OpExpression(op)
    {
      this->operand = operand;
    }

    virtual ~UnaryExpression()
    {
      delete operand;
    }

    virtual inline NodeType getType() const
    {
      return UNARY_EXPRESSION;
    }

    virtual void printContents(ostream &out, int i)
    {
      OpExpression::printContents(out, i);
      printMember(out, "operand", operand, i);
    }
  };

  class UpdateExpression : public UnaryExpression
  {
  public:
    bool prefix;

    UpdateExpression(Token::Value op, Expression *operand, bool prefix) : UnaryExpression(op, operand)
    {
      this->prefix = prefix;
    }

    virtual inline NodeType getType() const
    {
      return UPDATE_EXPRESSION;
    }

    virtual void printContents(ostream &out, int i)
    {
      UnaryExpression::printContents(out, i);
      printMember(out, "prefix", prefix, i);
    }
  };


  class BinaryExpression : public OpExpression
  {
  public:
    Expression *left;
    Expression *right;

    BinaryExpression(Token::Value op, Expression *left, Expression *right) :
      OpExpression(op)
    {
      this->left = left;
      this->right = right;
    }

    virtual ~BinaryExpression()
    {
      delete left;
      delete right;
    }

    virtual inline NodeType getType() const
    {
      return BINARY_EXPRESSION;
    }

    virtual void printContents(ostream &out, int i)
    {
      OpExpression::printContents(out, i);
      printMember(out, "left", left, i);
      printMember(out, "right", right, i);
    }
  };

  class Statement : public Node
  {
  };

  class CtrlStatement : public Statement
  {
    Expression *condition;
    Statement *body;

  public:
    CtrlStatement(Expression *condition, Statement *body)
    {
      this->condition = condition;
      this->body = body;
    }

    virtual ~CtrlStatement()
    {
      delete condition;
      delete body;
    }

    void printContents(ostream &out, int i)
    {
      printMember(out, "condition", condition, i);
      printMember(out, "body", body, i);
    }
  };

  class Typedef : public Statement
  {
  public:
    string name;
    Type *type;

    Typedef(Type* type, string name) : type(type), name(name) {}

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "type", type, i);
      printMember(out, "name", name, i);
    }

    virtual inline NodeType getType() const
    {
      return TYPEDEF;
    }
  };

  class LoopStatement : public CtrlStatement {
  public:
    LoopStatement(Expression *condition, Statement *body): CtrlStatement(condition, body)
    {}
  };

  class ForStatement : public LoopStatement
  {
    Expression *init;
    Expression *loop;

  public:
    ForStatement(Expression *init, Expression *condition, Expression *loop, Statement *body) :
      LoopStatement(condition, body)
    {
      this->init = init;
      this->loop = loop;
    }
    virtual ~ForStatement()
    {
      delete init;
      delete loop;
    }

    virtual inline NodeType getType() const
    {
      return FOR_STATEMENT;
    }

    void printContents(ostream &out, int i)
    {
      CtrlStatement::printContents(out, i);
      printMember(out, "init", init, i);
      printMember(out, "loop", loop, i);
    }
  };

  // no difference in these other than the way
  // they're printed.
  class DoWhileStatement : public LoopStatement {
    virtual inline NodeType getType() const
    {
      return DO_WHILE_STATEMENT;
    }
  };

  class WhileStatement : public LoopStatement
  {
  public:
    WhileStatement(Expression *condition, Statement *body) :
      LoopStatement(condition, body) {}

    virtual inline NodeType getType() const
    {
      return WHILE_STATEMENT;
    }
  };

  class IfStatement : public CtrlStatement
  {
    Statement *elseBody;
  public:
    IfStatement(Expression *condition, Statement *body, Statement *elseBody) :
      CtrlStatement(condition, body)
    {
      this->elseBody = elseBody;
    }
    virtual ~IfStatement()
    {
      delete elseBody;
    }

    virtual inline NodeType getType() const
    {
      return IF_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
      CtrlStatement::printContents(out, i);
      printMember(out, "elseBody", elseBody, i);
    }
  };


  class Declarator : public Statement
  {
    Variable* variable;
    Expression* init;

  public:
    Declarator(Variable* variable, Expression* init) : variable(variable), init(init)
    {}

    virtual ~Declarator()
    {
      delete variable;
      delete init;
    }

    virtual inline NodeType getType() const
    {
      return DECLARATOR;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "variable", variable, i);
      printMember(out, "init", init, i);
    }
  };

  class Declaration : public Statement
  {
    List<Declarator *> *declarations;

  public:
    Declaration(List<Declarator *> *declarations)
    {
      this->declarations = declarations;
    }
    virtual ~Declaration()
    {
      delete declarations;
    }

    virtual inline NodeType getType() const
    {
      return DECLARATION;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "declarations", declarations, i);
    }
  };

  class BlockStatement : public Statement
  {
    List<Statement *> *statements;

  public:
    BlockStatement(List<Statement *> *statements)
    {
      this->statements = statements;
    }
    virtual ~BlockStatement() {
      //delete statements;
    }

    virtual inline NodeType getType() const
    {
      return BLOCK_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "statements", statements, i);
    }
  };

  class ExpressionStatement : public Statement
  {
    Expression *expression;

  public:
    ExpressionStatement(Expression *expression)
    {
      this->expression = expression;
    }
    virtual ~ExpressionStatement()
    {
      //delete expression;
    }

    virtual inline NodeType getType() const
    {
      return EXPRESSION_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "expression", expression, i);
    }
  };

  class ReturnStatement : public Statement
  {
    Expression *expression;

  public:
    ReturnStatement(Expression *expression)
    {
      this->expression = expression;
    }
    virtual ~ReturnStatement()
    {
      delete expression;
    }

    virtual inline NodeType getType() const
    {
      return RETURN_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "expression", expression, i);
    }
  };

  class Argument : public Variable
  {
  public:
    Argument(string name, Type* type) : Variable(name, type)
    {}

    virtual inline NodeType getType() const
    {
      return ARGUMENT;
    }
  };

  class ArgumentList : public List<Argument *>{
  public:
    bool variable;
    ArgumentList(bool variable) : variable(variable)
    {}
  };

  class FunctionPrototype : public Statement {
  public:
    Type *returnType;
    string name;
    ArgumentList* arguments;

    typedef ArgumentList::iterator ArgumentIterator;

    FunctionPrototype(
        Type* returnType,
        string name,
        ArgumentList* arguments
      )
    {
      this->returnType = returnType;
      this->name = name;
      this->arguments = arguments;
    }
    virtual ~FunctionPrototype()
    {
      delete returnType;
      delete arguments;
    }

    virtual inline NodeType getType() const
    {
      return FUNCTION_PROTOTYPE;
    }

    bool matches(FunctionPrototype* proto)
    {
      ArgumentList* other = proto->arguments;

      if(arguments->size() == other->size())
      {
        bool match = true;

        ArgumentIterator otherArg = other->begin();
        ArgumentIterator thisArg = arguments->begin();

        while(match && otherArg != other->end() && thisArg != arguments->end())
        {
          match = (*otherArg)->type->matches((*thisArg)->type);
          otherArg++;
          thisArg++;
        }
        return match;
      }

      return false;
    }

    void printContents(ostream &out, int i)
    {
      printMember(out, "returnType", returnType, i);
      printMember(out, "name", name, i);
      printMember(out, "arguments", arguments, i);
    }
  };

  class FunctionDeclaration : public Statement {
  public:
    FunctionPrototype *prototype;
    BlockStatement *body;

    FunctionDeclaration(
        FunctionPrototype *prototype,
        BlockStatement *body
      )
    {
      this->prototype = prototype;
      this->body = body;
    }
    virtual ~FunctionDeclaration()
    {
      delete prototype;
      delete body;
    }

    virtual inline NodeType getType() const
    {
      return FUNCTION_DECLARATION;
    }

    void printContents(ostream &out, int i)
    {
      printMember(out, "prototype", prototype, i);
      printMember(out, "body", body, i);
    }

  };

  class FunctionInvocation : public Expression
  {
    FunctionPrototype *function;
    List<Expression *> *arguments;
  public:
    FunctionInvocation(FunctionPrototype *function, List<Expression *> *arguments)
    {
      this->arguments = arguments;
      this->function = function;
    }
    virtual ~FunctionInvocation()
    {
      delete function;
      delete arguments;
    }

    virtual inline NodeType getType() const
    {
      return FUNCTION_INVOCATION;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "arguments", arguments, i);
      if(function)
      {
        printMember(out, "function", function->name, i);
      }
    }
  };

  class MemberExpression : public Assignable
  {
    Expression *object;
    string member;

  public:
    MemberExpression(Expression *object, string member)
    {
      this->object = object;
      this->member = member;
    }
    virtual ~MemberExpression()
    {
      //delete object;
    }

    virtual inline NodeType getType() const
    {
      return MEMBER_EXPRESSION;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "object", object, i);
      printMember(out, "member", member, i);
    }
  };

  class ArrayAccess : public Assignable
  {
    Expression *array;
    Expression *index;

  public:
    ArrayAccess(Expression* array, Expression* index)
    {
      this->array = array;
      this->index = index;
    }
    virtual ~ArrayAccess()
    {
      delete array;
      delete index;
    }

    virtual inline NodeType getType() const
    {
      return ARRAY_ACCESS;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "array", array, i);
      printMember(out, "index", index, i);
    }
  };


  class EnumStatement : public Statement
  {
    string name;
  public:
    EnumStatement(string name)
    {
      this->name = name;
    }

    virtual inline NodeType getType() const
    {
      return ENUM_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
    }
  };

  class BreakStatement : public Statement
  {
    virtual string getTypeString()
    {
      return "BreakStatement";
    }
  };


  class ContinueStatement : public Statement
  {
    virtual inline NodeType getType() const
    {
      return CONTINUE_STATEMENT;
    }
  };

  class SwitchStatement : public Statement
  {
    Expression *variable;

  public:
    SwitchStatement(Expression *variable)
    {
      this->variable = variable;
    }
    virtual ~SwitchStatement()
    {
      delete variable;
    }

    virtual inline NodeType getType() const
    {
      return SWITCH_STATEMENT;
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "variable", variable, i);
    }

  };

};

#endif
