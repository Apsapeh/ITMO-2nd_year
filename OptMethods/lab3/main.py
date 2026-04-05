import math

EPS = 0.0001

def f(x):
    return (1/4) * x**4 + x**2 - 8*x + 12


def quadratic_min(x1, x2, x3, f1, f2, f3):
    num = (x2**2 - x3**2)*f1 + (x3**2 - x1**2)*f2 + (x1**2 - x2**2)*f3
    den = (x2 - x3)*f1 + (x3 - x1)*f2 + (x1 - x2)*f3
    if abs(den) < 1e-15:
        return None
    return 0.5 * num / den

def solve(a, b):
    x1 = a
    x2 = (a + b) / 2
    x3 = b
    dx = abs(a + b) / 2

    iteration = 0
    x_prev = None

    while True:
        iteration += 1

        f1, f2, f3 = f(x1), f(x2), f(x3)

        x_line = quadratic_min(x1, x2, x3, f1, f2, f3)

        
        vals = [(f1, x1), (f2, x2), (f3, x3)]
        f_min, x_min = min(vals, key=lambda p: p[0])

        cond1 = abs((f_min - f(x_line)) / f(x_line)) < EPS
        cond2 = abs((x_min - x_line) / x_line) < EPS

        if (cond1 and cond2):
            print(f"x* = {x_line}")
            break

        if (x_line >= x1 and x_line <= x3):
            # Шаг 6
            pass
        else:
            # Шаг 2
            x1 = x_line
            x2 = x1 + dx

            f1, f2 = f1(x1), f(x2)

            if (f1 > f2):
                x3 = x1 + 2 * dx
            else:
                x3 = x1 - dx

        
        
        
