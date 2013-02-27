UI.Literal = (function() {

  function Literal(name, val) {
    UI.TreeItem.call(this, "li", name, val, "literal");
  }

  util.inherits(Literal, UI.TreeItem);

  var _proto = Literal.prototype,
    _super = UI.TreeItem.prototype;

  return Literal;
})();

