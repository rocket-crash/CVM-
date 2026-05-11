#include <iostream>
#include "lexer.h"
#include "parsing.h"
#include <string>
int main(){
    std::string code="10+20*3;";
    Lexer lexer(code);
    // Token token;
    // do {
    //     token = lexer.getNextToken();
    //     std::cout<< "Type:"<< static_cast<int>(token.type)<< " Value:"<< token.value<< std::endl;
    // } while (token.type != TokenType::EndOfFile);
    // return 0;
    Parser parser(lexer);
    auto ast=parser.parse();
    ast-> print();
    return 0;
}