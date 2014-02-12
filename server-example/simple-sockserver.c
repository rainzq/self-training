#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include "rio.h"

#define MAXLINE 4096

int open_clientfd(char *hostname, int port)
{
    int clientfd;
    struct hostent *hp;
    struct sockaddr_in servaddr;
    
    if ((clientfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if ((hp = gethostbyname(hostname)) == NULL) {
        return -1;
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    memcpy((void *)&servaddr.sin_addr.s_addr, (void *)hp->h_addr, hp->h_length);
    
    if (connect(clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        return -1;
    }
    return clientfd;
}


int open_listenfd(int port)
{
    int listenfd;
    struct sockaddr_in servaddr;
    int on = 1;
    
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        return -1;
    }
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0) {
        return -1;
    }
    
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_port = htons(port);
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        return -1;
    }
    if (listen(listenfd, 10) < 0) {
        return -1;
    }
    return listenfd;
}


int client_func(int argc, char *argv[])
{
    int clientfd;
    rio_t rio;
    char buf[MAXLINE];
    int port = 9527;
    
    
    if (argc == 2) {
        clientfd = open_clientfd(argv[1], port);
    } else {
        clientfd = open_clientfd("127.0.0.1", port);
    }

    rio_readinitb(&rio, clientfd);
    while (fgets(buf, MAXLINE, stdin) != NULL) {
        rio_writen(clientfd, buf, strlen(buf));
        rio_readlineb(&rio, buf, MAXLINE);
        fputs(buf, stdout);
    }
    close(clientfd);
    return 0;
}

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


int main(int argc, char *argv[])
{
    int listenfd, connfd, clientfd, clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;
    rio_t rio;
    char buf[MAXLINE];
    int port = 9527;
    
    listenfd = open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        hp = gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, sizeof(clientaddr.sin_addr.s_addr), AF_INET);
        haddrp = inet_ntoa(clientaddr.sin_addr);
        printf("server connected to %s (%s)\n", hp->h_name, haddrp);
        echo(connfd);
        close(connfd);
    }
    return 0;
}
