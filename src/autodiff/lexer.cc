#include "lexer.h"

using namespace std;

list<Token> Tokenize(list<char> &cs)
{
    assert(cs.back() == '\0');

    while (isspace(cs.front()))
    {
        cs.pop_front();
    }

    auto token = Token();

    if (cs.front() == '\0')
    {
        token.type = Token::END;
        return {token};
    }

    char c = cs.front();
    if (isalpha(c))
    {
        token.type = Token::IDENT;
        do
        {
            token.s += c;
            cs.pop_front();
            c = cs.front();
        } while (isalnum(c));
    }
    else if (isnumber(cs.front()))
    {
        token.type = Token::NUMBER;

        auto take_num = [&]() {
            do
            {
                token.s += cs.front();
                cs.pop_front();
                c = cs.front();
            } while (isnumber(c));
        };

        take_num();
        if (c == '.')
        {
            take_num();
        }
    }
    else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^')
    {
        token.type = Token::OPERATOR;
        token.s = c;
        cs.pop_front();
    }
    else if (cs.front() == '(')
    {
        token.type = Token::LPAREN;
        token.s = "(";
        cs.pop_front();
    }
    else if (cs.front() == ')')
    {
        token.type = Token::RPAREN;
        token.s = ")";
        cs.pop_front();
    }

    auto ret = list<Token>{token};
    ret.splice(ret.end(), Tokenize(cs));
    return ret;
}

list<Token> Tokenize(string s)
{
    list<char> cs(s.begin(), s.end());
    cs.push_back('\0');
    return Tokenize(cs);
}
