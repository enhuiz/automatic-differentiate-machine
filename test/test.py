from sympy import symbols, diff, sympify
from math import tan, sin, cos, exp, log, isclose
from termcolor import colored
import os

PATH = os.path.dirname(__file__)

import subprocess


def log_pass(*args):
    print(colored("Pass:", "green"), *args)


def log_fail(*args):
    print(colored("Fail:", "red"), *args)


def run_auto_deriv(expr, value):
    if not isinstance(value, list):
        value = [value]
    value = ' '.join(map(str, value))

    ret = subprocess.check_output([os.path.join(PATH, '../build/main'),
                                   '-e',
                                   expr,
                                   '-v',
                                   value]).decode('utf8')
    return [float(v) for v in ret.strip().split('\n')]


def run_sympy(expr, value):
    if not isinstance(value, list):
        value = [value]

    sym_expr = sympify(expr)
    vars_ = sorted([str(var) for var in sym_expr.free_symbols])
    value_by_var = {var: value for var,
                    value in zip(vars_, value)}
    evaluation = sym_expr.evalf(subs=value_by_var)
    gradients = [diff(sym_expr, var).evalf(subs=value_by_var)
                 for var in value_by_var.keys()]

    return [evaluation, *gradients]


def is_lst_close(xs, ys):
    xs = list(xs)
    ys = list(ys)
    return all([isclose(x, y, rel_tol=1e-5) for x, y in zip(xs, ys)])


def test(exprs, vals):
    for args in [(expr, val) for expr in exprs for val in vals]:
        try:
            ad_res = run_auto_deriv(*args)
            sp_res = run_sympy(*args)
            if not is_lst_close(ad_res, sp_res):
                log_fail(*args, ad_res, sp_res)
            else:
                log_pass(*args)
        except Exception as e:
            print(e)
            log_fail(args)


def test_univar():
    exprs = [
        'x',
        'x^2',
        'log(x)',
        'sin(x)',
        'cos(x)',
        'tan(x)',
        'exp(x)',
        'tan(x)',
        '1/x',
        '1/x^2',
        'sin(x^2)',
        '1/tan(x^2)^2 + sin(log(x^2)^3)',
        '1/tan(x^2)^2',
        '1/tan(1/x)^2*2/5^x',
        '1/log(10+x)*sin(x)/cos(x)',
        '5*sin(x)^2',
        'x^2 / cos(x) * tan(x)',
        '5+x^2/2'
    ]

    vals = [
        1,
        2,
        3,
        123,
        1234
    ]

    test(exprs, vals)


def test_multivar():
    exprs = [
        'x/y',
        'x^2 + y^2',
        'log(x)*log(y)',
        'sin(x)/sin(y)',
        'cos(x)*cos(y)',
        'tan(x)/tan(y)',
        'exp(x)/exp(y)',
        'tan(x)*tan(y)',
        '1/x + 1/y',
        '1/x^2 + 1/y^2',
        'sin(x^2) * sin(y^2)',
        '1/tan(x^2)^2 + sin(log(x^2)^3) + log(y^2)/tan(y^2*x)',
        '1/tan(x^2)^2*y^2',
    ]

    vals = [
        [1, 2],
        [2, 3],
        [3, 4],
        [123, 321],
        [1234, 542]
    ]

    test(exprs, vals)


def main():
    test_univar()
    test_multivar()


if __name__ == '__main__':
    main()
