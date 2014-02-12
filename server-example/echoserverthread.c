#include <stdlib.h>
#include <pthread.h>
#include "rio.h"
#include "sockfd.h"
#include <poll.h>
#include <semaphore.h>

typedef struct sbuf_t
{
    int *buf;
    int n;
    int front;
    int tail;
    sem_t mutex;
    sem_t slots;
    sem_t items;
}sbuf_t;
sbuf_t sbuf;

void P(sem_t *s)
{
    sem_wait(s);
}

void V(sem_t *s)
{
    sem_post(s);
}


void sbuf_init(sbuf_t *sp, int n)
{
    sp->buf = malloc(n * sizeof(int));
    sp->n = n;
    sp->front = sp->tail = 0;
    sem_init(&sp->mutex, 0, 1);
    sem_init(&sp->slots, 0, n);
    sem_init(&sp->items, 0, 0);
}

void sbuf_deinit(sbuf_t *sp)
{
    free(sp->buf);
}

void sbuf_insert(sbuf_t *sp, int item)
{
    P(&sp->slots);
    P(&sp->mutex);
    sp->buf[(sp->tail+1)%sp->n] = item;
    V(&sp->mutex);
    V(&sp->items);
}

int sbuf_remove(sbuf_t *sp)
{
    int item;
    P(&sp->items);
    P(&sp->mutex);
    item = sp->buf[(sp->front+1)%sp->n];
    V(&sp->mutex);
    V(&sp->slots);
    return item;
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

void *thread(void *p)
{
    int connfd;
    pthread_detach(pthread_self());
    connfd = sbuf_remove(&sbuf);
    echo(connfd);
    close(connfd);
    return NULL;
}



int main(int argc, char *argv[])
{
    int i, listenfd, connfd, clientlen, nready;
    pthread_t tid;
    struct pollfd client;
    struct sockaddr_in clientaddr;
	int thread_num = 10;
    
    clientlen = sizeof(clientaddr);
    
    sbuf_init(&sbuf, 100);
    
    listenfd = open_listenfd(9527);
    client.fd = listenfd;
    client.events =  POLLIN;
    
    for (i = 0; i < thread_num; i++) {
        pthread_create(&tid, NULL, thread,  NULL);
    }
    
    while (1) {
        nready = poll(&client, 1, -1);
        if (nready > 0 && (client.revents & POLLIN)) {
            connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
            sbuf_insert(&sbuf, connfd);
        }
    }
    return 0;
}
