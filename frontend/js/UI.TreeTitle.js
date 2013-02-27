UI.TreeTitle = (function() {

  function TreeTitle(name, val) {
    UI.Control.call(this, "h3", "title");

    var right;

    if(val && typeof val == "object") {
      this.classList.add("expandable");

      if(val instanceof Array) {
        right = "List";
      }
      else {
        right = val.typeName;
      }
    }
    else if(typeof val == "string") {
      right = "\"" + val + "\"";
    }
    else {
      right = val;
    }

    this.innerHTML = "<span class='name'>" + name + "</span>: <span class='value'>" + right + "</span>";
  }

  util.inherits(TreeTitle, UI.Control);

  var _proto = TreeTitle.prototype,
    _super = UI.Control.prototype;

  _proto.open = function() {
    this.classList.add("open");
  };
  _proto.close = function() {
    this.classList.remove("open");
  };

  return TreeTitle;
})();

