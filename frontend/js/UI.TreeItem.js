UI.TreeItem = (function() {

  function TreeItem(tagName, name, val, classes) {
    UI.Control.call(this, tagName, classes || "item");

    this.title = new UI.TreeTitle(name, val);

    this.append(this.title);
  }

  util.inherits(TreeItem, UI.Control);

  var _proto = TreeItem.prototype,
    _super = UI.Control.prototype;

  return TreeItem;
})();

