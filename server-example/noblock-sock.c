#include   <sys/types.h>  
#include   <sys/socket.h>  
#include   <arpa/inet.h>  
#include   <netinet/in.h>  
#include   <sys/param.h>  
#include   <sys/types.h>  
#include   <errno.h>  
#include   <sys/stat.h>  
#include   <stdio.h>  
#include   <stdlib.h>  
#include   <unistd.h>  
#include   <stropts.h>  
#include   <string.h>  
#include   <pthread.h>  
#include   <strings.h> 
#include   <signal.h> 
#include   <fcntl.h>
#include   <unistd.h>

int main()
{
    int   sock;
      
    struct   sockaddr_in   sin;
      
       
    sock   =   socket(AF_INET,   SOCK_STREAM,   0);
      
    if   (sock   ==   -1)  
    {
          
        perror("client   socket");
          
        return   -1;
          
    }
      
       
    memset(&sin,   0,   sizeof(sin));
      
    sin.sin_family   =   AF_INET;
      
    sin.sin_addr.s_addr   =   inet_addr("127.0.0.1");
      
    sin.sin_port   =   htons(8910);
      
    int flags;
    
    if ((flags = fcntl(sock, F_GETFL)) == -1) {
        
        
        close(sock);
        
        return -1;
        
    }
    

    flags |= O_NONBLOCK;
    

    if (fcntl(sock, F_SETFL, flags) == -1) {
        close(sock);
        return -1;
        
    }
    
    if   (connect(sock,   (struct   sockaddr   *)&sin,   sizeof(sin))   ==   -1)  
    {
          
        perror("client   connect   error");
          
        return   -1;
          
    }
    return 0;
    
}
