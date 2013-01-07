#ifndef AST_
#define AST_

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <ostream>

#include "token.h"

using namespace std;

class Ast {

public:

  /**
  * Super class for all ast types to allow them to
  * be printed to JSON
  */
  class Printable {
  public:
    int c;

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

    // get's the type's name
    virtual string getTypeString() { return ""; }

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

    void printVar(const char *filename, string var)
    {
      ofstream out;
      out.open(filename);

      out << "var " << var << " = ";
      print(out);
      out << ";";

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
      for(int i = 0; i < this->size(); i++)
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

  };


  class Type : public Printable {

    short unsigned type;

  public:

    const static char *types[];

    Type(short unsigned type)
    {
      this->type = type;
    }

    virtual void printContents(ostream &out, int i)
    {
      string name = "";
      unsigned short mask = 1;

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

    virtual string getTypeString()
    {
      return "Type";
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
  //   {
  //     start
  //   }

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

    virtual string getTypeString()
    {
      return "Literal";
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

    virtual string getTypeString()
    {
      return "NumberLiteral";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "raw", raw, i);
      printMember(out, "value", value.intVal, i);
    }
  };


  class Assignable : public Expression {};


  class Variable : public Assignable
  {
  public:
    string name;
    Type *type;

    Variable(string name, Type *type)
    {
      this->name = name;
      this->type = type;
    }
    ~Variable()
    {
      delete type;
    }

    virtual string getTypeString()
    {
      return "VariableExpression";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "raw", name, i);
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
    ~AssignExpression()
    {
      delete left;
      delete right;
    }

    virtual string getTypeString()
    {
      return "AssignExpression";
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

    ~UnaryExpression()
    {
      delete operand;
    }

    virtual string getTypeString()
    {
      return "UnaryExpression";
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

    virtual string getTypeString()
    {
      return "UpdateExpression";
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

    ~BinaryExpression()
    {
      delete left;
      delete right;
    }

    string getTypeString()
    {
      return "BinaryExpression";
    }

    virtual void printContents(ostream &out, int i)
    {
      OpExpression::printContents(out, i);
      printMember(out, "left", left, i);
      printMember(out, "right", right, i);
    }
  };

  class Argument : public Node
  {
    Type *type;
    string name;

  public:
    Argument(Type *type, string name)
    {
      this->type = type;
      this->name = name;
    }

    string getTypeString()
    {
      return "Argument";
    }

    void printContents(ostream &out, int i)
    {
      printMember(out, "name", name, i);
      printMember(out, "type", type, i);
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

    ~CtrlStatement()
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
    ~ForStatement()
    {
      delete init;
      delete loop;
    }

    string getTypeString()
    {
      return "ForStatement";
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
    virtual string getTypeString()
    {
      return "DoWhileStatement";
    }
  };
  class WhileStatement : public LoopStatement
  {
  public:
    WhileStatement(Expression *condition, Statement *body) :
      LoopStatement(condition, body) {}

    virtual string getTypeString()
    {
      return "WhileStatement";
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
    ~IfStatement()
    {
      delete elseBody;
    }

    virtual string getTypeString()
    {
      return "IfStatement";
    }

    virtual void printContents(ostream &out, int i)
    {
      CtrlStatement::printContents(out, i);
      printMember(out, "elseBody", elseBody, i);
    }
  };


  class Declarator : public Statement
  {
    string name;
    Expression *init;

  public:
    Declarator(string name, Expression *init)
    {
      this->name = name;
      this->init = init;
    }
    ~Declarator()
    {
      delete init;
    }

    virtual string getTypeString()
    {
      return "Declarator";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "name", name, i);
      printMember(out, "init", init, i);
    }
  };

  class Declaration : public Statement
  {
    Type *type;
    List<Declarator *> *declarations;

  public:
    Declaration(Type *type, List<Declarator *> *declarations)
    {
      this->type = type;
      this->declarations = declarations;
    }
    ~Declaration()
    {
      declarations->clear();
      delete type;
      delete declarations;
    }

    virtual string getTypeString()
    {
      return "Declaration";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "type", type, i);
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
    ~BlockStatement() {
      statements->clear();
      delete statements;
    }

    virtual string getTypeString()
    {
      return "BlockStatement";
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
    ~ExpressionStatement()
    {
      delete expression;
    }

    virtual string getTypeString()
    {
      return "ExpressionStatement";
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
    ~ReturnStatement()
    {
      delete expression;
    }

    virtual string getTypeString()
    {
      return "ReturnStatement";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "expression", expression, i);
    }
  };

  class Function : public Statement {
  public:
    Type *returnType;
    string name;
    List<Argument *> *arguments;
    BlockStatement *body;

    Function(
        Type *returnType,
        string name,
        List<Argument *> *arguments,
        BlockStatement *body
      )
    {
      this->returnType = returnType;
      this->name = name;
      this->arguments = arguments;
      this->body = body;
    }
    ~Function()
    {
      arguments->clear();
      delete returnType;
      delete arguments;
      delete body;
    }

    virtual string getTypeString()
    {
      return "Function";
    }

    bool isPrototype()
    {
      return body == NULL;
    }

    void printContents(ostream &out, int i)
    {
      printMember(out, "returnType", returnType, i);
      printMember(out, "name", name, i);
      printMember(out, "arguments", arguments, i);
      printMember(out, "body", body, i);
    }

  };

  class FunctionInvocation : public Expression
  {
    Function *function;
    List<Expression *> *arguments;
  public:
    FunctionInvocation(Function *function, List<Expression *> *arguments)
    {
      this->arguments = arguments;
      this->function = function;
    }
    ~FunctionInvocation()
    {
      arguments->clear();
      delete function;
      delete arguments;
    }

    virtual string getTypeString()
    {
      return "FunctionInvocation";
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
    ~MemberExpression()
    {
      delete object;
    }

    virtual string getTypeString()
    {
      return "MemberExpression";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "object", object, i);
      printMember(out, "member", member, i);
    }
  };

  class StructStatement : public Statement
  {
    string name;
    List<Declaration *> *members;

  public:
    StructStatement(string name, List<Declaration *> *members)
    {
      this->name = name;
      this->members = members;
    }
    ~StructStatement()
    {
      members->clear();
      delete members;
    }

    virtual string getTypeString()
    {
      return "StructStatement";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out,"name", name, i);
      printMember(out, "members", members, i);
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

    virtual string getTypeString()
    {
      return "EnumStatement";
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
    virtual string getTypeString()
    {
      return "ContinueStatement";
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
    ~SwitchStatement()
    {
      delete variable;
    }

    virtual string getTypeString()
    {
      return "SwitchStatement";
    }

    virtual void printContents(ostream &out, int i)
    {
      printMember(out, "variable", variable, i);
    }

  };

};

#endif
