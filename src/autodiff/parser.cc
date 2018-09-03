#include "parser.h"

using namespace std;

static auto kBinOpPrecedence = std::map<string, int>{{"+", 0},
                                                     {"-", 0},
                                                     {"*", 1},
                                                     {"/", 1},
                                                     {"^", 2}};

int GetOpPrecedence(const Token &token)
{
    if (kBinOpPrecedence.count(token.s))
    {
        return kBinOpPrecedence[token.s];
    }
    return -1;
}

void AssertRParen(const Token &token)
{
    if (token.type != Token::RPAREN)
    {
        throw runtime_error("Error: ')' is needed here.");
    }
}

shared_ptr<Node> Parser::ParseNumber(list<Token> &tokens)
{
    const auto &token = tokens.front();
    double val = stod(token.s);
    tokens.pop_front();
    return make_shared<NumNode>(val);
}

shared_ptr<Node> Parser::ParseParenExpr(list<Token> &tokens)
{
    tokens.pop_front(); // eat (.
    auto node = ParseExpr(tokens);
    if (node == nullptr)
        return nullptr;

    AssertRParen(tokens.front());
    tokens.pop_front(); // eat ).

    return node;
}

shared_ptr<Node> Parser::ParseIdent(list<Token> &tokens)
{
    auto token = tokens.front();
    tokens.pop_front();
    if (tokens.front().type == Token::LPAREN)
    {
        tokens.pop_front(); // eat (.
        auto node = ParseExpr(tokens);
        AssertRParen(tokens.front());
        tokens.pop_front(); // eat ).

        if (token.s == "log" || token.s == "ln")
        {
            return make_shared<LnOpNode>(node);
        }
        else if (token.s == "exp")
        {
            return make_shared<ExpOpNode>(node);
        }
        else if (token.s == "abs")
        {
            return make_shared<AbsOpNode>(node);
        }
        else if (token.s == "sin")
        {
            return make_shared<SinOpNode>(node);
        }
        else if (token.s == "cos")
        {
            return make_shared<CosOpNode>(node);
        }
        else if (token.s == "tan")
        {
            return make_shared<TanOpNode>(node);
        }
        throw runtime_error("Error: Unknown function '" + token.s + "'");
    }

    string var = token.s;

    if (table_.count(var))
        return table_[var].lock();

    auto node = make_shared<VarNode>(var);
    table_[var] = node;
    return node;
}

shared_ptr<Node> Parser::ParsePrimary(list<Token> &tokens)
{
    auto token = tokens.front();
    switch (token.type)
    {
    case Token::IDENT:
        return ParseIdent(tokens);
    case Token::NUMBER:
        return ParseNumber(tokens);
    case Token::LPAREN:
        return ParseParenExpr(tokens);
    case Token::END:
        return nullptr;
    default:
        throw runtime_error("Error: Unkown token when expecting an expression");
    }
}

shared_ptr<Node> MakeBinOpNode(string op, shared_ptr<Node> lhs, shared_ptr<Node> rhs)
{
    shared_ptr<Node> ret = nullptr;
    if (op == "+")
    {
        ret = make_shared<PlusOpNode>(lhs, rhs);
    }
    else if (op == "-")
    {
        ret = make_shared<MinusOpNode>(lhs, rhs);
    }
    else if (op == "*")
    {
        ret = make_shared<MulOpNode>(lhs, rhs);
    }
    else if (op == "/")
    {
        ret = make_shared<DivOpNode>(lhs, rhs);
    }
    else if (op == "^")
    {
        ret = make_shared<PowOpNode>(lhs, rhs);
    }
    return ret;
}

// this is an amazing function, i will dig into it later
shared_ptr<Node> Parser::ParseBinOpRhs(list<Token> &tokens,
                                       std::shared_ptr<Node> lhs,
                                       int root_prec)
{
    while (tokens.front().type == Token::OPERATOR)
    {
        auto token = tokens.front();
        int cur_prec = GetOpPrecedence(token);

        if (root_prec >= cur_prec)
            break;

        tokens.pop_front();

        // parse
        auto primary = ParsePrimary(tokens);
        auto rhs = ParseBinOpRhs(tokens, primary, cur_prec);

        // combine it with lhs, then update lhs
        lhs = MakeBinOpNode(token.s, lhs, rhs);
    }
    return lhs;
}

shared_ptr<Node> Parser::ParseExpr(list<Token> &tokens)
{
    auto lhs = ParsePrimary(tokens);
    return ParseBinOpRhs(tokens, lhs, -1);
}

Graph Parser::Parse(string expr)
{
    table_ = SymbolTable();
    auto tokens = Tokenize(expr);
    shared_ptr<Node> root = nullptr;
    try
    {
        root = ParseExpr(tokens);
    }
    catch (const runtime_error &e)
    {
        cerr << e.what() << endl;
    }
    return Graph(root, move(table_));
}
