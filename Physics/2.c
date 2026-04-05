// 17.02.2026

//

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>

#define RAD2DEG_M(_a) ((_a) * M_PI / 180.0)

const double g = 9.8;

int main(void) {
    double V0_in, alpha_in, x_in, y_in, tau_in;
    scanf("%lf %lf %lf %lf %lf\n", &V0_in, &alpha_in, &x_in, &y_in, &tau_in);

    alpha_in = RAD2DEG_M(alpha_in);

    double Vx0 = cos(alpha_in) * V0_in;
    double Vy0 = sin(alpha_in) * V0_in;

    printf("    X           Y          Vx          Vy         V\n\n");

    // Эйлер
    {
        double x = x_in;
        double y = y_in;
        double Vx = Vx0;
        double Vy = Vy0;

        while (1) {
            printf("%f  %f  %f  %f  %f\n", x, y, Vx, Vy, sqrt(Vx * Vx + Vy * Vy));
            x = x + tau_in * Vx;
            y = y + tau_in * Vy;

            if (y <= 0)
                break;

            Vy = Vy - g * tau_in;
        }
    }

    printf("\n\nРунге Кут:\n");
    printf("X           Y           Vx           Vy          V\n\n");
    {
        double x = x_in;
        double y = y_in;
        double Vx = Vx0;
        double Vy = Vy0;

        while (1) {
            printf("%f  %f  %f  %f  %f\n", x, y, Vx, Vy, sqrt(Vx * Vx + Vy * Vy));
            x = x + tau_in * Vx;

            Vy = Vy - g * tau_in;

            y = y + tau_in * Vy - 1.0 / 2.0 * tau_in * tau_in * g;

            if (y <= 0)
                break;
        }
    }


    return 0;
}
