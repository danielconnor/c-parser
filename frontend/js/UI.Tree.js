UI.Tree = (function() {

  function Tree(name, obj) {
    UI.TreeItem.call(this, "li", name, obj, "tree");

    this.isOpen = false;

    this.list = new UI.TreeList(obj);

    this.append(this.list);

    this.title.addEventListener("click", this.toggleOpen.bind(this), false);
  }

  util.inherits(Tree, UI.TreeItem);

  var _proto = Tree.prototype,
    _super = UI.TreeItem.prototype;

  _proto.toggleOpen = function() {
    if(this.isOpen) {
      this.close();
    }
    else {
      this.open();
    }
  };

  _proto.open = function() {
    this.classList.add("open");
    this.isOpen = true;
  };
  _proto.close = function() {
    this.classList.remove("open");
    this.isOpen = false;
  };

  return Tree;
})();

