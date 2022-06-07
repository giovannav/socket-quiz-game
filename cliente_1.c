#include <stdio.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main(void)
{
    struct sockaddr_in sock;
    int con, sockid;
    int port = 5002;
    char buffer[BUFFER_SIZE];
    char get_concat[500];
    struct in_addr addr;

    sockid = socket(AF_INET, SOCK_STREAM, 0); //criando o socket
    
    bzero(&(sock), sizeof(sock));    
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0){
        printf("\nErro ao conectar\n");
    }
    else{
        printf("\nConectado\n");
    }
    bzero(buffer, BUFFER_SIZE);

    bzero(buffer, BUFFER_SIZE);
    while(recv(sockid, buffer, BUFFER_SIZE-1, 0) != 0)
    {
        printf("%s", buffer);
        if(!strcmp(buffer, "Digite a hora:\n\n"))
        {
            //printf("%s", buffer);
            bzero(buffer, BUFFER_SIZE);
            scanf("%s", buffer);
            send(sockid, buffer, strlen(buffer), 0);
            bzero(buffer, BUFFER_SIZE);
            printf("%s", buffer);
           // recv(sockid, buffer, BUFFER_SIZE-1, 0);
        }
        
    }
    close(sockid);
}