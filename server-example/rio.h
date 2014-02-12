#ifndef __RIO_H
#define __RIO_H

#include <unistd.h>
#include <errno.h>
#include <string.h>

#define RIO_BUFSIZE 8192
typedef struct 
{
    int rio_fd;
    int rio_cnt;
    char *rio_bufptr;
    char rio_buf[RIO_BUFSIZE];
}rio_t;

void rio_readinitb(rio_t *rp, int fd)
{
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, char *userbuf, size_t n)
{
    int cnt;
    
    while (rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) {
                return -1;
            }
        }
        else if (rp->rio_cnt == 0) {
            return 0;
        } 
        else {
            rp->rio_bufptr = rp->rio_buf;
        }
    }
    
    cnt = n;
    if (rp->rio_cnt < n) {
        cnt = rp->rio_cnt;
    }
    memcpy(userbuf, rp->rio_bufptr, cnt);
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}

ssize_t rio_readnb(rio_t *rp, char *userbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = userbuf;
    
    while (nleft > 0) {
        if ((nread = rio_read(rp, bufp, nleft)) < 0) {
            if (errno == EINTR) {    /* interrupted by sig handler return */
                nread = 0;           /* call read() again */
            } else {                 /* error */
                return -1;
            }
        }
        else if (nread == 0) break;  /* EOF */
        else {
            nleft -= nread;
            bufp += nread;
        }
    }
    return (n - nleft);
}

ssize_t rio_readlineb(rio_t *rp, char *userbuf, size_t maxlen)
{
    char c, *bufp;
    int n, rc;
    bufp = userbuf;
    
    for (n = 1; n < maxlen; n++) {     /* filled at most (maxlen - 1) charactors */
        if ((rc = rio_read(rp, &c, 1)) == 1) {
            *bufp++ = c;
            if (c == '\n') break;
        } else if (rc == 0) {          /* EOF */
            if (n == 1) {              /* no data read */
                return 0;
            } else break;              /* some data was read */
        } else {                       /* error */
            return -1;
        }
    }
    
    *bufp = '\0';
    return n;
}


ssize_t rio_readn(int fd, void *userbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nread;
    char *bufp = userbuf;
    
    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nread = 0;
            } else {
                return -1;
            }
        } else if (nread == 0) break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

ssize_t rio_writen(int fd, void *userbuf, size_t n)
{
    size_t nleft = n;
    ssize_t nwrite;
    char *bufp = userbuf;
    
    while (nleft > 0) {
        if ((nwrite = write(fd, bufp, nleft)) < 0) {
            if (errno == EINTR) {
                nwrite = 0;
            } else {
                return -1;
            }
        }
        nleft -= nwrite;
        bufp += nwrite;
    }
    return n;
}
#endif
