
import math

def f(x):
    return (1/4)*x**4 + x**2 - 8*x + 12

def quadratic_min(x1, x2, x3, f1, f2, f3):
    num = (x2**2 - x3**2)*f1 + (x3**2 - x1**2)*f2 + (x1**2 - x2**2)*f3
    den = (x2 - x3)*f1 + (x3 - x1)*f2 + (x1 - x2)*f3
    if abs(den) < 1e-15:
        return None
    return 0.5 * num / den

def solve(a, b, eps=0.0001):
    x1, x2, x3 = a, (a + b) / 2, b

    iteration = 0
    x_prev = None

    while True:
        iteration += 1
        f1, f2, f3 = f(x1), f(x2), f(x3)

        x_star = quadratic_min(x1, x2, x3, f1, f2, f3)

        print(f"Итерация {iteration}:")
        print(f"  x1={x1:.6f}  x2={x2:.6f}  x3={x3:.6f}")
        print(f"  f1={f1:.6f}  f2={f2:.6f}  f3={f3:.6f}")
        print(f"  x* = {x_star:.6f}  f(x*) = {f(x_star):.6f}")
        print()

        if x_star is None:
            print("Знаменатель равен 0, остановка.")
            break

        if x_prev is not None and abs(x_star - x_prev) < eps:
            print(f"Ответ: x* = {x_star:.6f},  f(x*) = {f(x_star):.6f}")
            break

        x_prev = x_star

        # Выбираем новую тройку из 4 точек: берём три, окружающие минимум
        pts = sorted([(x1, f1), (x2, f2), (x3, f3), (x_star, f(x_star))], key=lambda p: p[0])
        best = min(pts, key=lambda p: p[1])
        idx = pts.index(best)

        if idx == 0:
            x1, x2, x3 = pts[0][0], pts[1][0], pts[2][0]
        elif idx == len(pts) - 1:
            x1, x2, x3 = pts[-3][0], pts[-2][0], pts[-1][0]
        else:
            x1, x2, x3 = pts[idx-1][0], pts[idx][0], pts[idx+1][0]

solve(0, 2)
