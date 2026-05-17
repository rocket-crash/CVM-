#include "compiler.h"
#include "lexer.h"
#include "parsing.h"
#include "bytecode.h"
#include <iostream>
#include <string>

int main() {
  // ── Test: arithmetic + while loop + if/else + print ──
  std::string code = "x = 0;"
                     "while (x < 5) {"
                     "    x = x + 1;"
                     "}"
                     "print(x);" // should print 5
                     "y = 2 + 3 * 4;"
                     "print(y);" // should print 14
                     "if (y > 10) {"
                     "    print(1);" // should print 1
                     "} else {"
                     "    print(0);"
                     "}"
                     "x=x+10;"
                     "print(x)";
  // 1. Lex & Parse
  Lexer lexer(code);
  Parser parser(lexer);
  auto ast = parser.parse();

  std::cout << "===== AST =====" << std::endl;
  ast->print();

  // 2. Compile AST → Bytecode
  Compiler compiler;
  compiler.compile(*ast);
  compiler.disassemble();

  // // 3. Execute Bytecode on the VM
  std::cout << "===== VM Output =====" << std::endl;
  VM vm(compiler.getCode(), compiler.getNames());
  vm.run();

  return 0;
}