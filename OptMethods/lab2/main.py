import math

EPS = 1 / 10000
A, B = 1.0, 7.0

# Исходная функция
def f(x):
    return math.sin(x) * (1 + 0.3 * math.sin(10 * x))

# Производная
def df(x):
    return math.cos(x) * (1 + 0.3 * math.sin(10 * x)) + 3 * math.sin(x) * math.cos(10 * x)

# Вторая производная
def d2f(x):
    return (-math.sin(x) * (1 + 0.3 * math.sin(10 * x))
            + 6 * math.cos(x) * math.cos(10 * x)
            - 30 * math.sin(x) * math.sin(10 * x))

# Метод деления отрезка пополам
def bisection(a, b):
    for i in range(1, 1001):
        # Делим диапаон пополам
        mid = (a + b) / 2

        # КРИТЕРИЙ ОСТАНОВА:
        # Если производная почти ноль, либа отрезок стал крошечным
        if abs(df(mid)) < EPS or (b - a) / 2 < EPS:
            return mid, i

        # Определям в какой половине производная меняет знак
        if df(a) * df(mid) < 0:
            b = mid
        else:
            a = mid

    # Если не сошлось за 1000 шагов
    return (a + b) / 2, 1000


# Метод золотого сечения
def golden_section(a, b):
    # Коэффициент Золотого сечения ~= 0.618
    phi = (math.sqrt(5) - 1) / 2

    # Расставляем две пробные точки внтури отрезка
    x1 = b - phi * (b - a)
    x2 = a + phi * (b - a)
    
    for i in range(1, 1001):
        if abs(df(x1)) < abs(df(x2)):
            b, x2, x1 = x2, x1, b - phi * (b - x2)
        else:
            a, x1, x2 = x1, x2, a + phi * (x1 - a)

        # КРИТЕРИЙ ОСТАНОВА
        # Отрезок стал очень маленьким
        if (b - a) < EPS:
            return (a + b) / 2, i

    # Если не сошлось за 1000 шагов
    return (a + b) / 2, 1000


# Метод Хорд
def chord(a, b):
    # Начальные концы отрезка
    x0, x1 = a, b
    for i in range(1, 1001):
        dfa, dfb = df(x0), df(x1)

        # Формула Хорды
        x2 = x1 - dfb * (x1 - x0) / (dfb - dfa)

        # КРИТЕРИЙ ОСТАНОВА
        # Новая точка почти не сдвинулась
        if abs(x2 - x1) < EPS:
            return x2, i

        # Сдвигаем окно, x0 -> x1, x1 -> x2
        x0, x1 = x1, x2

    # Если не сошлись за 1000 шагов
    return x1, 1000


# Метод Ньютона
def newton(x0):
    # Стартовая точка, середина подотрезка
    x = x0

    for i in range(1, 1001):
        # Вторая производная, наклон графика
        d2 = d2f(x)

        # Если наклон графика практически нулевой, то деление на ноль
        # Выходим
        if abs(d2) < 1e-15:
            break
        # Формула Ньютона
        x_new = x - df(x) / d2

        # КРИТЕРИЙ ОСТАНОВА
        # Шаг стал крошечным - сошлись
        if abs(x_new - x) < EPS:
            return x_new, i

        # Переходим в новую точку
        x = x_new

    # Если не сошлись за 1000
    return x, 1000


# Находим интервалы, где меняются знаки
def find_intervals(a, b, n=50000):
    step = (b - a) / n
    intervals = []
    xa, fa = a, df(a)
    for _ in range(n):
        xb = xa + step
        fb = df(xb)
        if fa * fb < 0:
            intervals.append((xa, xb))
        xa, fa = xb, fb
    return intervals


def ext_type(x):
    return "МАКСИМУМ" if d2f(x) < 0 else "МИНИМУМ"


def main():
    print("f(x) = sin(x)*(1 + 0.3*sin(10x))   на  [1, 7]\n")

    intervals = find_intervals(A, B)
    print(f"Подынтервалов со сменой знака f'(x): {len(intervals)}\n")

    fmt = lambda v: f"{v:.4f}"
    print("   # | Бисекция   | Золотое    | Хорды      | Ньютон     |     f(x) | Тип")

    results = []
    for idx, (a, b) in enumerate(intervals, 1):
        x_bis, n_bis = bisection(a, b)
        x_gld, n_gld = golden_section(a, b)
        x_chd, n_chd = chord(a, b)
        x_nwt, n_nwt = newton((a + b) / 2)
        x_avg = (x_bis + x_gld + x_chd + x_nwt) / 4
        tp = ext_type(x_avg)
        results.append((idx, x_bis, x_gld, x_chd, x_nwt, x_avg, f(x_avg), tp,
                        n_bis, n_gld, n_chd, n_nwt, a, b))
        print(f"  {idx:>2} | {fmt(x_bis):>10} | {fmt(x_gld):>10} | "
              f"{fmt(x_chd):>10} | {fmt(x_nwt):>10} | {f(x_avg):>8.4f} | {tp}")

    maxima = [r for r in results if r[7] == "МАКСИМУМ"]
    minima = [r for r in results if r[7] == "МИНИМУМ"]
    print(f"  Всего: {len(results)}  |  Максимумов: {len(maxima)}  |  Минимумов: {len(minima)}")

if __name__ == "__main__":
    main()
