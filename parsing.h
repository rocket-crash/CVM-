#pragma once
#include<memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include "lexer.h"
#include <vector>
struct Expr {
    virtual ~Expr() = default;
    //print
    virtual void print(int indent = 0) const = 0;
};
struct NumberExpr : Expr {
    int value;

    NumberExpr(int val): value(val) {}
    //for print
    void print(int indent = 0) const override {
    for (int i = 0; i < indent; i++)std::cout << "  ";
    std::cout << value << std::endl;
    }
};
struct BinaryExpr : Expr {
    char op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    BinaryExpr(
        char oper,
        std::unique_ptr<Expr> lhs,
        std::unique_ptr<Expr> rhs
    )
        : op(oper),
          left(std::move(lhs)),
          right(std::move(rhs)) {}
    //for print
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << op << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
};
struct VariableExpr : Expr {
    std::string name;
    VariableExpr(const std::string& n)
        : name(n) {}
    //print
    void print(int indent = 0) const override {
        for(int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << name << std::endl;
    }
};
struct AssignExpr : Expr {
    std::string name;
    std::unique_ptr<Expr> value;
    AssignExpr(
        const std::string& n,
        std::unique_ptr<Expr> val
    )
        : name(n),
          value(std::move(val)) {}
    //print
    void print(int indent = 0) const override {
    for (int i = 0; i < indent; i++)std::cout << "  ";
    std::cout << "=" << std::endl;
    for (int i = 0; i < indent + 1; i++)std::cout << "  ";
    std::cout << name << std::endl;
    value->print(indent + 1);
}
};
struct BlockExpr :Expr{
    std::vector<std::unique_ptr<Expr>>statements;
    //print
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << "BLOCK" << std::endl;
        for (const auto& stmt : statements) {stmt->print(indent + 1);}
    }
};
struct IfExpr :Expr{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> thenBranch;
    std::unique_ptr<Expr> elseBranch;
    IfExpr(
        std::unique_ptr<Expr> cond,
        std::unique_ptr<Expr> thenBr,
        std::unique_ptr<Expr> elseBr
    )
    :condition(std::move(cond)),thenBranch(std::move(thenBr)),elseBranch(std::move(elseBr)) {}
    //print
    void print(int indent = 0) const override {
    for (int i = 0; i < indent; i++)std::cout << "  ";
    std::cout << "IF" << std::endl;
    condition->print(indent + 1);
    thenBranch->print(indent + 1);
    if(elseBranch){
        for(int i=0;i<indent;i++)std::cout<<"  ";
        std::cout<<"Else"<<std::endl;
        elseBranch->print(indent+1);
    }
}
};
struct WhileExpr :Expr{
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Expr> body;
    WhileExpr(
        std::unique_ptr<Expr> cond,
        std::unique_ptr<Expr> bod
    )
        : condition(std::move(cond)),
          body(std::move(bod))
    {}
    //print
    void print(int indent=0) const override {
    for (int i=0;i<indent;i++)std::cout << "  ";
    std::cout<<"WHILE"<< std::endl;
    condition->print(indent+1);
    body->print(indent+1);
}
};
struct CompareExpr : Expr {
    std::string op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    CompareExpr(
        std::string oper,
        std::unique_ptr<Expr> lhs,
        std::unique_ptr<Expr> rhs
    )
        : op(std::move(oper)),
          left(std::move(lhs)),
          right(std::move(rhs))
    {}
    //print
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << op << std::endl;
        left->print(indent + 1);
        right->print(indent + 1);
    }
};
struct BooleanExpr:Expr {
    bool value;
    BooleanExpr(bool val):value (val){}
    //print
    void print(int indent=0)const override{
        for(int i=0;i<indent;i++){
            std::cout<<"  ";
        }
        std::cout<<(value ? "true":"false")<<std::endl;
    }
};
struct PrintExpr : Expr {
    std::unique_ptr<Expr> value;
    PrintExpr(std::unique_ptr<Expr> val)
        : value(std::move(val))
    {}
    //print
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << "PRINT" << std::endl;
        value->print(indent + 1);
    }
};
struct InputExpr : Expr {
    void print(int indent = 0) const override {
        for (int i = 0; i < indent; i++)std::cout << "  ";
        std::cout << "INPUT" << std::endl;
    }
};
class Parser {
private:
    Lexer& lexer;
    Token currentToken;
public:
    Parser(Lexer& lex)
        : lexer(lex)
    {
        currentToken = lexer.getNextToken();
    }
    std::unique_ptr<Expr> parse();
private:
    void eat(TokenType type);
    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
    std::unique_ptr<Expr> statement();
    std::unique_ptr<Expr> block();
    std::unique_ptr<Expr> IfStatement();
    std::unique_ptr<Expr> WhileStatement();
    std::unique_ptr<Expr> printStatement();
};
void Parser::eat(TokenType type) {

    if (currentToken.type == type) {
        currentToken = lexer.getNextToken();
    }
    else {
        throw std::runtime_error("Unexpected token");
    }
}
std::unique_ptr<Expr> Parser::factor() {

    if (currentToken.type == TokenType::Number) {
        int value = std::stoi(std::string(currentToken.value));
        eat(TokenType::Number);
        return std::make_unique<NumberExpr>(value);
    }
    if (currentToken.type == TokenType::LeftParen) {
        eat(TokenType::LeftParen);
        auto node = expression();
        eat(TokenType::RightParen);
        return node;
    }
    if(currentToken.type==TokenType::True){
        eat (TokenType::True);
        return std::make_unique<BooleanExpr> (true);
    }
    if(currentToken.type==TokenType::False){
        eat (TokenType::False);
        return std::make_unique<BooleanExpr> (false);
    }
    if (currentToken.type == TokenType::Input) {
        eat(TokenType::Input);
        eat(TokenType::LeftParen);
        eat(TokenType::RightParen); 
        return std::make_unique<InputExpr>();
    }
    if (currentToken.type == TokenType::Identifier) {
        std::string name(currentToken.value);
        eat(TokenType::Identifier);
        return std::make_unique<VariableExpr>(name);
    }
    throw std::runtime_error("Invalid factor");
}
std::unique_ptr<Expr> Parser::term() {
    auto node = factor();
    while (
        currentToken.type == TokenType::Multiply ||
        currentToken.type == TokenType::Divide
    ) {
        char op = currentToken.value[0];
        if (currentToken.type == TokenType::Multiply)
            eat(TokenType::Multiply);
        else
            eat(TokenType::Divide);
        auto right=factor();
        node = std::make_unique<BinaryExpr>(
            op,
            std::move(node),
            std::move (right)
        );
    }
    return node;
}
std::unique_ptr<Expr> Parser::expression() {
    auto node = term();
    while (
        currentToken.type == TokenType::Plus ||
        currentToken.type == TokenType::Minus
    ) {
        char op = currentToken.value[0];
        if (currentToken.type == TokenType::Plus)
            eat(TokenType::Plus);
        else
            eat(TokenType::Minus);
        auto right=term();
        node = std::make_unique<BinaryExpr>(
            op,
            std::move(node),
            std::move(right)
        );
    }
    return node;
}
std::unique_ptr<Expr> Parser::comparison(){
    auto node=expression();
    while (
        currentToken.type == TokenType::Greater ||
        currentToken.type == TokenType::Less ||
        currentToken.type == TokenType::GreaterEqual ||
        currentToken.type == TokenType::LessEqual ||
        currentToken.type == TokenType::EqualEqual ||
        currentToken.type == TokenType::NotEqual
    ) {
        std::string op(currentToken.value);
        TokenType type = currentToken.type;
        eat(type);
        auto right = expression();
        node = std::make_unique<CompareExpr>(
            op,
            std::move(node),
            std::move(right)
        );
    }
    return node;
}
std::unique_ptr<Expr> Parser::assignment() {
    auto left = comparison();
    if (currentToken.type == TokenType::Equals) {
        auto var =dynamic_cast<VariableExpr*>(left.get());
        if (!var) {
            throw std::runtime_error(
                "Invalid assignment target"
            );
        }
        eat(TokenType::Equals);
        auto value = assignment();
        return std::make_unique<AssignExpr>(
            var->name,
            std::move(value)
        );
    }
    return left;
}
std::unique_ptr<Expr> Parser::statement() {
    if (currentToken.type == TokenType::Print) {
        return printStatement();
    }   
    if(currentToken.type==TokenType::If){
        return IfStatement();
    }
    if(currentToken.type==TokenType::While){
        return WhileStatement();
    }
    if(currentToken.type==TokenType::LeftBrace){
        return block();
    }
    return assignment();
}
std::unique_ptr<Expr> Parser::block(){
    eat(TokenType::LeftBrace);
    auto block=std::make_unique<BlockExpr>();
    while(currentToken.type !=TokenType::RightBrace){
        auto stmt=statement();
        block->statements.push_back(std::move(stmt));
        if(currentToken.type==TokenType::Semicolon){
            eat(TokenType::Semicolon);
        }
    }
    eat(TokenType::RightBrace);
    return block;
}

std::unique_ptr<Expr> Parser::IfStatement(){
    
    eat(TokenType::If);
    eat(TokenType::LeftParen);
    auto condition=assignment();
    eat(TokenType::RightParen);
    auto thenBranch=statement();
    std::unique_ptr<Expr> elseBranch;
    if(currentToken.type==TokenType::Else){
        eat(TokenType::Else);
        elseBranch=statement();
    }
    return std::make_unique<IfExpr>(
        std::move(condition),std::move(thenBranch),std::move(elseBranch)
    );
}
std::unique_ptr<Expr> Parser::WhileStatement(){
    eat(TokenType::While);
    eat(TokenType::LeftParen);
    auto condition=assignment();
    eat(TokenType::RightParen);
    auto body=statement();
    return std::make_unique<WhileExpr>(
        std::move(condition),std::move(body)
    );
}
std::unique_ptr<Expr> Parser::parse() {
    auto block = std::make_unique<BlockExpr>();
    while (currentToken.type != TokenType::EndOfFile) {
        auto stmt = statement();
        block->statements.push_back(
            std::move(stmt)
        );
        if (currentToken.type == TokenType::Semicolon) {
            eat(TokenType::Semicolon);
        }
        // else if(currentToken.type==TokenType::RightBrace){
        //     continue;
        // }
        // else {
        //     throw std::runtime_error(
        //         "Expected semicolon"
        //     );
        // }
    }
    return block;
}
std::unique_ptr<Expr> Parser::printStatement() {
    eat(TokenType::Print);
    eat(TokenType::LeftParen);
    auto value = assignment();
    eat(TokenType::RightParen);
    return std::make_unique<PrintExpr>(
        std::move(value)
    );
}