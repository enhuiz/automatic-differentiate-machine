#include "graph.h"

#include <climits>

using namespace std;

void Graph::Set(string var, double value)
{
    if (VariableExist(var))
        table_[var].lock()->val(value);
    else
        cerr << "Error: Unknown variable \"" << var << "\"" << endl;
}

double Graph::Evaluate()
{
    if (root_ == nullptr)
        return numeric_limits<double>::quiet_NaN();

    if (forward_dirty)
    {
        root_->ForwardProp();
        forward_dirty = !forward_dirty;
    }
    return root_->val();
}

double Graph::Gradient(string var)
{
    if (VariableExist(var) && root_ != nullptr)
    {
        if (forward_dirty)
        {
            root_->ForwardProp();
            forward_dirty = !forward_dirty;
        }
        if (backward_dirty)
        {
            root_->ResetGrad();
            root_->grad(1); // dr/dr = 1, r for root
            root_->BackwardProp();
            backward_dirty = !backward_dirty;
        }
        return table_[var].lock()->grad();
    }
    return numeric_limits<double>::quiet_NaN();
}

vector<string> Graph::GetVariables() const
{
    auto ret = vector<string>();
    for (const auto &kv : table_)
    {
        ret.push_back(kv.first);
    }
    return ret;
}

bool Graph::VariableExist(string var) const
{
    return table_.count(var) && !table_.at(var).expired();
}