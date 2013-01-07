/*global util, EventEmitter, Node, Element*/
var DOMElement = (function() {
  "use strict";

  var last_id = 0;

  function DOMElement(ns, tagName) {
    EventEmitter.call(this);

    this.children = [];

    this.element = document.createElementNS(ns, tagName || "div");
    this.style = this.element.style;
    this.element._domElement = this;
  }

  util.inherits(DOMElement, EventEmitter);

  var _proto = DOMElement.prototype,
    _super = EventEmitter.prototype;

  _proto.append = function(element) {
    if(arguments.length > 1) {
      for(var i = 0, il = arguments.length; i < il; i++) {
        this.append(arguments[i]);
      }
    }
    else if(element instanceof DOMElement) {
      this.children.push(element);
      this.element.appendChild(element.element);
      return element;
    }
    else if(typeof element === "string") {
      this.element.appendChild(document.createTextNode(element));
    }
    else {
      this.element.appendChild(element);
    }
  };
  _proto.prepend = function(element) {
    this.children.unshift(element);
    this.element.insertBefore(element.element, this.element.firstChild);
  };

  _proto.hide = function() {
    this.element.style.display = "none";
  };

  _proto.show = function() {
    this.element.style.display = "block";
  };

  _proto.remove = function(element) {
    this.children.splice(this.children.indexOf(element), 1);
    this.element.removeChild(element.element);
    return element;
  };

  _proto.removeSelf = function() {
    var element = this.element,
      parent = element.parentNode;
    if(parent) parent.removeChild(element);
  };

  _proto.setAttribute =
  _proto.setAttr = function(namespace, name, val) {
    if(!val) {
      this.element.setAttribute(namespace, name);
    }
    else {
      this.element.setAttributeNS(namespace, name, val);
    }
  };

  _proto.setAttrs = function(attrs) {
    var element = this.element;
    for(var name in attrs) {
      if(attrs.hasOwnProperty(name)) {
        element.setAttribute(name, attrs[name]);
      }
    }
  };

  _proto.getAttribute =
  _proto.getAttr = function(name) {
    return this.element.getAttributeNS(null, name);
  };
  
  _proto.removeAttribute =
  _proto.removeAttr = function(name) {
    return this.element.removeAttribute(name);
  };

  // there's no point in giving every element an id when generated, so just create one
  // when requested or return the one it already has
  Object.defineProperty(_proto, "id", {
    get: function() {
      var id = this.getAttr("id");
      if(!id) {
        id = this.id = "0-_-0" + (++last_id) + "0-_-0";
      }
      return id;
    },
    set: function(id) {
      this.setAttr("id", id);
    }
  });

  _proto.clone = function(deep) {
    var element = this.element,
      attrs = element.attributes,
      cloned = new DOMElement(element.namespaceURI, element.tagName),
      i,
      il;

    for(i = 0, il = attrs.length; i < il; i++) {
      var attr = attrs[i];
      cloned.setAttr(attr.name, attr.value);
    }

    if(deep) {
      var children = this.children;

      for(i = 0, il = children.length; i < il; i++) {
        cloned.append(children[i].clone(true));
      }
    }

    // the clone event gives a listener an oportunity to
    // add stuff to the cloned element
    this.emit("clone", cloned);

    return cloned;
  };

  _proto.addEventListener = function() {
    if(arguments.length === 3 && "on" + arguments[0] in this.element) {
      return Element.prototype.addEventListener.apply(this.element, arguments);
    }
    else {
      return _super.addEventListener.apply(this, arguments);
    }
  };

  Object.defineProperty(_proto, "outerHTML", {
    get: function() {
      var el = this.element;

      // SVGElements don't have an outerHTML property
      if(el.outerHTML === undefined) {
        var temp = document.createElement("div");
        temp.appendChild(el.cloneNode(true));
        return temp.innerHTML;
      }
      else {
        return el.outerHTML;
      }
    }
  });

  Object.defineProperty(_proto, "innerHTML", {
      get: function() {
        return this.element.innerHTML;
      },
      set: function(html) {
        this.element.innerHTML = html;
      }
  });

  Object.defineProperty(_proto, "textContent", {
      get: function() {
        return this.element.textContent;
      },
      set: function(text) {
        this.element.textContent = text;
      }
  });

  return DOMElement;
})();