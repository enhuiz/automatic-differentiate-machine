#include <iostream>
#include <algorithm>

#include "autodiff/core.h"

using namespace std;

int main()
{
    auto parser = Parser();

    while (true)
    {
        cout << "--------------------------" << endl;
        cout << "Expression: ";
        string expr;

        do
        {
            getline(cin, expr);
            remove_if(expr.begin(), expr.end(), [](char c) { return isspace(c); });
        } while (expr.size() == 0);

        auto graph = parser.Parse(expr);
        for (auto var : graph.GetVariables())
        {
            double val;
            cout << "Value of variable \""
                 << var
                 << "\": ";
            cin >> val;
            graph.Set(var, val);
        }
        cout << "Expression evaluate: " << graph.Evaluate() << endl;
        for (auto var : graph.GetVariables())
        {
            cout << "Gradient of \"" << var << "\": " << graph.Gradient(var) << endl;
        }
    }

    return 0;
}