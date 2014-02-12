#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#if 0
int hex2int(const char *p, int n)
{
    int i;
    int r = 0;
    int t = 0;
    
    
    if (p == NULL || n == 0) {
        return 0;
    }
    for (i = 0; i < n; i++) {
        switch(*(p + i)) {
        case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
            t = (*(p + i)) - '0';
            break;
        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
            t = (*(p + i)) - 'a' + 10;
            break;
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
            t = (*(p + i)) - 'A' + 10;
            break;
        default:
            break;
        }
        r = r * 16 + t;
    }
    return r;
}

int another_main(int argc, char *argv[])
{
    if (argc < 2) {
        printf("please input a hex number\n");
        return 0;
    } else if (argc == 2) {
        const char *str = argv[1];
        const char *p = strstr(str, "0x");
        int arr[4] = {0};
        
        if (p == NULL) {
            printf("The number is not a hex number\n");
            return 0;
        } else {
            p = p + 2;
            size_t n = strlen(p), i = n, j = 3;
            int t;
            while (i > 0) {
                if (i - 1 > 0) {
                    t = hex2int(p + i - 2, 2);
                    i -= 2;
                } else {
                    t = hex2int(p + i - 1, 1);
                    i -= 1;
                }
                arr[j--] = t;
            }
        }
        printf("%d.%d.%d.%d\n", arr[0], arr[1], arr[2], arr[3]);
        
    }
    return 0;
}
#endif

int main(int argc, char *argv[])
{
    struct in_addr sa;
    unsigned int addr;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <hex number>\n", argv[0]);
        return 0;
    }
    sscanf(argv[1], "%x", &addr);
    sa.s_addr = htonl(addr);
    printf("%s\n", inet_ntoa(sa));
    return 0;
}
