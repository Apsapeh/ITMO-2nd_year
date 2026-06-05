
#include <stdio.h>

int s(int n) {
    if (n <= 0)
        return -1;

    int total = 0;
    for (int i = 1; i < n + 1; ++i) {
        if (i % 2 == 0)
            total += i;
    }
    return total;
}

int main(void) {
    int n = 100000;

    printf("%d\n", s(n));
    printf("%d\n", (n / 2) * ((n / 2) + 1));
    
    return 0;
}
