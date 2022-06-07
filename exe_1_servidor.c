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

int connect_client2()
{
    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE];
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(5000);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));
    if (con < 0)
    {
        printf("\nNão foi possível conectar.\n");
        close(sockid);
    }
    else
    {
        printf("\nConexão realizada: %d na porta %d \n", con, 5000);
        send(sockid, "Ola\n", strlen("Ola\n"), 0);
        recv(sockid, buffer, BUFFER_SIZE-1, 0);
        printf("%s", buffer);
        close(sockid);
    }
}

int main(void)
{
    int sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);

    if (sockid < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    int opcao = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    // Determinar qual porta o servidor irá atuar
    struct sockaddr_in servidor;
    bzero(&servidor, sizeof(servidor));

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(5002);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockid, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
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
            printf("Conexão estabelecida. - socket 1\n");
        }

        // Criar processo filho
        int pid = fork();

        if (pid < 0)
        {
            printf("\nErro ao criar processo filho\n");
        }
        if (pid == 0)
        {
            close(sockid);
            send(newSocket_1, "Olá, eu sou o servidor\n", strlen("Olá, eu sou o servidor\n"), 0);
            connect_client2();
            close(newSocket_1);
            exit(0);
        } // if do filho
        else
        { // Processo pai
            close(newSocket_1);
        }
    }
}