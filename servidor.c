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

#define tamanho_buffer 2048

typedef struct
{
    char pergunta[tamanho_buffer];
    char resp_1[tamanho_buffer];
    char resp_2[tamanho_buffer];
    char resp_3[tamanho_buffer];
    char resp_4[tamanho_buffer];
    char resp_certa[tamanho_buffer];

} Pergunta;


int compare_clientes(int pontoscliente2)
{
    return pontoscliente2;
}

int connect_client2(char *ip, char *port, Pergunta pcliente1[3], int socket_cliente1)
{
    struct sockaddr_in sock;
    int con, sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    sock.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &sock.sin_addr);
    Pergunta p[3] = {0};
    char buffer[tamanho_buffer] = {0};
    char con_info[9] = {0};
    char msg[20000] = {0};
    char msg_received[200] = {0};
    char get_ip[20] = {0};
    char get_port[20] = {0};
    bzero(con_info, sizeof(con_info));
    bzero(msg, sizeof(msg));
    bzero(msg_received, sizeof(msg_received));
    bzero(buffer, sizeof(buffer));

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));
    if (con < 0)
    {
        printf("\nNão foi possível conectar.\n");
        close(sockid);
    }
    else
    {
        strcpy(msg, "3. Deseja inicializar o jogo? (s/n): ");
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
                    sprintf(msg, "\nInsira a %dª pergunta: ", i+1);
                    printf("\nMensagem: %s \n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].pergunta, msg_received);
                    printf("\nPergunta recebida: %s", p[i].pergunta);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    sprintf(msg, "\nInsira a resposta 1 (pergunta %d): ", i+1);
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_1, msg_received);
                    printf("\nResposta 1: %s", p[i].resp_1);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    sprintf(msg, "\nInsira a resposta 2 (pergunta %d): ", i+1);
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_2, msg_received);
                    printf("\nResposta 2: %s", p[i].resp_2);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                   sprintf(msg, "\nInsira a resposta 3 (pergunta %d): ", i+1);
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_3, msg_received);
                    printf("\nResposta 3: %s", p[i].resp_3);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    sprintf(msg, "\nInsira a resposta 4 (pergunta %d): ", i+1);
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_4, msg_received);
                    printf("\nResposta 4: %s", p[i].resp_4);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    sprintf(msg, "\nInsira a resposta certa (a/b/c/d): ");
                    printf("\nMensagem: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    strcpy(p[i].resp_certa, msg_received);
                    printf("\nResposta certa: %s", p[i].resp_certa);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));

                    printf("\nValor de i: %d\n", i);
                }

                int pontos_cliente2 = 0;

                sprintf(msg, "5Pergunta 1: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d):", pcliente1[0].pergunta, pcliente1[0].resp_1, pcliente1[0].resp_2, pcliente1[0].resp_3, pcliente1[0].resp_4);
                printf("\nAlternativa certa: %s\n", pcliente1[0].resp_certa);
                send(sockid, msg, strlen(msg), 0);
                bzero(msg_received, sizeof(msg_received));
                recv(sockid, msg_received, sizeof(msg_received), 0);
                printf("\nResposta do cliente 2: %s\n", msg_received);
                if (msg_received[0] == pcliente1[0].resp_certa[0])
                {
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                    pontos_cliente2 = pontos_cliente2 + 50;
                    sprintf(msg, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente2);
                    send(sockid, msg, strlen(msg), 0);
                    printf("\nValor em msg: %s\n", msg);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 2: %s\n", msg_received);
                    bzero(msg, sizeof(msg));
                    bzero(msg_received, sizeof(msg_received));
                }
                else
                {
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                    strcpy(msg, "Resposta errada!\n");
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 2: %s\n", msg_received);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                }
                bzero(msg_received, sizeof(msg_received));
                /// -----------------------
                sprintf(msg, "6Pergunta 2: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[1].pergunta, pcliente1[1].resp_1, pcliente1[1].resp_2, pcliente1[1].resp_3, pcliente1[1].resp_4);
                printf("\nAlternativa certa: %s\n", pcliente1[0].resp_certa);
                send(sockid, msg, strlen(msg), 0);
                bzero(msg_received, sizeof(msg_received));
                recv(sockid, msg_received, sizeof(msg_received), 0);
                printf("\nResposta do cliente 1: %s\n", msg_received);
                if (msg_received[0] == pcliente1[1].resp_certa[0])
                {
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                    pontos_cliente2 = pontos_cliente2 + 50;
                    sprintf(msg, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente2);
                    send(sockid, msg, strlen(msg), 0);
                    printf("\nValor em msg: %s\n", msg);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 2: %s\n", msg_received);
                    bzero(msg, sizeof(msg));
                    bzero(msg_received, sizeof(msg_received));
                }
                else
                {
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                    strcpy(msg, "Resposta errada!\n");
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 1: %s\n", msg_received);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                }
                bzero(msg_received, sizeof(msg_received));
                /// -----------------------
                sprintf(msg, "7Pergunta 3: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[2].pergunta, pcliente1[2].resp_1, pcliente1[2].resp_2, pcliente1[2].resp_3, pcliente1[2].resp_4);
                printf("\nAlternativa certa: %s\n", pcliente1[0].resp_certa);
                send(sockid, msg, strlen(msg), 0);
                bzero(msg_received, sizeof(msg_received));
                recv(sockid, msg_received, sizeof(msg_received), 0);
                printf("\nResposta do cliente 1: %s\n", msg_received);
                if (msg_received[0] == pcliente1[1].resp_certa[0])
                {
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                    pontos_cliente2 = pontos_cliente2 + 50;
                    sprintf(msg, "Resposta correta!\nVocê tem %d pontos!\n", pontos_cliente2);
                    printf("\nValor em msg: %s\n", msg);
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 2: %s\n", msg_received);
                    bzero(msg, sizeof(msg));
                    bzero(msg_received, sizeof(msg_received));
                }
                else
                {
                    bzero(msg, sizeof(msg));
                    strcpy(msg, "Resposta errada!\n");
                    send(sockid, msg, strlen(msg), 0);
                    recv(sockid, msg_received, sizeof(msg_received), 0);
                    printf("\nResposta do cliente 2: %s\n", msg_received);
                    bzero(msg_received, sizeof(msg_received));
                    bzero(msg, sizeof(msg));
                }

            }

            close(sockid);
        }

        return sockid;
    }
}

int main(void)
{
    Pergunta p[3] = {0};
    int socket_con;
    struct sockaddr_in servidor;
    char buffer_send[tamanho_buffer] = {0};
    char buffer_receive[tamanho_buffer] = {0};
    char ip_cliente2[20];
    char port_cliente2[20];

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
    servidor.sin_port = htons(5004);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(socket_con, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
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
            printf("\nMensagem: %s\n", buffer_send);
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
            printf("\nEndereço IP recebido: %s\n", buffer_receive);
            strcpy(ip_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            //  ------------------------------

            // ---- Recebe a porta do cliente 2 e envia de volta ao cliente 1
            strcpy(buffer_send, "2");
            printf("\nMensagem: %s\n", buffer_send);
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
            printf("\nEndereço de porta recebido: %s\n", buffer_receive);
            strcpy(port_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            //  ------------------------------

            // ---- Envia mensagem de inicar o jogo com o cliente 1
            strcpy(buffer_send, "3. Deseja inicializar o jogo? (s/n): ");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

            if (buffer_receive[0] == 's')
            {
                bzero(buffer_send, sizeof(buffer_send));
                strcpy(buffer_send, "4");
                printf("\nMensagem: %s\n", buffer_send);
                send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

                if (strcmp(buffer_receive, "ready") == 0)
                {
                    bzero(buffer_receive, sizeof(buffer_receive));
                    for (int i = 0; i < 3; i++)
                    {
                        sprintf(buffer_send, "\nInsira a %dª pergunta: ", i+1);
                       // printf("\nMensagem: %s \n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].pergunta, buffer_receive);
                        printf("\nPergunta recebida: %s\n", p[i].pergunta);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta 1 (pergunta %d): ", i+1);
                      //  printf("\nMensagem: %s\n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].resp_1, buffer_receive);
                        printf("\nResposta 1: %s", p[i].resp_1);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta 2 (pergunta %d): ", i+1);
                     //   printf("\nMensagem: %s\n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].resp_2, buffer_receive);
                        printf("\nResposta 2: %s", p[i].resp_2);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta 3 (pergunta %d): ", i+1);
                     //   printf("\nMensagem: %s\n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].resp_3, buffer_receive);
                        printf("\nResposta 3: %s", p[i].resp_3);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta 4 (pergunta %d): ", i+1);
                        printf("\nMensagem: %s\n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].resp_4, buffer_receive);
                        printf("\nResposta 4: %s", p[i].resp_4);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        sprintf(buffer_send, "Insira a resposta certa (a/b/c/d): ");
                     //   printf("\nMensagem: %s\n", buffer_send);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(p[i].resp_certa, buffer_receive);
                        printf("\nResposta certa: %s", p[i].resp_certa);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
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
                int a = connect_client2(ip_cliente2, port_cliente2, p, socket_cliente1); // <---- parei aqui
                printf("\nConexão chamada %d", a);
                //  ------------------------------
            }
            else
            {
                strcpy(buffer_send, "\n\nJogo cancelado.\n\n");
                send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            }
           // close(socket_cliente1);
           // exit(0);
        }
        else
        { 
            close(socket_cliente1);
        }
    }
}