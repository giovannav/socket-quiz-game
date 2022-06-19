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
#define MAX_CLIENTS 10
#define SERVER_PORT 5100
#define CLIENTE2_PORT "10100"

struct cliente2
{
    char cliente2_ip[20], cliente2_port[20];
};

typedef struct cliente2 Cliente2;

void separador()
{
    printf("\n =^..^= =^..^= =^..^= =^..^= =^..^= =^..^= \n");
}

void aguarde()
{
    printf("\n\n");
    printf("\nAguarde o player 2 :)\n");
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

Cliente2 welcome()
{

    Cliente2 c2 = {0};
    char get_ip[20] = {0};
    char get_port[20] = {0};
    bzero(get_ip, 20);
    bzero(get_port, 20);

    separador();
    printf("\n ____  ____  __  __    _  _  ____  _  _  ____  _____");
    printf("\n(  _ \\( ___)(  \\/  )  ( \\/ )(_  _)( \\( )(  _ \\(  _  )");
    printf("\n ) _ < )__)  )    (    \\  /  _)(_  )  (  )(_) ))(_)(");
    printf("\n(____/(____)(_/\\/\\_)    \\/  (____)(_)\\_)(____/(_____)");

    printf("\n\n");
    separador();
    printf("\nVocê é o player 1! \n");
    printf("\nVocê deve iniciar quiz!\n");
    printf("\nVocê deve escolher o player 2 digitando o endereço IP para realizar a conexão!\n");
    printf("\nO jogo conta com 3 perguntas com 4 alternativas\n");
    printf("\nCuidado: o tamanho máximo de uma pergunta/questão é de 4096 bytes. :)\n");
    printf("\nPressione enter para passar para a próxima pergunta/questão.\n");
    printf("\n--> AGUARDE QUANDO O JOGO PEDIR <---\n");
    printf("\n--> CERTIFIQUE-SE QUE O PLAYER 2 ESTÁ ONLINE! <---\n");
    printf("\n Cada questão vale 50 pontos, quem fizer mais pontos ao final, ganha o jogo!\n");
    printf("\nBOA SORTE! <3.\n");
    printf("\nPressione enter para continuar.\n");
    separador();
    printf("\n");
    getchar();

    printf("\n");
    printf("\nDigite o IP com que deseja se conectar: ");
    scanf("%s", get_ip);
    __fpurge(stdin);
    strcpy(c2.cliente2_ip, get_ip);

    // A porta do cliente 2 sempre será a 10100
    strcpy(c2.cliente2_port, CLIENTE2_PORT);
    return c2;
}

int main(void)
{

    struct sockaddr_in sock;
    int con, sockid;
    char ip_cliente2[20] = {0};
    char port_cliente2[20] = {0};
    char buffer_recv[BUFFER_SIZE] = {0};
    char buffer_send[BUFFER_SIZE] = {0};
    char buffer_temp[BUFFER_SIZE] = {0};
    struct in_addr addr;
    sockid = socket(AF_INET, SOCK_STREAM, 0); // criando o socket
    bzero(&(sock), sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);
    Cliente2 c2_main = welcome();

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0)
    {
        printf("\nErro ao conectar com servidor. Tente novamente.\n");
        close(sockid);
        exit(0);
    }
    bzero(buffer_recv, sizeof(buffer_recv));

    while (recv(sockid, buffer_recv, BUFFER_SIZE, 0) != 0)
    {
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
            strcpy(buffer_temp, buffer_recv);
            buffer_temp[0] = '\n';
            separador();
            printf("\n%s", buffer_temp);
            bzero(buffer_temp, sizeof(buffer_temp));
            scanf("%s", buffer_temp);
            buffer_send[0] = tolower(buffer_temp[0]);
            __fpurge(stdin);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        if (buffer_recv[0] == '4')
        {
            bzero(buffer_send, sizeof(buffer_send));
            send(sockid, "ready", strlen("ready"), 0);
            for (int i = 0; i < 3; i++)
            {
                separador();
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_temp, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                buffer_send[0] = tolower(buffer_temp[0]);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));
                bzero(buffer_temp, sizeof(buffer_temp));  
            }
            aguarde();
        }

        if (buffer_recv[0] == '5')
        {
            if(strlen(buffer_recv) <= 73){
                close(sockid);
                break;
                exit(0);
            }
            separador();
            strcpy(buffer_temp, buffer_recv);
            buffer_temp[0] = '\n';
            separador();
            printf("\n%s", buffer_temp);
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
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
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
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
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
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            bzero(buffer_temp, sizeof(buffer_temp));
            separador();
        }
        bzero(buffer_recv, sizeof(buffer_recv));
        bzero(buffer_send, sizeof(buffer_send));
    }
    fim();
    close(sockid);
}