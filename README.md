# Automatic Differentiation Machine

## Introduction

This is toy automatic differentiation machine which parses expressions and then differentiate them according to different variables.

## Build

```
$ mkdir build
$ cmake ..
$ make
```

## Run Repl

```
$ build/repl
```

## Run Test

```
$ python test/test.py
```

## Use it in your code

```c++
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
```

## References

- http://llvm.org/docs/index.html

- https://github.com/PanagiotisPtr/Reverse-Mode-Automatic-Differentiation-Cpp

---

Have fun!
