// 10.02.2026
//
// Филипов. 94
//
// Воронка имеет форму конуса радиуса R = 6 см и высоты Н = 10 см, обращенного
// вершиной вниз. За какое время вытечет вся вода из воронки через круглое
// отверстие диаметра 0,5 см, сделанное в вершине конуса?

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

// Условия задачи
const double R = 0.06;
const double H = 0.1;
const double r = 0.0025;

const double g = 9.8;

// Площадь выходного отверстия
const double S = M_PI * r * r;

const double k = R / H;

int main(void) {
    // Временной шаг
    const double dt = 0.000001;
    // Прошедшее время
    double t = 0;
    // Уровень остевщейся воды в воронке
    double h = H;

    // Считаем пока в воронке есть вода
    while (h > 0.000000001) {
        double R = k * h;
        // Находим объём воды в воронке
        double V = 1.0 / 3.0 * M_PI * h * (R * R + R * r + r * r);

        // Скорость через Торичелли
        const double speed = sqrtf(2 * g * h);
        // Количество утекаемой воды через дырку внизу
        const double Q = S * speed;

        // Изменение объёма за временной шаг
        double dV = Q * dt;

        // Через производную V(h)
        double dh = dV / (M_PI * R * R);
        
        h -= dh;
        t += dt;
    }

    printf("%f\n", t);
    return 0;
}
