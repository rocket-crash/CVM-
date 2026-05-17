#pragma once
#include <string_view>
#include <iostream>
#include <cctype>
enum class TokenType {Identifier,Number,Plus,Minus,Multiply,Divide,Equals,
    Semicolon,LeftParen,RightParen,EndOfFile,LeftBrace,RightBrace,
    If,Else,While,Greater,Less,GreaterEqual,LessEqual,EqualEqual,NotEqual,True,False,Print,Input,Unknown};
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
        char peek();
        void skipWhitespace();
        Token number();
        Token identifier();
};
char Lexer::peek(){
    if(position+1>=source.length())return '\0';
    return source[position+1];
}
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
    if(text=="print"){return {TokenType::Print,text};}
    if(text=="input"){return {TokenType::Input,text};}
    if(text=="if"){return {TokenType::If,text};}
    if(text=="else"){return {TokenType::Else,text};}
    if(text=="while"){return {TokenType::While,text};}
    if(text=="true"){return {TokenType::True,text};}
    if(text=="false"){return {TokenType::False,text};}
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
            // case '/':
            //     position++;
            //     return {TokenType::Divide, "/"};
            case '/':
                if (peek() == '/') {
                    // It's a comment! Skip until the end of the line
                    while (currentChar() != '\n' && currentChar() != '\0') {
                        position++;
                    }
                    continue; // Go back to the top of the while loop to get the next real token
                }
                position++;
            return {TokenType::Divide, "/"};
            // case '=':
            //     position++;
            //     return {TokenType::Equals, "="};
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
            case '>':
                if(peek()=='='){
                    size_t start=position;
                    position++;
                    position++;
                    return {
                        TokenType::GreaterEqual,
                        source.substr(start,2)
                    };
                }
                position++;
                return {
                    TokenType::Greater,">"
                };
            case '<':
                if(peek()=='='){
                    size_t start=position;
                    position++;
                    position++;
                    return {
                        TokenType::LessEqual,
                        source.substr(start,2)
                    };
                }
                position++;
                return {
                    TokenType ::Less,"<"
                };
            case '=':
                if (peek() == '=') {
                size_t start = position;
                position++;
                position++;
                return {
                    TokenType::EqualEqual,
                    source.substr(start, 2)
                };
                }
                position++;
                return {
                    TokenType::Equals,"="
                 };
            case '!':
                if (peek() == '=') {
                size_t start = position;
                position++;
                position++;
                return {
                    TokenType::NotEqual,
                    source.substr(start, 2)
                };
                }
                throw std::runtime_error("Unexpected character: !");
            default:
                position++;
                return {TokenType::Unknown, source.substr(position-1,1)};
        }
    }
    return {TokenType::EndOfFile, ""};
}