function addCallbacksToFoldables() {
  var toggler = document.getElementsByClassName("caret");
  var i;

  for (i = 0; i < toggler.length; i++) {
    toggler[i].addEventListener(
        "click", function() {
          this.parentElement.querySelector(".nested").classList.toggle(
              "active");
          this.classList.toggle("caret-down");
        });
  }
}

function showAST(AST) {
  var element = document.getElementById('AST');
  // clearing
  element.innerHTML = "";
  // lets build the AST
  var j = JSON.parse(AST);

  var ul = document.createElement('ul');
  ul.setAttribute('id', 'astUL');
  element.appendChild(ul);
  processASTNode(ul, j);
  addCallbacksToFoldables();
}
function createFoldableNode(parentNode, name) {
  var typeli = document.createElement('li');
  var span = document.createElement('span');
  span.setAttribute("class", "caret");
  span.innerHTML = name;
  typeli.appendChild(span);
  parentNode.appendChild(typeli);
  return typeli;
}

function processASTNode(parentNode, node) {
  // create foldable container
  var typeli = createFoldableNode(parentNode, node.type);

  // start appending to it
  var typeBody = document.createElement('ul');
  typeBody.setAttribute("class", "nested");
  typeli.appendChild(typeBody);

  if (node.op) {
    var opli = document.createElement('li');
    opli.innerHTML = "operator : " + node.op;
    typeBody.appendChild(opli);
  }

  if (node.value) {
    var valueli = document.createElement('li');
    valueli.innerHTML = "value: " + node.value;
    typeBody.appendChild(valueli);
  }

  if (node.left) {
    processASTNode(typeBody, node.left);
  }
  if (node.right) {
    processASTNode(typeBody, node.right);
  }
}

