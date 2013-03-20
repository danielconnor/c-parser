#ifndef _SCOPE
#define _SCOPE

#include <map>
#include <string>
#include <vector>

#include "ast.h"

using namespace std;

class Scope {
protected:
  Scope* parent;
  vector<Scope*> children;
  map<string, Ast::Node*> identifiers;

  bool _declareIdentifier(const string&, Ast::Node*);

public:
  Scope()
  {
    Scope(NULL);
  }

  Scope::Scope(Scope* parentScope) : parent(parentScope)
  {}

  bool isGlobal();
  Scope* createChildScope();
  Scope* closeScope();

  // variable
  bool declareIdentifier(const string&, Ast::Variable*);
  // struct
  bool declareIdentifier(const string&, Ast::Struct*);
  // union
  bool declareIdentifier(const string&, Ast::Union*);

  Ast::NodeType lookupIdentifier(const string&, Ast::Node*&);
  Ast::NodeType lookupIdentifier(const string&);
};

class GlobalScope : public Scope {

public:
  GlobalScope::GlobalScope() : Scope(NULL)
  {}

  // The following identifiers types are only allowed to
  // be defined within the global scope.
  // typedef
  bool declareIdentifier(const string&, Ast::Typedef*);
  // functions
  bool declareIdentifier(const string&, Ast::FunctionPrototype*);
};

#endif
