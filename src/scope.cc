#include "scope.h"

using namespace std;

bool Scope::isGlobal()
{
  return parent == NULL;
}

bool Scope::_declareIdentifier(const string& name, Ast::Node *val)
{
  if(identifiers[name] == NULL)
  {
	identifiers[name] = val;
	return true;
  }
  return false;
}
bool Scope::declareIdentifier(const string& name, Ast::Variable* var)
{
  return _declareIdentifier(name, (Ast::Node*)var);
}
bool Scope::declareIdentifier(const string& name, Ast::Struct* s)
{
  return _declareIdentifier(name, (Ast::Node*)s);
}
bool Scope::declareIdentifier(const string& name, Ast::Union* u)
{
  return _declareIdentifier(name, (Ast::Node*)u);
}
Ast::NodeType Scope::lookupIdentifier(const string& name, Ast::Node*& node)
{
  node = identifiers[name];
  if(node)
  {
    return node->getType();
  }
  else if (parent)
  {
    return parent->lookupIdentifier(name, node);
  }
  return Ast::NONE;
}
Ast::NodeType Scope::lookupIdentifier(const string& name)
{
  Ast::Node* node;
  return lookupIdentifier(name, node);
}

Scope* Scope::createChildScope()
{
  Scope *s = new Scope(this);
  children.push_back(s);
  return s;
}
Scope* Scope::closeScope()
{
	return parent;
}
bool GlobalScope::declareIdentifier(const string& name, Ast::Typedef* def)
{
  return _declareIdentifier(name, (Ast::Node*)def);
}
bool GlobalScope::declareIdentifier(const string& name, Ast::FunctionPrototype* func)
{
  return _declareIdentifier(name, (Ast::Node*)func);
}





