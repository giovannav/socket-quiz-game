#include <stdio.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <locale.h>

#define BUFFER_SIZE 1024
#define QUESTION_SIZE 2048

int main(void)
{
    // nc -l -p 5000

    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE] = {0};
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5003);
    sock.sin_addr.s_addr = htonl(INADDR_ANY);
    char buffer_recv[BUFFER_SIZE] = {0};

    if (sockid < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    int opcao = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    if (bind(sockid, (struct sockaddr *)&sock, sizeof(sock)) < 0)
    {
        printf("Erro ao fazer o bind.\n");
        close(sockid);
        exit(1);
    }

    listen(sockid, 5);

    while (1)
    {
        char buffer_recv[BUFFER_SIZE] = {0};
        char buffer_send[BUFFER_SIZE] = {0};
        int newSocket_1;
        struct sockaddr_in cliente1;
        bzero(&cliente1, sizeof(cliente1));
        int c1 = sizeof(struct sockaddr_in);

        // Nessa linha o cliente ficará parado esperando a conexão de um cliente
        newSocket_1 = accept(sockid, (struct sockaddr *)&cliente1, (socklen_t *)&c1);
        if (newSocket_1 < 0)
        {
            printf("Erro ao aceitar conexão. - socket 1\n");
        }
        else
        {
            while (recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0) != 0)
            {
                if (buffer_recv[0] == '3')
                {
                    printf("\n%s", buffer_recv);
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if (buffer_recv[0] == '4')
                {
                   // printf("Buffer valor buffer: \n%s\n", buffer_recv);
                   // printf("Buffer: \n%s\n", buffer_recv);
                    bzero(buffer_send, sizeof(buffer_send));
                    send(newSocket_1, "ready", strlen("ready"), 0);

                    for (int i = 0; i < 3; i++)
                    {
                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                     //   fflush(stdin);
                     //   printf("Pergunta %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                     //   fflush(stdin);
                     //   printf("Resposta 1: %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                     //   fflush(stdin);
                     //   printf("Resposta 2: %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                      //  fflush(stdin);
                      //  printf("Resposta 3: %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                      //  fflush(stdin);
                      //  printf("Resposta 4: %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                      //  fflush(stdin);
                      //  printf("Resposta certa: %s\n", buffer_send);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));
                    }
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if(buffer_recv[0] == '5')
                {      
                    printf("\n%s", buffer_recv);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                    recv(newSocket_1, buffer_recv, sizeof(buffer_recv), 0);
                    printf("\n%s\n", buffer_recv);
                    send(newSocket_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if(buffer_recv[0] == '6')
                {      
                    printf("\n%s", buffer_recv);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                    recv(newSocket_1, buffer_recv, sizeof(buffer_recv), 0);
                    printf("\n%s\n", buffer_recv);
                    send(newSocket_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if(buffer_recv[0] == '7')
                {      
                    printf("\n%s", buffer_recv);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                    recv(newSocket_1, buffer_recv, sizeof(buffer_recv), 0);
                    printf("\n%s\n", buffer_recv);
                    send(newSocket_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
            }
        }
    }
    close(sockid);
}