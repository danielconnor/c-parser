UI.TreeList = (function() {

  function TreeList(obj) {
    UI.Control.call(this, "ul", "list");

    if(obj && typeof obj === "object") {

      var keys = Object.keys(obj);

      for(var i = 0, il = keys.length; i < il; i++) {

        var key = keys[i],
          val = obj[key];

        if(key === "typeName") continue;

        if(typeof val == "object") {
          this.append(new UI.Tree(key, val));
        }
        else {
          this.append(new UI.Literal(key, val));
        }
      }
    }
  }

  util.inherits(TreeList, UI.Control);

  var _proto = TreeList.prototype,
    _super = UI.Control.prototype;

  return TreeList;
})();

