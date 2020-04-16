#include <assert.h>
#include <stdio.h>
#include <string.h>

// this should be run from the build/mono folder
const char *outputFile = "../../core/includes/binder/autogen/astgen.h";
const char *fileHeader =
    "THIS IS AN AUTOGENERATED FILE FROM THE METACOMPILER DO NOT MODIFY!\n";
const char *version = "Metacompiler for \"TheBinder\" language v0.0.1\n";
const char *BASE_CLASS_NAME = "Expr";

struct ASTNodeDefinition {
  const char *className;
  const char *members;
  ;
};

const ASTNodeDefinition definitions[] = {
    {"Binary", "Expr* left, TOKEN_TYPE op, Expr* right"},
    {"Grouping", "Expr* expr"},
    {"Literal", "TOKEN_TYPE type, const char* value"},
    {"Unary", "TOKEN_TYPE op, Expr* right"},
};

void writeHeader(FILE *fp) {
  fprintf(fp, "/*\n");
  fprintf(fp, fileHeader);
  fprintf(fp, version);
  fprintf(fp, "*/\n\n");
}

void writeIncludes(FILE *fp) {
  fprintf(fp, "#include \"binder/tokens.h\" \n\n");
}

void WriteASTNode(FILE *fp, const ASTNodeDefinition &definition) {
  fprintf(fp, "class ");
  fprintf(fp, definition.className);
  fprintf(fp, " : public ");
  fprintf(fp, BASE_CLASS_NAME);
  fprintf(fp, "\n{\npublic:\n\t");
  fprintf(fp, definition.className);
  fprintf(fp, "():Expr(){}\n\tvirtual ~");
  fprintf(fp, definition.className);
  fprintf(fp, "()=default;\n");

  // this might not be the fastest code but is by far the simpler
  const char *source = definition.members;
  int len = strlen(source);
  for (int i = 0; i < len; ++i) {
    // if we have a space after the comma we skip it
    if (source[i] == ' ') {
      ++i;
    }
    // adding indentation
    fputc('\t', fp);
    // parsing new member
    while (source[i] != ',' && i < len) {
      fputc(source[i], fp);
      ++i;
    }

    // closing new member
    fprintf(fp, ";\n");
  }
  // adding the interface
  // accept function
  fprintf(fp, "\tvoid* accept(Visitor* visitor) override\n"
              "\t{ \n \t\treturn visitor->accept");
  fprintf(fp, definition.className);
  fprintf(fp, "(this);\n\n\t};\n");

  // closing class definition
  fprintf(fp, "};\n\n");
}

void generateASTClasses(FILE *fp) {
  int count = sizeof(definitions) / sizeof(definitions[0]);
  for (int i = 0; i < count; ++i) {
    WriteASTNode(fp, definitions[i]);
  }
}

void openNamespace(FILE *fp) { fprintf(fp, "namespace binder::autogen{\n\n"); }

void closeNamespace(FILE *fp) {
  fprintf(fp, "\n}// namespace binder::autogen");
}

void generateExpressionBaseClass(FILE *fp) {
  fprintf(fp,
          "class Expr {\n public:\n\tExpr() = default;\n"
          "\tvirtual ~Expr()=default;\n\t //interface\n"
          "\tvirtual void* accept(Visitor* visitor)=0;\n};\n\n");
}

void generateVisitorBaseClass(FILE *fp) {
  // forward declare of the expr class
  fprintf(fp, "class Expr;\n\n");
  //generate a forward delcare for each class 
  int count = sizeof(definitions) / sizeof(definitions[0]);
  for (int i = 0; i < count; ++i) {
    fprintf(fp, "class ");
    fprintf(fp, definitions[i].className);
    fprintf(fp, ";\n");
  }


  // declaring the visitor interface, here the class and constructor destructor
  fprintf(fp, "\nclass Visitor{\n public:\n\tVisitor() "
              "= default;\n"
              "\tvirtual ~Visitor()=default;\n\t//interface\n");

  //here we loop all the class the visitor needs to be able to accept
  //and generate an accept method for the specific class, we use the 
  //specific name to not get too crazy with overload, but probably 
  //overload would make the code a bit clearer? not sure
  for (int i = 0; i < count; ++i) {
    fprintf(fp, "\tvirtual void* accept");
    fprintf(fp, definitions[i].className);
    fprintf(fp, "(");
    fprintf(fp, definitions[i].className);
    fprintf(fp, "* expr) = 0;\n");
  }
  // closing the class
  fprintf(fp, "\n};\n");
}

int main() {
  FILE *fp = fopen(outputFile, "w");
  assert(fp != nullptr);

  //writing header guard
  fprintf(fp, "#pragma once \n");
  writeHeader(fp);
  writeIncludes(fp);
  openNamespace(fp);
  generateVisitorBaseClass(fp);
  generateExpressionBaseClass(fp);
  generateASTClasses(fp);
  closeNamespace(fp);
  fclose(fp);

  return 0;
}
