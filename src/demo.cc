#include "autodiff/core.h"

using namespace std;

int main()
{
    auto expr = "x^2 + 5";
    auto parser = Parser();
    auto graph = parser.Parse(expr);
    graph.Set("x", 5);
    cout << graph.Evaluate() << endl;    // 30
    cout << graph.Gradient("x") << endl; // 10
    return 0;
}