#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <list>

struct Token
{
    enum Type
    {
        OPERATOR, // 0
        LPAREN,   // 1
        RPAREN,   // 2
        IDENT,    // 3
        NUMBER,   // 4
        END       // 5
    } type;
    std::string s;
};

std::list<Token> Tokenize(std::string s);

#endif