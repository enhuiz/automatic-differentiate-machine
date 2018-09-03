#include <iostream>

#include "autodiff/core.h"

using namespace std;

#include <algorithm>
#include <sstream>

char *getCmdOption(char **begin, char **end, const std::string &option)
{
    char **itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
    {
        return *itr;
    }
    return nullptr;
}

bool cmdOptionExists(char **begin, char **end, const std::string &option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char *argv[])
{
    auto option_exists = [&](string option) {
        return cmdOptionExists(argv, argv + argc, option);
    };

    auto get_option = [&](string option) {
        return string(getCmdOption(argv, argv + argc, option));
    };

    if (option_exists("-e"))
    {
        auto expr = get_option("-e");
        if (option_exists("-v"))
        {
            istringstream iss(get_option("-v"));
            auto parser = Parser();
            auto graph = parser.Parse(expr);
            double value;
            for (auto var : graph.GetVariables())
            {
                iss >> value;
                graph.Set(var, value);
            }

            cout << graph.Evaluate() << endl;

            for (auto var : graph.GetVariables())
            {
                iss >> value;
                cout << graph.Gradient(var) << endl;
            }
        }
    }

    return 0;
}
