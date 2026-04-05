
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
    dx = (b - a) / 2
    x1 = a
    x2 = x1 + dx
    x3 = x1 + 2 * dx

    iteration = 0

    while True:
        iteration += 1
        f1, f2, f3 = f(x1), f(x2), f(x3)

        x_line = quadratic_min(x1, x2, x3, f1, f2, f3)

        vals = [(f1, x1), (f2, x2), (f3, x3)]
        f_min, x_min = min(vals, key=lambda p: p[0])

        print(f"=== Итерация {iteration} ===")
        print(f"  x1 = {x1:.4f},  f(x1) = {f1:.4f}")
        print(f"  x2 = {x2:.4f},  f(x2) = {f2:.4f}")
        print(f"  x3 = {x3:.4f},  f(x3) = {f3:.4f}")
        print(f"  F_min = {f_min:.4f},  x_min = {x_min:.4f}")
        print(f"  x̄ = {x_line:.4f},  f(x̄) = {f(x_line):.4f}")

  
        cond1 = abs((f_min - f(x_line)) / f(x_line)) < EPS
        cond2 = abs((x_min - x_line) / x_line) < EPS
        print(f"  |F_min - f(x̄)| / f(x̄) = {abs((f_min - f(x_line)) / f(x_line)):.6f}  (eps1 = {EPS})")
        print(f"  |x_min - x̄| / |x̄|     = {abs((x_min - x_line) / x_line):.6f}  (eps2 = {EPS})")

        if cond1 and cond2:
            print(f"  Условие выполнено. x* = {x_line:.6f},  f(x*) = {f(x_line):.6f}")
            break

        if x1 <= x_line <= x3:
            print(f"  x̄ внутри [x1, x3] — шаг 6")
            pts = sorted([(x1, f1), (x2, f2), (x3, f3), (x_line, f(x_line))], key=lambda p: p[0])
            best_idx = min(range(len(pts)), key=lambda i: pts[i][1])
            x1, x2, x3 = pts[best_idx-1][0], pts[best_idx][0], pts[best_idx+1][0]
        else:
            print(f"  x̄ вне [x1, x3] — шаг 2")
            x1 = x_line
            x2 = x1 + dx
            f1, f2 = f(x1), f(x2)
            x3 = x1 + 2*dx if f1 > f2 else x1 - dx

        print()

solve(0, 2)
