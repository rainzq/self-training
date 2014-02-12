#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    int flags, n, error, sockfd;
    socklen_t len;
    fd_set rset, wset;
    struct sockaddr_in sa;
    struct timeval tval;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(9527);
    inet_pton(AF_INET, argv[1], &sa.sin_addr);

    flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    
    error = 0;
    if ((n = connect(sockfd, (struct sockaddr *)&sa, sizeof(sa))) < 0) {
        cerr << "connect not ok" << endl;
        if (errno != EINPROGRESS) {
            cout << "in progress" << endl;
            return -1;
        }
    }

    if (n == 0) {
        goto done;
    }

    FD_ZERO(&rset);
    FD_SET(sockfd, &rset);
    wset = rset;
    tval.tv_sec = 1;
    tval.tv_usec = 0;

    if ((n = select(sockfd, &rset, &wset, NULL, &tval)) == 0) {
        cerr << "time out" << endl;
        close(sockfd);
        errno = ETIMEDOUT;
        return -1;
    }

    if (FD_ISSET(sockfd, &rset) || FD_ISSET(sockfd, &wset)) {
        len = sizeof(error);
        if (getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &error, &len) < 0) 
            cerr << "connect error" << endl;
            return -1;
    } else {
        perror("select error: sockfd not set");
    }

done:
    fcntl(sockfd, F_SETFL, flags);
    if (error) {
        close(sockfd);
        errno = error;
        return -1;
    }
    cout << "connected OK" << endl;
    return 0;
}
