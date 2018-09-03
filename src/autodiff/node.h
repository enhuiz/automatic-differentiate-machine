#ifndef NODE_H
#define NODE_H

#include <functional>
#include <memory>
#include <climits>
#include <string>
#include <map>
#include <iostream>

class Node
{
  public:
    virtual void ForwardProp() {}
    virtual void BackwardProp() {}

    virtual void ResetGrad()
    {
        grad_ = 0;
    }

    void IncGrad(double inc)
    {
        grad_ += inc;
    }

    double val() const
    {
        return val_;
    }

    double grad() const
    {
        return grad_;
    }

    void grad(double grad)
    {
        grad_ = grad;
    }

  protected:
    double val_;
    double grad_;
};

class ValNode : public Node
{
  public:
    ValNode(double val)
    {
        val_ = val;
    }

    void val(double val)
    {
        val_ = val;
    }
};

class NumNode : public ValNode
{
  public:
    NumNode(double val) : ValNode(val) {}
};

class VarNode : public ValNode
{
  public:
    VarNode(std::string name)
        : ValNode(std::numeric_limits<double>::quiet_NaN()),
          name_(name)
    {
    }

    std::string name() const
    {
        return name_;
    }

    void name(std::string name)
    {
        name_ = name;
    }

  private:
    std::string name_;
};

class UnaryNode : public Node
{
  public:
    UnaryNode(std::shared_ptr<Node> node)
        : node_(node) {}

    virtual void ForwardProp() override = 0;
    virtual void BackwardProp() override = 0;

    void ResetGrad() override
    {
        Node::ResetGrad();
        node_->ResetGrad();
    }

  protected:
    std::shared_ptr<Node> node_;
};

class ExpOpNode : public UnaryNode
{
  public:
    ExpOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class LnOpNode : public UnaryNode
{
  public:
    LnOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class AbsOpNode : public UnaryNode
{
  public:
    AbsOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class SinOpNode : public UnaryNode
{
  public:
    SinOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class CosOpNode : public UnaryNode
{
  public:
    CosOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class TanOpNode : public UnaryNode
{
  public:
    TanOpNode(std::shared_ptr<Node> node)
        : UnaryNode(node) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class BinOpNode : public Node
{
  public:
    BinOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : lhs_(lhs), rhs_(rhs) {}

    virtual void ForwardProp() override = 0;
    virtual void BackwardProp() override = 0;
    void ResetGrad() override
    {
        Node::ResetGrad();
        lhs_->ResetGrad();
        rhs_->ResetGrad();
    }

  protected:
    std::shared_ptr<Node> lhs_, rhs_;
};

class PlusOpNode : public BinOpNode
{
  public:
    PlusOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : BinOpNode(lhs, rhs) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class MinusOpNode : public BinOpNode
{
  public:
    MinusOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : BinOpNode(lhs, rhs) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class MulOpNode : public BinOpNode
{
  public:
    MulOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : BinOpNode(lhs, rhs) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class DivOpNode : public BinOpNode
{
  public:
    DivOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : BinOpNode(lhs, rhs) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

class PowOpNode : public BinOpNode
{
  public:
    PowOpNode(std::shared_ptr<Node> lhs, std::shared_ptr<Node> rhs)
        : BinOpNode(lhs, rhs) {}

    void ForwardProp() override;
    void BackwardProp() override;
};

#endif