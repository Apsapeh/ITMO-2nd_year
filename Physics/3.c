// 24.02.2026

//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD2DEG_M(_a) ((_a) * M_PI / 180.0)

const double g = 9.8;


double get_max_distance(
        double V0_in, double alpha_in, double k_in, double m_in, double tau_in, char need_to_print
) {
    alpha_in = RAD2DEG_M(alpha_in);

    double x = 0;
    double y = 0;
    double Vx = cos(alpha_in) * V0_in;
    double Vy = sin(alpha_in) * V0_in;
    double Ax = 0;
    double Ay = 0;

    double time = 0;

    while (1) {
        if (need_to_print) {
        // printf("%f %f  %f  %f  %f\n", time, x, y, Vx, Vy);
            printf("%f %f\n", x, y);
        }

        x = x + tau_in * Vx;
        y = y + tau_in * Vy;

        if (y <= 0)
            break;

        double Ax_tmp = Ax;
        double Ay_tmp = Ay;


        Ax = -k_in / m_in * Vx;
        Ay = -k_in / m_in * Vy - g;

        Vx = Vx + Ax_tmp * tau_in;
        Vy = Vy + Ay_tmp * tau_in;

        time += tau_in;
    }

    return x;
}


int main(void) {
    double V0_in, alpha_in, k_in, m_in, tau_in;
    scanf("%lf %lf %lf %lf %lf\n", &V0_in, &alpha_in, &k_in, &m_in, &tau_in);

    double max = -1;
    double max_ang = 0;
    for (double a = 0; a < 90; a += 0.01) {
        double d = get_max_distance(V0_in, a, k_in, m_in, tau_in, 0);
        if (d > max) {
            max = d;
            max_ang = a;
        }
    }
    printf("Угол: %f, Xmax: %f\n", max_ang, max);


    double d = get_max_distance(V0_in, alpha_in, k_in, m_in, tau_in, 1);

    return 0;
}
