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
#include <ctype.h>

#define BUFFER_SIZE 4096

void separador()
{
    printf("\n=^..^= =^..^= =^..^= =^..^= =^..^= =^..^= \n");
}

void aguarde()
{
    printf("\n\n");
    printf("\nAguarde o player 1 :)\n");
    printf("\n** * **   * **  *   *");
    printf("\n    *   *  *  *\n");
    printf("\n  *            *\n");
    printf("\n*  *            \n");
}

void fim()
{
    printf("\n\n");
    printf("\nObrigado por jogar! :)\n");
    printf("\n** * **   * **  *   *");
    printf("\n    *   *  *  *\n");
    printf("\n  *            *\n");
    printf("\n*  *            \n");
}

void welcome()
{
    separador();
    printf("\n ____  ____  __  __    _  _  ____  _  _  ____  _____");
    printf("\n(  _ \\( ___)(  \\/  )  ( \\/ )(_  _)( \\( )(  _ \\(  _  )");
    printf("\n ) _ < )__)  )    (    \\  /  _)(_  )  (  )(_) ))(_)(");
    printf("\n(____/(____)(_/\\/\\_)    \\/  (____)(_)\\_)(____/(_____)");
    printf("\n\n");
    separador();
    printf("\nVocê é o player 2! \n");
    printf("\nO jogo conta com 3 perguntas com 4 alternativas\n");
    printf("\nCuidado: o tamanho máximo de uma pergunta/questão é de 4096 bytes. :)\n");
    printf("\nPressione enter para passar para a próxima pergunta/questão.\n");
    printf("\n--> AGUARDE QUANDO O JOGO PEDIR <---\n");
    printf("\n Cada questão vale 50 pontos, quem fizer mais pontos ao final, ganha o jogo!\n");
    printf("\n BOA SORTE! <3.\n");
    printf("\n");
    printf("\nAguarde o player 1 iniciar o quiz.\n");
    separador();
    printf("\n");
}

int main(void)
{
    // nc -l -p 5000

    welcome();

    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE] = {0};
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    sock.sin_family = AF_INET;
    // Sempre esperando na porta 10100
    sock.sin_port = htons(10100);
    sock.sin_addr.s_addr = htonl(INADDR_ANY);
    char buffer_recv[BUFFER_SIZE] = {0};
    char buffer_temp[BUFFER_SIZE] = {0};

    if (sockid < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(0);
    }

    int opcao = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    if (bind(sockid, (struct sockaddr *)&sock, sizeof(sock)) < 0)
    {
        printf("Erro ao fazer o bind.\n");
        close(sockid);
        exit(0);
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
            exit(0);
        }
        else
        {
            while (recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0) != 0)
            {
                if (buffer_recv[0] == '3')
                {
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    printf("\n%s", buffer_temp);
                    bzero(buffer_temp, sizeof(buffer_temp));
                    scanf("%s", buffer_temp);
                    buffer_send[0] = tolower(buffer_temp[0]);
                    __fpurge(stdin);
                    send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if (buffer_recv[0] == '4')
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    send(newSocket_1, "ready", strlen("ready"), 0);

                    for (int i = 0; i < 3; i++)
                    {
                        separador();
                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        recv(newSocket_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_temp, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        buffer_send[0] = tolower(buffer_temp[0]);
                        send(newSocket_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));
                        bzero(buffer_temp, sizeof(buffer_temp));
                    }
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }
                if (buffer_recv[0] == '5')
                {
                    separador();
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
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
                    bzero(buffer_temp, sizeof(buffer_temp));
                }
                if (buffer_recv[0] == '6')
                {
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
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
                    bzero(buffer_temp, sizeof(buffer_temp));
                }
                if (buffer_recv[0] == '7')
                {
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
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
                    bzero(buffer_temp, sizeof(buffer_temp));
                    aguarde();
                }
                if (buffer_recv[0] == '8')
                {
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    send(newSocket_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    bzero(buffer_temp, sizeof(buffer_temp));
                    separador();
                    fim();
                    close(sockid);
                    exit(0);
                }
            }
        }
    }
    close(sockid);
}