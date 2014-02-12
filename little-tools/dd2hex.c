#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct in_addr sa;
    
    if (argc != 2) {
        fprintf(stderr, "usage: %s <dotted-decimal ip>\n", argv[0]);
        return 0;
    }
    inet_aton(argv[1], &sa);
    printf("0x%x\n", ntohl(sa.s_addr));
    return 0;
    
}

