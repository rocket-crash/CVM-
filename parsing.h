#pragma once
#include<memory>
#include <iostream>
#include <stdexcept>
#include <string>
#include "lexer.h"
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
    std::unique_ptr<Expr> term();
    std::unique_ptr<Expr> factor();
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
        node = std::make_unique<BinaryExpr>(
            op,
            std::move(node),
            term()
        );
    }
    return node;
}
std::unique_ptr<Expr> Parser::parse() {
    return expression();
}