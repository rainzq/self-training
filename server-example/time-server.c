#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <time.h>

const int N = 1024;

int main(int argc, char **argv) {
    int fd, accept_fd;
    struct sockaddr_in addr;
    struct sockaddr_in c_addr;
    char buff[N];
    time_t ticks;
    socklen_t len = sizeof(c_addr);

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9527);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Cannot bind");
        return -1;
    }
    listen(fd, 1024);
    while (1) {
        if ((accept_fd = accept(fd, (struct sockaddr *)&c_addr, &len)) < 0) {
            perror("accept error");
            return -1;
        }
        ticks = time(NULL);
        snprintf(buff, N, "%.24s\r\n", ctime(&ticks));
        write(accept_fd, buff, strlen(buff));
        close(accept_fd);
    }
    return 0;
}

