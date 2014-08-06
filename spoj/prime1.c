#include <stdio.h>
#include <math.h>

int is_prime(int n)
{
    if (n & 0x1 == 0 || n % 3 == 0) {
        return 0;
    }
    int limit = (int)sqrt(n);

}

