#include <assert.h>
#include <stdio.h>
#include <string.h>

// this should be run from the build/mono folder
const char *outputFile = "../../core/src/autogen/astgen.cpp";
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
    {"Binary", "Expr* left, TOKEN_TYPE operator, Expr* right"},
    {"Grouping", "Expr* expr"},
    {"Literal", "const char* value"},
    {"Unary", "TOKEN_TYPE operator, Expr* right"},
};

void writeHeader(FILE *fp) {
  fprintf(fp, "/*\n");
  fprintf(fp, fileHeader);
  fprintf(fp, version);
  fprintf(fp, "*\\\n\n");
}

void writeIncludes(FILE *fp) {
  fprintf(fp, "#include \"binder/expression.h\" \n\n");
}

void WriteASTNode(FILE *fp, const ASTNodeDefinition &definition) {
  fprintf(fp, "class ");
  fprintf(fp, definition.className);
  fprintf(fp, " : ");
  fprintf(fp, BASE_CLASS_NAME);
  fprintf(fp, "\n{\npublic:\n");

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
  fprintf(fp, "\tExpr* accept(Visitor* visitor) override\n"
              "\t{ \n \t\tvisitor->accept");
  fprintf(fp, definition.className);
  fprintf(fp, "(this);\n \t\treturn this;\n\t};\n");

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

int main() {
  FILE *fp = fopen(outputFile, "w");
  assert(fp != nullptr);

  writeHeader(fp);
  writeIncludes(fp);
  openNamespace(fp);
  generateASTClasses(fp);
  closeNamespace(fp);
  fclose(fp);

  return 0;
}
