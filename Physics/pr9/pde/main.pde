TITLE 'Corrected diffusion equation'

VARIABLES
    u

DEFINITIONS
    k = 3
    { Параметры из вашего Rust-кода }
    A = 15
    B = 1
    L = 9
    H = 7
    
    { Функция для правой границы из Rust: ls(y) = B + A/6 * y }
    { Масштабируем y под высоту 7 }
    ls_y = B + (A/6.0) * (y * (6/H))

EQUATIONS
    { Если это просто распределение без лазера, ставим 0. }
    { Если с лазером — добавляем Q, как делали раньше. }
    div(k*grad(u)) = 0 

BOUNDARIES
    REGION 1
        START(0,0)

        { Нижняя граница: линейный рост от 0 до B (как в Rust) }
        VALUE(u) = (x/L)*B             LINE TO (L,0)

        { Правая граница: используем функцию ls_y }
        { NATURAL(u)=0 подошло бы для изоляции, но в Rust там были значения }
        VALUE(u) = ls_y                LINE TO (L,H)

        { Верхняя граница: линейно от A до 0 }
        VALUE(u) = (x/L)*A             LINE TO (0,H)

        { Левая граница: в Rust была 0 }
        VALUE(u) = 0                   LINE TO CLOSE

PLOTS
    CONTOUR(u)
    VECTOR(-k*grad(u))    { Поток тепла идет от горячего к холодному, нужен минус }
    SURFACE(u)

END