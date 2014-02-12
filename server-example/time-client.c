#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>

const int N = 1024;

int main(int argc, char **argv) {
    int fd, n;
    char buff[N+1];
    struct sockaddr_in addr;
    if (argc != 2) {
        printf("usage app <ip_address>");
        return -1;
    }
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9527); // time server port
    inet_pton(AF_INET, argv[1], &addr.sin_addr);
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Cannot connect");
        return -1;
    }
    while ((n = read(fd, buff, N)) > 0) {
        buff[n] = '\0';
        if (fputs(buff, stdout) == EOF) {
            perror("fputs error");
            return -1;
        }
    }
    return 0;
}
