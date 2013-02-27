#include "scope.h"

using namespace std;

Scope::Scope(Scope* parentScope)
{
  parent = parentScope;
}

Scope::Scope()
{
  Scope(NULL);
}

bool Scope::isGlobal()
{
  return parent == NULL;
}

void Scope::declareVar(string name, Ast::Type *val)
{
  variables[name] = val;
}

Ast::Type *Scope::lookupVar(string name)
{
  Ast::Type *type = variables[name];
  if(type)
  {
    return type;
  }
  else if (parent)
  {
    return parent->lookupVar(name);
  }
  return NULL;
}

Scope *Scope::createChildScope()
{
  Scope *s = new Scope(this);
  children.push_back(s);
  return s;
}


GlobalScope::GlobalScope() : Scope(NULL)
{
}

void GlobalScope::declareFunc(string name, Ast::FunctionPrototype *func)
{
  functions[name] = func;
}

Ast::FunctionPrototype *GlobalScope::lookupFunc(string name)
{
  Ast::FunctionPrototype *func = functions[name];

  return functions[name];
}
