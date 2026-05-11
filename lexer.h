#pragma once
#include <string_view>
#include <iostream>
#include <cctype>
enum class TokenType {Identifier,Number,Plus,Minus,Multiply,Divide,Equals,Semicolon,LeftParen,RightParen,EndOfFile,LeftBrace,RightBrace,If,While,Unknown};
struct Token {
    TokenType type;
    std::string_view value;
};
class Lexer {
    private:
        std::string_view source;
        size_t position;

    public:
        Lexer(const std::string_view& input)
            : source(input), position(0) {}

        Token getNextToken();

    private:
        char currentChar();
        void skipWhitespace();
        Token number();
        Token identifier();
};
char Lexer::currentChar() {
    if (position >= source.length())
        return '\0';

    return source[position];
}
void Lexer::skipWhitespace() {
    while (isspace(currentChar())) {
        position++;
    }
}
Token Lexer::number() {
    size_t start=position;
    while (isdigit(currentChar())) {
        position++;
    }
    return {
        TokenType::Number,
        source.substr(start, position - start)
    };
}
Token Lexer::identifier() {
    size_t start=position;
    while (isalnum(currentChar()) || currentChar() == '_') {
        position++;
    }
    auto text=source.substr(start,position-start);
    if(text=="if"){return {TokenType::If,text};}
    if(text=="while"){return {TokenType::While,text};}
    return {
        TokenType::Identifier,
        text
    };
}
Token Lexer::getNextToken() {
    while (currentChar() != '\0') {
        if (isspace(currentChar())) {
            skipWhitespace();
            continue;
        }
        if (isdigit(currentChar())) {
            return number();
        }
        if (isalpha(currentChar()) || currentChar() == '_') {
            return identifier();
        }
        char c = currentChar();
        switch (c) {
            case '+':
                position++;
                return {TokenType::Plus, "+"};
            case '-':
                position++;
                return {TokenType::Minus, "-"};
            case '*':
                position++;
                return {TokenType::Multiply, "*"};
            case '/':
                position++;
                return {TokenType::Divide, "/"};
            case '=':
                position++;
                return {TokenType::Equals, "="};
            case ';':
                position++;
                return {TokenType::Semicolon, ";"};
            case '(':
                position++;
                return {TokenType::LeftParen, "("};
            case ')':
                position++;
                return {TokenType::RightParen, ")"};
            case '{':
                position++;
                return {TokenType::LeftBrace,"{"};
            case '}':
                position++;
                return {TokenType::RightBrace,"}"};
            default:
                position++;
                return {TokenType::Unknown, source.substr(position-1,1)};
        }
    }
    return {TokenType::EndOfFile, ""};
}