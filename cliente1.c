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

struct cliente2
{
    char cliente2_ip[20], cliente2_port[20];
};

typedef struct cliente2 Cliente2;

Cliente2 welcome()
{

    Cliente2 c2 = {0};
    char get_ip[20] = {0};
    char get_port[20] = {0};
    bzero(get_ip, 20);
    bzero(get_port, 20);

    printf("******************************");
    printf("\nBem vindo ao cliente_1.c\n");
    printf("\nCertifique-se que o cliente 2 também está online.\n");
    printf("\nPressione qualquer tecla para continuar.\n");
    printf("******************************");
    getchar();
    printf("\n*******\nDigite o IP com que deseja se conectar:\n*******\n");
    scanf("%s", get_ip);
    fflush(stdin);
    __fpurge(stdin);
    printf("\n*******\nDigite a porta com que deseja se conectar:\n*******\n");
    scanf("%s", get_port);
    __fpurge(stdin);
    fflush(stdin);
    strcpy(c2.cliente2_ip, get_ip);
    strcpy(c2.cliente2_port, get_port);
    return c2;
}

int main(void)
{

    struct sockaddr_in sock;
    int con, sockid;
    int port = 5004;
    char ip_cliente2[20] = {0};
    char port_cliente2[20] = {0};
    char buffer_recv[BUFFER_SIZE] = {0};
    char buffer_send[BUFFER_SIZE] = {0};
    struct in_addr addr;
    sockid = socket(AF_INET, SOCK_STREAM, 0); // criando o socket
    bzero(&(sock), sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);
    Cliente2 c2_main = welcome();

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0)
    {
        printf("\nErro ao conectar\n");
        close(sockid);
    }

    bzero(buffer_recv, sizeof(buffer_recv));
    while (recv(sockid, buffer_recv, BUFFER_SIZE, 0) != 0)
    {
        // printf("\n%s\n", buffer_recv);
        if (strcmp(buffer_recv, "1") == 0)
        {
            strcpy(buffer_send, c2_main.cliente2_ip);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        if (strcmp(buffer_recv, "2") == 0)
        {
            strcpy(buffer_send, c2_main.cliente2_port);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        if (buffer_recv[0] == '3')
        {
            printf("\n%s", buffer_recv);
            scanf("%s", buffer_send);
            __fpurge(stdin);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        if (buffer_recv[0] == '4')
        {

            // printf("Buffer: \n%s\n", buffer_recv);
            bzero(buffer_send, sizeof(buffer_send));
            send(sockid, "ready", strlen("ready"), 0);

            for (int i = 0; i < 3; i++)
            {
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Pergunta %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Resposta 1: %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Resposta 2: %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Resposta 3: %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Resposta 4: %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                // fflush(stdin);
                // printf("Resposta certa: %s\n", buffer_send);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));
            }
        }

        /* recv(sockid, buffer_recv, BUFFER_SIZE, 0);*/
        if (buffer_recv[0] == '5')
        {
            printf("\n%s", buffer_recv);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            scanf("%s", buffer_send);
            __fpurge(stdin);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        if (buffer_recv[0] == '6')
        {
            printf("\n%s", buffer_recv);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            scanf("%s", buffer_send);
            __fpurge(stdin);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }

        bzero(buffer_recv, sizeof(buffer_recv));
        bzero(buffer_send, sizeof(buffer_send));
    }
    close(sockid);
}