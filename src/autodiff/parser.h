#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "node.h"
#include "graph.h"

class Parser
{
  public:
    Graph Parse(std::string expr);

  private:
    std::shared_ptr<Node> ParseNumber(std::list<Token> &tokens);
    std::shared_ptr<Node> ParseParenExpr(std::list<Token> &tokens);
    std::shared_ptr<Node> ParseIdent(std::list<Token> &tokens);
    std::shared_ptr<Node> ParsePrimary(std::list<Token> &tokens);
    std::shared_ptr<Node> ParseBinOpRhs(std::list<Token> &tokens,
                                        std::shared_ptr<Node> lhs,
                                        int prec);
    std::shared_ptr<Node> ParseExpr(std::list<Token> &tokens);

  private:
    SymbolTable table_;
};
#endif