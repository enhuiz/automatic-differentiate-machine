#include "node.h"

#include <cmath>

using namespace std;

// unary-op

void ExpOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::exp(node_->val());
}

void ExpOpNode::BackwardProp()
{
    node_->IncGrad(grad_ * std::exp(node_->val()));
    node_->BackwardProp();
}

void LnOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::log(node_->val());
}

void LnOpNode::BackwardProp()
{
    node_->IncGrad(grad_ * 1 / node_->val());
    node_->BackwardProp();
}

void AbsOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::abs(node_->val());
}

void AbsOpNode::BackwardProp()
{
    node_->IncGrad(grad_ * node_->val() > 0 ? 1 : -1);
    node_->BackwardProp();
}

void SinOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::sin(node_->val());
}

void SinOpNode::BackwardProp()
{
    node_->IncGrad(grad_ * std::cos(node_->val()));
    node_->BackwardProp();
}

void CosOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::cos(node_->val());
}

void CosOpNode::BackwardProp()
{
    node_->IncGrad(grad_ * -std::sin(node_->val()));
    node_->BackwardProp();
}

void TanOpNode::ForwardProp()
{
    node_->ForwardProp();
    val_ = std::tan(node_->val());
}

void TanOpNode::BackwardProp()
{
    node_->IncGrad(grad_ / std::pow(std::cos(node_->val()), 2));
    node_->BackwardProp();
}

// bin-op

void PlusOpNode::ForwardProp()
{
    lhs_->ForwardProp();
    rhs_->ForwardProp();
    val_ = lhs_->val() + rhs_->val();
}

void PlusOpNode::BackwardProp()
{
    lhs_->IncGrad(grad_ * 1);
    rhs_->IncGrad(grad_ * 1);

    lhs_->BackwardProp();
    rhs_->BackwardProp();
}

void MinusOpNode::ForwardProp()
{
    lhs_->ForwardProp();
    rhs_->ForwardProp();
    val_ = lhs_->val() - rhs_->val();
}

void MinusOpNode::BackwardProp()
{
    lhs_->IncGrad(grad_ * 1);
    rhs_->IncGrad(grad_ * -1);

    lhs_->BackwardProp();
    rhs_->BackwardProp();
}

void MulOpNode::ForwardProp()
{
    lhs_->ForwardProp();
    rhs_->ForwardProp();
    val_ = lhs_->val() * rhs_->val();
}

void MulOpNode::BackwardProp()
{
    lhs_->IncGrad(grad_ * rhs_->val()); // y
    rhs_->IncGrad(grad_ * lhs_->val()); // x

    lhs_->BackwardProp();
    rhs_->BackwardProp();
}

void DivOpNode::ForwardProp()
{
    lhs_->ForwardProp();
    rhs_->ForwardProp();
    val_ = lhs_->val() / rhs_->val();
}

void DivOpNode::BackwardProp()
{
    lhs_->IncGrad(grad_ * 1 / rhs_->val());                           // 1/y
    rhs_->IncGrad(grad_ * (-lhs_->val() / std::pow(rhs_->val(), 2))); //  -x/y^2

    lhs_->BackwardProp();
    rhs_->BackwardProp();
}

void PowOpNode::ForwardProp()
{
    lhs_->ForwardProp();
    rhs_->ForwardProp();
    val_ = std::pow(lhs_->val(), rhs_->val());
}

void PowOpNode::BackwardProp() // x ^ y
{
    lhs_->IncGrad(grad_ * rhs_->val() * std::pow(lhs_->val(), rhs_->val() - 1));  // yx^(y-1)
    rhs_->IncGrad(grad_ * std::pow(lhs_->val(), rhs_->val()) * log(lhs_->val())); // x^ylnx

    lhs_->BackwardProp();
    rhs_->BackwardProp();
}
