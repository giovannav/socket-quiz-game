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

#define tamanho_buffer 4096
#define tamanho_mensagem 25000
#define SERVER_PORT 5100
#define CLIENTE2_PORT "10100"

struct pergunta
{
    char pergunta[tamanho_buffer];
    char resp_1[tamanho_buffer];
    char resp_2[tamanho_buffer];
    char resp_3[tamanho_buffer];
    char resp_4[tamanho_buffer];
    char resp_certa[tamanho_buffer];
    int pontos;
};

void connect_client2(char *ip, char *port, struct pergunta *pcliente2, struct pergunta pcliente1[3], int *pontos_cliente1, int *pontos_cliente2, int *is_game_end)
{
    struct sockaddr_in sock;
    int con, sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &sock.sin_addr);
    char buffer_send[tamanho_mensagem] = {0};
    char buffer_receive[tamanho_mensagem] = {0};
    char get_ip[20] = {0};
    char get_port[20] = {0};
    int contagem_pontos_cliente2 = 0;
    bzero(buffer_send, sizeof(buffer_send));
    bzero(buffer_receive, sizeof(buffer_receive));

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));
    if (con < 0)
    {
        printf("\nNão foi possível conectar.\n");
        close(sockid);
    }
    if (*is_game_end != 0)
    {
        bzero(buffer_send, sizeof(buffer_send));
        bzero(buffer_receive, sizeof(buffer_receive));
        if (*pontos_cliente2 > *pontos_cliente1)
        {
            snprintf(buffer_send, sizeof(buffer_send), "8Você venceu! :D O player 1 fez %d pontos\n", *pontos_cliente1);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            close(sockid);
        }
        else if (*pontos_cliente2 < *pontos_cliente1)
        {
            snprintf(buffer_send, sizeof(buffer_send), "8Você perdeu! :( O player 1 fez %d pontos\n", *pontos_cliente1);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            close(sockid);
        }
        else
        {
            snprintf(buffer_send, sizeof(buffer_send), "8Empate! :D O player 1 também fez %d pontos\n", *pontos_cliente1);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            close(sockid);
        }
    }
    else
    {
        strcpy(buffer_send, "3Deseja inicializar o jogo? (s/n): ");
        send(sockid, buffer_send, strlen(buffer_send), 0);
        recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
        if (buffer_receive[0] == 's')
        {
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_receive, sizeof(buffer_receive));
            strcpy(buffer_send, "4");
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

            if (strcmp(buffer_receive, "ready") == 0)
            {
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));
                for (int i = 0; i < 3; i++)
                {
                    sprintf(buffer_send, "\nInsira a %dª pergunta: ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].pergunta,buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    sprintf(buffer_send, "Insira a resposta a (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_1, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    sprintf(buffer_send, "Insira a resposta b (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_2, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    sprintf(buffer_send, "Insira a resposta c (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_3, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    sprintf(buffer_send, "Insira a resposta d (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_4, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    sprintf(buffer_send, "Insira a resposta certa (a/b/c/d): ");
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_certa, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                } 

                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));
                sprintf(buffer_send, "5Pergunta 1: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[0].pergunta, pcliente1[0].resp_1, pcliente1[0].resp_2, pcliente1[0].resp_3, pcliente1[0].resp_4);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                if (buffer_receive[0] == pcliente1[0].resp_certa[0])
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    contagem_pontos_cliente2 = contagem_pontos_cliente2 + 50;
                    pcliente2[0].pontos = contagem_pontos_cliente2;
                    sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                else
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    sprintf(buffer_send, "Resposta errada!\nA resposta certa é: %s \nVocê tem %d pontos!", pcliente1[0].resp_certa, contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));
                sprintf(buffer_send, "6Pergunta 2: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[1].pergunta, pcliente1[1].resp_1, pcliente1[1].resp_2, pcliente1[1].resp_3, pcliente1[1].resp_4);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

                if (buffer_receive[0] == pcliente1[1].resp_certa[0])
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    contagem_pontos_cliente2 = contagem_pontos_cliente2 + 50;
                    pcliente2[0].pontos = contagem_pontos_cliente2;
                    sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                else
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    sprintf(buffer_send, "Resposta errada!\nA resposta certa é %s \nVocê tem %d pontos!", pcliente1[1].resp_certa, contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));
                /// -----------------------
                sprintf(buffer_send, "7Pergunta 3: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[2].pergunta, pcliente1[2].resp_1, pcliente1[2].resp_2, pcliente1[2].resp_3, pcliente1[2].resp_4);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

                if (buffer_receive[0] == pcliente1[2].resp_certa[0])
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    contagem_pontos_cliente2 = contagem_pontos_cliente2 + 50;
                    pcliente2[0].pontos = contagem_pontos_cliente2;
                    sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                else
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    sprintf(buffer_send, "Resposta errada!\nA resposta certa é %s \nVocê tem %d pontos!", pcliente1[2].resp_certa, contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }
                close(sockid);
            }
        }
    }
}

int main(void)
{
    struct pergunta pcliente1[3] = {0};
    struct pergunta pcliente2[3] = {0};
    int socket_con;
    struct sockaddr_in servidor;
    char buffer_send[tamanho_mensagem] = {0};
    char buffer_receive[tamanho_mensagem] = {0};
    char ip_cliente2[20];
    char port_cliente2[20];
    int pontos_cliente1 = 0;
    int is_game_end = 0;

    socket_con = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_con < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    int opcao = 1;
    setsockopt(socket_con, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    bzero(&servidor, sizeof(servidor));
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(SERVER_PORT);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(socket_con, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        printf("Erro ao fazer o bind.\n");
        close(socket_con);
        exit(1);
    }

    listen(socket_con, 5);

    while (1)
    {
        int socket_cliente1;
        struct sockaddr_in cliente1;
        bzero(&cliente1, sizeof(cliente1));
        int c1 = sizeof(struct sockaddr_in);
        socket_cliente1 = accept(socket_con, (struct sockaddr *)&cliente1, (socklen_t *)&c1);

        int pid = fork();

        if (socket_cliente1 < 0)
        {
            printf("Erro ao aceitar conexão. - socket 1\n");
        }
        else
        {
            printf("Conexão estabelecida. - socket 1\n");
        }

        if (pid < 0)
        {
            printf("\nErro ao criar processo filho\n");
        }
        if (pid == 0)
        {
            // ------------------------ Mensagem de boas vindas ao cliente 1
            close(socket_con);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_receive, sizeof(buffer_receive));

            // ---- Recebe o IP do cliente 2 e envia de volta ao cliente 1
            strcpy(buffer_send, "1");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
            strcpy(ip_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            //  ------------------------------

            // ---- Recebe a porta do cliente 2 e envia de volta ao cliente 1
            strcpy(buffer_send, "2");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
            strcpy(port_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            //  ------------------------------

            // ---- Envia mensagem de inicar o jogo com o cliente 1
            strcpy(buffer_send, "3Deseja inicializar o jogo? (s/n): ");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

            if (buffer_receive[0] == 's')
            {
                bzero(buffer_send, sizeof(buffer_send));
                strcpy(buffer_send, "4");
                send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

                if (strcmp(buffer_receive, "ready") == 0)
                {
                    bzero(buffer_receive, sizeof(buffer_receive));
                    for (int i = 0; i < 3; i++)
                    {
                        sprintf(buffer_send, "\nInsira a %dª pergunta: ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].pergunta, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta a (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_1, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta b (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_2, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta c (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_3, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta d (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_4, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta certa (a/b/c/d): ");
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_certa, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    connect_client2(ip_cliente2, port_cliente2, pcliente2, pcliente1, &pcliente1->pontos, &pcliente2->pontos, &is_game_end);
                    sprintf(buffer_send, "5Pergunta 1: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente2[0].pergunta, pcliente2[0].resp_1, pcliente2[0].resp_2, pcliente2[0].resp_3, pcliente2[0].resp_4);
                    send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                    recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                    if (buffer_receive[0] == pcliente2[0].resp_certa[0])
                    {
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        pontos_cliente1 = pontos_cliente1 + 50;
                        pcliente1[0].pontos = pontos_cliente1;
                        sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_receive, sizeof(buffer_receive));
                    }
                    else
                    {
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        sprintf(buffer_send, "Resposta errada!\nA resposta certa é: %s \nVocê tem %d pontos!", pcliente2[0].resp_certa, pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    bzero(buffer_receive, sizeof(buffer_receive));
                    bzero(buffer_send, sizeof(buffer_send));
                    /// -----------------------
                    sprintf(buffer_send, "6Pergunta 2: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente2[1].pergunta, pcliente2[1].resp_1, pcliente2[1].resp_2, pcliente2[1].resp_3, pcliente2[1].resp_4);
                    send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                    bzero(buffer_receive, sizeof(buffer_receive));
                    recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                    if (buffer_receive[0] == pcliente2[1].resp_certa[0])
                    {
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        pontos_cliente1 = pontos_cliente1 + 50;
                        pcliente1[0].pontos = pontos_cliente1;
                        sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_receive, sizeof(buffer_receive));
                    }
                    else
                    {
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        sprintf(buffer_send, "Resposta errada!\nA resposta certa é %s \nVocê tem %d pontos!", pcliente2[1].resp_certa, pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    bzero(buffer_receive, sizeof(buffer_receive));
                    bzero(buffer_send, sizeof(buffer_send));
                    sprintf(buffer_send, "7Pergunta 3: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente2[2].pergunta, pcliente2[2].resp_1, pcliente2[2].resp_2, pcliente2[2].resp_3, pcliente2[2].resp_4);
                    send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                    bzero(buffer_receive, sizeof(buffer_receive));
                    recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                    if (buffer_receive[0] == pcliente2[2].resp_certa[0])
                    {
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        pontos_cliente1 = pontos_cliente1 + 50;
                        pcliente1[0].pontos = pontos_cliente1;
                        sprintf(buffer_send, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_receive, sizeof(buffer_receive));
                    }
                    else
                    {
                        bzero(buffer_send, sizeof(buffer_send));
                        sprintf(buffer_send, "Resposta errada!\nA resposta certa é %s \nVocê tem %d pontos!", pcliente2[2].resp_certa, pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }

                    bzero(buffer_receive, sizeof(buffer_receive));
                    bzero(buffer_send, sizeof(buffer_send));

                    if (pcliente1[0].pontos > pcliente2[0].pontos)
                    {
                        snprintf(buffer_send, sizeof(buffer_send), "8Você venceu! :D O player 2 fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    else if (pcliente1[0].pontos < pcliente2[0].pontos)
                    {
                        snprintf(buffer_send, sizeof(buffer_send), "8Você perdeu! :( O player 2 fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    else
                    {
                        snprintf(buffer_send, sizeof(buffer_send), "8Empate! :D O player 2 também fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }

                    is_game_end = 1;
                    connect_client2(ip_cliente2, port_cliente2, pcliente2, pcliente1, &pcliente1->pontos, &pcliente2->pontos, &is_game_end);
                }
            }
            else
            {
                strcpy(buffer_send, "\n\nJogo cancelado.\n\n");
                send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            }
            close(socket_cliente1);
            exit(0);
        }
        else
        {
            close(socket_cliente1);
        }
    }
}