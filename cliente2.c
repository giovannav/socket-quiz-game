#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>


#define BUFFER_SIZE 1024
#define QUESTION_SIZE 2048

int main(void)
{
    // nc -l -p 5000

    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE];
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);
    char buffer_recv[BUFFER_SIZE];

    if (bind(sockid, (struct sockaddr *)&sock, sizeof(sock)) < 0)
    {
        printf("Erro ao fazer o bind.\n");
        close(sockid);
        exit(1);
    }

    listen(sockid, 5);

    while (1)
    {
        int newSocket_1;
        struct sockaddr_in cliente1;
        bzero(&cliente1, sizeof(cliente1));
        int c1 = sizeof(struct sockaddr_in);

        // Nessa linha o servidor ficará parado esperando a conexão de um cliente
        newSocket_1 = accept(sockid, (struct sockaddr *)&cliente1, (socklen_t *)&c1);
        if (newSocket_1 < 0)
        {
            printf("Erro ao aceitar conexão. - socket 1\n");
        }
        else
        {
            while (recv(sockid, buffer_recv, BUFFER_SIZE, 0) != 0)
            {
               if(strcmp(buffer_recv, "Ola") == 0)
               {
                    printf("%s", buffer_recv);
               }
                
            }            
        }

    }
    
}