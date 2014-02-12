#include <sys/select.h>
#include <stdlib.h>
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

struct pool 
{
    int maxfd; /* the max fd */
    int maxi; /* highest index into client array */
    fd_set readset;
    fd_set readyset; 
    int nready;
    int clientfds[FD_SETSIZE];
    rio_t clientrios[FD_SETSIZE];
};

 
void pool_init(struct pool *p, int fd)
{
    int i;
    p->maxfd = fd;
    p->maxi = -1;
    p->nready = -1;
    for (i = 0; i < FD_SETSIZE; ++i) {
        p->clientfds[i] = -1;
    }
    FD_ZERO(&p->readset);
    FD_SET(fd, &p->readset);
    p->readyset = p->readset;
}

void addclient(struct pool *p, int fd)
{
    int i;
    p->nready--; /* exclude listenfd */
    for (i = 0; i < FD_SETSIZE; ++i) {  /* find a available slot */
        if (p->clientfds[i] < 0) {
            p->clientfds[i] = fd;
            rio_readinitb(&p->clientrios[i], fd);
            FD_SET(fd, &p->readset);
            if (p->maxfd < fd) {
                p->maxfd = fd;
            }
            if (p->maxi < i) {
                p->maxi = i;
            }
            break;
        }
    }
    if (i == FD_SETSIZE) {
        fprintf(stderr, "outof fd range\n");
        exit(0);
    }
}

void checkclient(struct pool *p)
{
    int i, n, fd;
    char buf[MAXLINE];
    rio_t rio;
    
    for (i = 0; (i <= p->maxi) && (p->nready > 0); i++) {
        fprintf(stderr, "in checkclient\n");
        
        fd = p->clientfds[i];
        rio = p->clientrios[i];
        if (fd > 0 && (FD_ISSET(fd, &p->readyset))) {
            fprintf(stderr, "active client\n");
            p->nready--;
            if ((n = rio_readlineb(&rio, buf, MAXLINE)) != 0) {
                printf("server received %d bytes\n", n);
                printf("%s", buf);
                memset(buf, 0, MAXLINE);
                snprintf(buf, sizeof(buf), "server roger that %d\n", fd);
                rio_writen(fd, buf, strlen(buf));
            } else {
                /* client closed connection */
                FD_CLR(fd, &p->readset);
                close(fd);
                p->clientfds[i] = -1;
            }
        }
    }
}


int main(int argc, char *argv[])
{
    int listenfd, connfd;
    int i, clientlen;
    struct pool pool_;
    struct sockaddr_in client_addr;
    int port = 9527;
    
    clientlen = sizeof(client_addr);
    listenfd = open_listenfd(port);

    pool_init(&pool_, listenfd);
    while (1) {
        fprintf(stderr, "selecting...\n");
        pool_.readyset = pool_.readset;
        
        pool_.nready = select(pool_.maxfd + 1, &pool_.readyset, NULL, NULL, NULL);
        if (FD_ISSET(listenfd, &pool_.readyset)) {
            fprintf(stderr, "accepting...\n");
            connfd = accept(listenfd, (struct sockaddr *)&client_addr, &clientlen);
            fprintf(stderr, "adding client...\n");
            addclient(&pool_, connfd);
        }
        fprintf(stderr, "checking client...\n");
        checkclient(&pool_);
    }

    return 0;
}
