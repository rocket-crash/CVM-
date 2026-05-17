#include "virtualmechine.h"
#include "lexer.h"
#include "parsing.h"
#include "bytecode.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int main() {
  try{
  std::ifstream file("input.txt");
  if(!file){std::cout<<"file not found"<<std::endl;return 0;}
  std::stringstream buffer;
  buffer<<file.rdbuf();
  std::string code=buffer.str();
  // std::cout<<code<<std::endl;
  Lexer lexer(code);
  Parser parser(lexer);
  auto ast = parser.parse();
  std::cout << "AST below- " << std::endl;
  ast->print();
  Compiler compiler;
  compiler.compile(*ast);
//   compiler.disassemble();

  std::cout << "VM output- " << std::endl;
  std::cout<<"Enter the value of a and b as input"<<std::endl;
  VM vm(compiler.getCode(), compiler.getNames());
  vm.run();
  return 0;
  }
  catch (const std::exception& e) {
        std::cerr << "Execution Error: " << e.what() << std::endl;
    }
}