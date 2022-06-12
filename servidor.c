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

int connect_client2(char *ip, char *port)
{
    // nc -l -p 5000
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
        recv(sockid, buffer, BUFFER_SIZE - 1, 0);
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
    servidor.sin_port = htons(5003);
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
            //  printf("\nProcesso filho criado\n");
            // ------------------------ Mensagem de boas vindas ao cliente 1
            close(sockid);
            char con_info[9];
            bzero(con_info, sizeof(con_info));
            char msg[200];
            bzero(msg, sizeof(msg));
            char msg_received[200];
            bzero(msg_received, sizeof(msg_received));
            char get_ip[20], get_port[20];

            // ---- Recebe o IP do cliente 2 e envia de volta ao cliente 1
            strcpy(msg, "1");
            printf("\nMensagem: %s\n", msg);
            send(newSocket_1, msg, strlen(msg), 0);
            recv(newSocket_1, con_info, sizeof(con_info), 0);
            printf("\nEndereço IP recebido: %s", con_info);
            strcpy(get_ip, con_info);
            bzero(con_info, sizeof(con_info));
            bzero(msg, sizeof(msg));
            //  ------------------------------

            // ---- Recebe a porta do cliente 2 e envia de volta ao cliente 1
            strcpy(msg, "2");
            printf("\nMensagem: %s\n", msg);
            send(newSocket_1, msg, strlen(msg), 0);
            recv(newSocket_1, con_info, sizeof(con_info), 0);
            printf("\nEndereço de porta recebido: %s", con_info);
            strcpy(get_port, con_info);
            bzero(con_info, sizeof(con_info));
            bzero(msg, sizeof(msg));
            //  ------------------------------

            // ---- Chama a conexão para a porta 2
            connect_client2(get_ip, get_port);
            //  ------------------------------

            // ---- Envia mensagem de inicar o jogo com o cliente 1
            strcpy(msg, "3. Deseja inicializar o jogo? (s/n)\n\n");
            send(newSocket_1, msg, strlen(msg), 0);
            recv(newSocket_1, con_info, sizeof(con_info), 0);

            if (con_info[0] == 's')
            {
                bzero(msg, sizeof(msg));
                snprintf(msg, sizeof(msg), "4");
                printf("\nMensagem: %s\n", msg);
                send(newSocket_1, msg, strlen(msg), 0);
                bzero(msg, sizeof(msg));
                recv(newSocket_1, msg, sizeof(msg), 0);
                
                if (strcmp(msg, "ready") == 0)
                {
                    for (int i = 1; i <= 3; i++)
                    {
                        strcpy(msg, "Insira pergunta: ");
                        printf("\nMensagem: %s \n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nPergunta recebida: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 1: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nResposta 1: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 2: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nResposta 2: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 3: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nResposta 3: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 4: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nResposta 4: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta certa: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        printf("\nResposta certa: %s", msg_received);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));
                    }
                }
            }
            else
            {
                strcpy(msg, "\n\nJogo cancelado.\n\n");
                send(newSocket_1, msg, strlen(msg), 0);
            }

            close(newSocket_1);
            exit(0);
        } // if do filho
        else
        { // Processo pai
            close(newSocket_1);
        }
    }
}