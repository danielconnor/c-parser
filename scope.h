#ifndef _SCOPE
#define _SCOPE

#include <map>
#include <string>
#include <vector>

#include "ast.h"

using namespace std;

class Scope {

public:
  Scope *parent;
  vector<Scope *> children;
  map<string, Ast::Type *> variables;

  Scope();
  Scope(Scope*);

  bool isGlobal();
  void declareVar(string, Ast::Type *);
  Ast::Type *lookupVar(string);
  Scope* createChildScope();

};

class GlobalScope : public Scope {

  map<string, Ast::Function *> functions;

public:
  GlobalScope();

  void declareFunc(string, Ast::Function *);
  Ast::Function *lookupFunc(string);
};

#endif
