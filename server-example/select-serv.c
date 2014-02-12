#include <sys/select.h>
#include "rio.h"
#include "sockfd.h"


void echo(int fd)
{
    size_t n;
    char buf[MAXLINE];
    rio_t rio;
    
    rio_readinitb(&rio, fd);
    while((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
        printf("server received %d bytes\n", n);
        memset(buf, 0, MAXLINE);
        snprintf(buf, sizeof(buf), "server roger that\n");
        rio_writen(fd, buf, strlen(buf));
    }
    
}

void command()
{
    char buf[MAXLINE];
    if ((fgets(buf, MAXLINE, stdin)) != NULL) {
        printf("hello, you input: %s\n", buf);
        return;
    }
}

int main(int argc, char *argv[])
{
    int listenfd, connfd;
    fd_set readset;
    int maxfd, i, nready, clientlen;
    struct sockaddr_in client_addr;
    int port = 9527;
    
    clientlen = sizeof(client_addr);
    listenfd = open_listenfd(port);
    maxfd = listenfd;
    FD_ZERO(&readset);
    while (1) {
        FD_SET(STDIN_FILENO, &readset);
        FD_SET(listenfd, &readset);

        fprintf(stderr, "selecting...\n");
        
        nready = select(maxfd + 1, &readset, NULL, NULL, NULL);
        if (nready < 0) { /* error */
            return -1;
        } else if (nready == 0) { /* timeout */
            fprintf(stderr, "time out\n");
            continue;
        } else {
            fprintf(stderr, "selected.\n");
            if (FD_ISSET(listenfd, &readset)) {
                connfd = accept(listenfd, (struct sockaddr *)&client_addr, &clientlen);
                echo(connfd);
            } 
            if (FD_ISSET(STDIN_FILENO, &readset)) {
                command();
            }
        }
    }
    return 0;
}
