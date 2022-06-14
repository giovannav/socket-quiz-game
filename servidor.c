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

typedef struct
{
    char pergunta[QUESTION_SIZE];
    char resp_1[QUESTION_SIZE];
    char resp_2[QUESTION_SIZE];
    char resp_3[QUESTION_SIZE];
    char resp_4[QUESTION_SIZE];
    char resp_certa[QUESTION_SIZE];

} Pergunta;

int connect_client2(char *ip, char *port, Pergunta pcliente1[3])
{
    Pergunta p[3] = {0};

    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE] = {0};
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &sock.sin_addr);
    bzero(buffer, BUFFER_SIZE);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));
    if (con < 0)
    {
        printf("\nNão foi possível conectar.\n");
        close(sockid);
    }
    else
    {
        char con_info[9] = {0};
        bzero(con_info, sizeof(con_info));
        char msg[200] = {0};
        bzero(msg, sizeof(msg));
        char msg_received[200] = {0};
        bzero(msg_received, sizeof(msg_received));
        char get_ip[20] = {0}, get_port[20] = {0};
        // ---- Envia mensagem de inicar o jogo com o cliente 1
        strcpy(msg, "3. Deseja inicializar o jogo? (s/n)\n\n");
        send(sockid, msg, strlen(msg), 0);
        recv(sockid, con_info, sizeof(con_info), 0);

        if (con_info[0] == 's')
        {
            bzero(msg, sizeof(msg));
            snprintf(msg, sizeof(msg), "4");
            printf("\nMensagem: %s\n", msg);
            send(sockid, msg, strlen(msg), 0);
            bzero(msg, sizeof(msg));
            recv(sockid, msg, sizeof(msg), 0);

            if (strcmp(msg, "ready") == 0)
            {
                bzero(msg, sizeof(msg));
                for (int i = 0; i < 3; i++)
                {
                    strcpy(msg, "Insira pergunta: ");
                    printf("\nMensagem: %s \n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].pergunta, msg_received);
                    printf("\nPergunta recebida: %s", p[i].pergunta);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    strcpy(msg, "Insira a resposta 1: ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_1, msg_received);
                    printf("\nResposta 1: %s", p[i].resp_1);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    strcpy(msg, "Insira a resposta 2: ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_2, msg_received);
                    printf("\nResposta 2: %s", p[i].resp_2);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    strcpy(msg, "Insira a resposta 3: ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_3, msg_received);
                    printf("\nResposta 3: %s", p[i].resp_3);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    strcpy(msg, "Insira a resposta 4: ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_4, msg_received);
                    printf("\nResposta 4: %s", p[i].resp_4);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    strcpy(msg, "Insira a resposta certa: ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_certa, msg_received);
                    printf("\nResposta certa: %s", p[i].resp_certa);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    printf("\nValor de i: %d\n", i);
                }
            }
            // Enviando uma questão por vez
            //Concatenando em uma única string
            // ---------------> parei aqui            
                printf("\nPergunta: %s", pcliente1[0].pergunta);
                printf("\nResposta 1: %s", pcliente1[0].resp_1);
                printf("\nResposta 2: %s", pcliente1[0].resp_2);
                printf("\nResposta 3: %s", pcliente1[0].resp_3);
                printf("\nResposta 4: %s", pcliente1[0].resp_4);
                printf("\nResposta certa: %s", pcliente1[0].resp_certa);
            

            close(sockid);
        }

        return sockid;
    }
}

int main(void)
{
    Pergunta p[3] = {0};
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
    servidor.sin_port = htons(5004);
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
            char con_info[9] = {0};
            bzero(con_info, sizeof(con_info));
            char msg[200] = {0};
            bzero(msg, sizeof(msg));
            char msg_received[200] = {0};
            bzero(msg_received, sizeof(msg_received));
            char get_ip[20] = {0}, get_port[20] = {0};

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
                    bzero(msg, sizeof(msg));
                    for (int i = 0; i < 3; i++)
                    {
                        strcpy(msg, "Insira pergunta: ");
                        printf("\nMensagem: %s \n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].pergunta, msg_received);
                        printf("\nPergunta recebida: %s", p[i].pergunta);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 1: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].resp_1, msg_received);
                        printf("\nResposta 1: %s", p[i].resp_1);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 2: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].resp_2, msg_received);
                        printf("\nResposta 2: %s", p[i].resp_2);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 3: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].resp_3, msg_received);
                        printf("\nResposta 3: %s", p[i].resp_3);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta 4: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].resp_4, msg_received);
                        printf("\nResposta 4: %s", p[i].resp_4);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        strcpy(msg, "Insira a resposta certa: ");
                        printf("\nMensagem: %s\n", msg);
                        send(newSocket_1, msg, strlen(msg), 0);
                        recv(newSocket_1, msg_received, sizeof(msg_received), 0);
                        strcpy(p[i].resp_certa, msg_received);
                        printf("\nResposta certa: %s", p[i].resp_certa);
                        bzero(msg_received, sizeof(msg_received));
                        bzero(msg, sizeof(msg));

                        printf("\nValor de i: %d\n", i);
                    }
                }
                printf("\nFim do for.\n");
                for (int j = 0; j < 3; j++)
                {
                    printf("\nPergunta: %s", p[j].pergunta);
                    printf("\nResposta 1: %s", p[j].resp_1);
                    printf("\nResposta 2: %s", p[j].resp_2);
                    printf("\nResposta 3: %s", p[j].resp_3);
                    printf("\nResposta 4: %s", p[j].resp_4);
                    printf("\nResposta certa: %s", p[j].resp_certa);
                }

                // ---- Chama a conexão para a porta 2
                printf("\nChamando conexão");
                connect_client2(get_ip, get_port, p);
                //  ------------------------------
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