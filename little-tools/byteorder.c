#include <stdio.h>

int main()
{
    union {
        short s;
        char cs[sizeof(short)];
    }u;
    u.s = 0x0102;
    if (u.cs[0] == 0x01 && u.cs[1] == 0x02) {
        printf("big-endian\n");
    } else if (u.cs[0] == 0x02 && u.cs[1] == 0x01) {
        printf("little-endian\n");
    } else {
        printf("unkown\n");
    }
    return 0;
}
