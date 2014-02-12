#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "rio.h"

#define N 4096

static void doit(int fd);
static int open_listenfd(int port);
static void sig_child(int sig);
static void sig_pipe(int sig);

int main()
{
    int listenfd, connfd;
    signal(SIGCHLD, SIG_IGN);
    // signal(SIGCHLD, sig_child);
    listenfd = open_listenfd(9527);
    while (1) {
        connfd = accept(listenfd, NULL, NULL);
        if (fork() == 0) { // child process
            close(listenfd);
            doit(connfd);
            close(connfd);
            return 0;
        } else { // parent process
            close(connfd);
        }
    }
    return 0;
}

void doit(int fd)
{
    char buff[N];
    int n;
    pid_t pid = getpid();
    rio_t rio;
    rio_readinitb(&rio, fd);
    while ((n = rio_readlineb(&rio, buff, N)) > 0) {
        // printf("recv %d bytes\n", n);
        snprintf(buff + n - 1, N - n, "pid %d server receive %d bytes from client", (int)pid, n);
        fputs(buff, stdout);
        // printf("fputs done\n");
        memset(buff, 0, N);
        // printf("memset done\n");
        snprintf(buff, N, "pid %d server send bytes to client\n", (int)pid);
        // printf("snprintf done\n");
        fputs(buff, stdout);
        // printf("fputs done\n");
        rio_writen(fd, buff, strlen(buff));
        // printf("write to client done\n");
    }
}

int open_listenfd(int port)
{
    int listenfd;
    struct sockaddr_in addr;
    socklen_t len;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    len = sizeof(addr);
    if (bind(listenfd, (struct sockaddr *)&addr, len) < 0) {
        perror("cannot bind");
        return -1;
    }
    listen(listenfd, 1024);
    return listenfd;
}

void sig_child(int sig)
{
    pid_t pid;
    int stat;

    pid = wait(&stat);
    printf("child %d terminated\n", pid);
    return;
}

void sig_pipe(int sig)
{
    printf("broken pipe\n");
    return;
}
