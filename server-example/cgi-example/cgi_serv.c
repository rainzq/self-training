#include <stdlib.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "rio.h"
#include "sockfd.h"

#define MAXBUF 4096

extern char** environ;


void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];
    rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n") != 0) {
        fprintf(stderr, "[line]: %s\n", buf);
        
        rio_readlineb(rp, buf, MAXLINE);
    }
    return;
}

void clienterror(int fd, char *cause, char *errnum, char *shortmsg, char *longmsg)
{
    char buf[MAXLINE], body[MAXBUF];
    sprintf(body, "<html><title>Server Error</title>");
    sprintf(body, "%s<body bgcolor=" "ffffff" ">\r\n", body);
    sprintf(body, "%s%s: %s\r\n", body, errnum, shortmsg);
    sprintf(body, "%s<p>%s: %s\r\n", body, longmsg, cause);
    sprintf(body, "%s<hr><em>The Tiny Web Server</em>\r\n", body);
    
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-length: %d\r\n\r\n", strlen(body));
    rio_writen(fd, buf, strlen(buf));
    rio_writen(fd, body, strlen(body));
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;
    if (!strstr(uri, "cgi-bin")) {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri)-1] == '/') {
            strcat(filename, "home.html");
        }
        return 1;
    } else {
        ptr = strchr(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        } else {
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

void get_filetype(char *filename, char *filetype)
{
    if (strstr(filename, ".html")) {
        strcpy(filetype, "text/html");
    } else if (strstr(filename, ".gif")) {
        strcpy(filetype, "image/gif");
    } else if (strstr(filename, ".jpg")) {
        strcpy(filetype, "image/jpeg");
    } else {
        strcpy(filetype, "text/plain");
    }
}

void server_static(int fd, char *filename, int filesize)
{
    fprintf(stderr, "Enter static server...\n");

    int srcfd;
    char *srcp, filetype[MAXLINE], buf[MAXBUF];
    
    get_filetype(filename, filetype);
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    sprintf(buf, "%sServer: Web Server\r\n", buf);
    sprintf(buf, "%sContent-type: %s\r\n", buf, filetype);
    sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, filesize);
    rio_writen(fd, buf, strlen(buf));
    
    srcfd = open(filename, O_RDONLY, 0);
    srcp = mmap(0, filesize, PROT_READ, MAP_PRIVATE, srcfd, 0);
    close(srcfd);
    rio_writen(fd, srcp, filesize);
    munmap(srcp, filesize);
}

void server_dynamic(int fd, char *filename, char *cgiargs)
{
    fprintf(stderr, "Enter dynamic server...\n");
    
    char buf[MAXLINE], *emptylist[] = {NULL};
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Web Server\r\n");
    rio_writen(fd, buf, strlen(buf));
    fprintf(stderr, "fork a child...\n");
    if (fork() == 0) { /* child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(fd, STDOUT_FILENO);
        execve(filename, emptylist, environ);
    }
    wait(NULL);
}


void doit(fd) 
{
    int is_static;
    struct stat sbuf;
    char filename[MAXLINE], cgiargs[MAXLINE];
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    rio_t rio;
    rio_readinitb(&rio, fd);
    rio_readlineb(&rio, buf, MAXLINE);
    sscanf(buf, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET") != 0) {
        clienterror(fd, method, "501", "Not Implemented", "Server does not implement this method");
        return;
    }
    fprintf(stderr, "read request headers...\n");
    
    read_requesthdrs(&rio);
    fprintf(stderr, "parse uri...\n");
    is_static = parse_uri(uri, filename, cgiargs);
    fprintf(stderr, "file stat...\n");
    if (stat(filename, &sbuf) < 0) {
        clienterror(fd, filename, "404", "Not found", "Server couldn't find this file");
        return;
    }
    
    if (is_static) {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Server couldn't read the file");
            return;
        }
        server_static(fd, filename, sbuf.st_size);
    } else {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IXUSR & sbuf.st_mode)) {
            clienterror(fd, filename, "403", "Forbidden", "Server couldn't run the CGI program");
            return;
        }
        server_dynamic(fd, filename, cgiargs);
    }
}


int main(int argc, char *argv[])
{
    int listenfd, connfd,  clientlen;
    struct sockaddr_in clientaddr;
    int port = 9527;

    listenfd = open_listenfd(port);
    while (1) {
        clientlen = sizeof(clientaddr);
        fprintf(stderr, "begin accept...\n");
        connfd = accept(listenfd, (struct sockaddr *)&clientaddr, &clientlen);
        fprintf(stderr, "begin process...\n");
        doit(connfd);
        fprintf(stderr, "finished process...\n");
        close(connfd);
        fprintf(stderr, "again...\n");
    }
    return 0;
}
