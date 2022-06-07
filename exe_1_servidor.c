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

int connect_client2(char *ip,  char *port)
{
    //nc -l -p 5000
    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE];
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &sock.sin_addr);

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
            printf("\nProcesso filho criado\n");

            // ------------------------ Mensagem de boas vindas ao cliente 1
            close(sockid);
            char welcome[100] = "\n************************\nMsg do servidor: Cliente 1 conectado\n*************************\n";
            send(newSocket_1, welcome, strlen(welcome), 0);
          //  bzero(&welcome, sizeof(welcome));
            //welcome[0] = '\0';
            // ----------------------------------------------------------------------------------

            // ------------------------ Recebe o IP do cliente 2 e envia de volta ao cliente 1
            char connect1[100] = "\n*******\nDigite o ip com que deseja se conectar:\n*******\n";
            send(newSocket_1, connect1, strlen(connect1), 0);
           // bzero(&connect1, sizeof(connect1));
            connect1[0] = '\0';
            char get_ip[20];
            char get_concat[500];
            get_concat[0] = '\0';
           // bzero(&get_ip, sizeof(get_ip));
            recv(newSocket_1, get_ip, sizeof(get_ip), 0);
            snprintf(get_concat, sizeof(get_concat), "\n\n****Msg do servidor: endereço recebido -> %s\n", get_ip);
            send(newSocket_1, get_concat, strlen(get_concat), 0);
            get_concat[0] = '\0';
         //  bzero(&get_concat, sizeof(get_concat));
            int control = 1;
            printf("\n------> Checkpoint 1\n");
            
            //  ---------------------------------------------------------------------------------

            // ------------------------ Recebe a porta do cliente 2 e envia de volta ao cliente 1
            if(control == 1);
            {
                printf("\n -----> Checkpoint 2\n");
                char connect2[100] = "\nDigite a porta do cliente 2:\n";
                printf("\n -----> Checkpoint 3\n");
                printf("connect2 %s", connect2);
                send(newSocket_1, connect2, strlen(connect2), 0);
                bzero(connect2, sizeof(connect2));
                printf("\n -----> Checkpoint 4\n");
                char get_port[20];
                bzero(get_port, sizeof(get_port));
                bzero(get_concat, sizeof(get_concat));
                recv(newSocket_1, get_port, sizeof(get_port), 0);
                snprintf(get_concat, sizeof(get_concat), "\n\n****Msg do servidor: porta recebida -> %s\n", get_port);
                send(newSocket_1, get_concat, strlen(get_concat), 0);
                bzero(get_concat, sizeof(get_concat));
                connect_client2(get_ip, get_port);
            }
            
            //  ---------------------------------------------------------------------------------

            

            //close(newSocket_1);
            exit(0);
        } // if do filho
        else
        { // Processo pai
            close(newSocket_1);
        }
    }
}