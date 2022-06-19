// Importando as bibliotecas que serão usadas.
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

// Definindo o tamanho máximo do buffer de envio/recebimento
#define BUFFER_SIZE 4096

// Definindo a portão em que o cliente 2 ficará esperando por mensagens.
#define CLIENTE2_PORT 10100

// Função que retorna um separador (apenas para organizar o que será mostrado para o cliente 2).
void separador()
{
    printf("\n=^..^= =^..^= =^..^= =^..^= =^..^= =^..^= \n");
}

// Função para pedir que o cliente 2 aguarde enquando o cliente 1 joga.
void aguarde()
{
    printf("\n\n");
    printf("\nAguarde o player 1 :)\n");
    printf("\n** * **   * **  *   *");
    printf("\n    *   *  *  *\n");
    printf("\n  *            *\n");
    printf("\n*  *            \n");
}

// Função que retorna uma mensagem de fim de jogo.
void fim()
{
    printf("\n\n");
    printf("\nObrigado por jogar! :)\n");
    printf("\n** * **   * **  *   *");
    printf("\n    *   *  *  *\n");
    printf("\n  *            *\n");
    printf("\n*  *            \n");
}

// Função que passa instruções ao cliente 2.
// O cliente 2 não irá iniciar o jogo, ele ficará esperando por um contato iniciado pelo cliente 1.
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
    welcome();

    // Declarando, zerando e populando a struct do socket.
    struct sockaddr_in sock;
    int con, sockid;
    char buffer[BUFFER_SIZE] = {0};
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&(sock), sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(CLIENTE2_PORT);
    sock.sin_addr.s_addr = htonl(INADDR_ANY);

    // Verifica se houve sucesso ao criar o socket,
    // se não houve, o programa para.
    if (sockid < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    // Reaproveitando a mesma porta a cada execução.
    int opcao = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    // Realiza o bind com a porta que em o cliente 2 ficará esperando por mensagens.
    if (bind(sockid, (struct sockaddr *)&sock, sizeof(sock)) < 0)
    {
        printf("Erro ao fazer o bind.\n");
        close(sockid);
        exit(1);
    }

    // Poderá ter até uma conexão por vez.
    listen(sockid, 1);

    while (1)
    {
        // Declarando e zerando as variáveis de envio/recebimento de mensagens.
        char buffer_recv[BUFFER_SIZE] = {0};
        char buffer_send[BUFFER_SIZE] = {0};
        char buffer_temp[BUFFER_SIZE] = {0};
        bzero(buffer_recv, BUFFER_SIZE);
        bzero(buffer_send, BUFFER_SIZE);
        bzero(buffer_temp, BUFFER_SIZE);

        // Criando, zerando e populando a nova struct do cliente 1 que irá se conectar.
        int new_sock_cliente_1;
        struct sockaddr_in cliente1;
        bzero(&cliente1, sizeof(cliente1));
        int c1 = sizeof(struct sockaddr_in);

        // Nessa linha o cliente 2 ficará parado esperando a conexão do cliente 1
        new_sock_cliente_1 = accept(sockid, (struct sockaddr *)&cliente1, (socklen_t *)&c1);
        if (new_sock_cliente_1 < 0)
        {
            // Se a conexão falhar, o programa para.
            printf("Erro ao aceitar conexão. - socket 1\n");
            exit(1);
        }
        else
        {
            // Entra em while enquando o servidor estiver enviando mensagens.
            while (recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0) != 0)
            {

                // Se o código enviado pelo servidor for "3", o cliente responde se
                // deseja iniciar o jogo.
                if (buffer_recv[0] == '3')
                {
                    // Assim como no cliente 1, o código é apagado da mensagem
                    // antes de ser mostrado ao cliente 2.
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    printf("\n%s", buffer_temp);
                    bzero(buffer_temp, sizeof(buffer_temp));

                    // O cliente 2 responde se deseja iniciar o jogo.
                    scanf("%s", buffer_temp);

                    // A resposta é enviada para o servidor.
                    buffer_send[0] = tolower(buffer_temp[0]);
                    __fpurge(stdin);
                    send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }

                // Ao enviar o código "4", o servidor indica que iniciou o jogo.
                // Como resosta, o cliente envia a palavra "ready", indicando que está pronto para jogar.
                if (buffer_recv[0] == '4')
                {
                    bzero(buffer_send, sizeof(buffer_send));
                    send(new_sock_cliente_1, "ready", strlen("ready"), 0);

                    // Como as perguntas estão sendo enviadas em um loop de 3 iterações
                    // do lado do servidor, o cliente responderá através de um loop de 3 iterações.
                    for (int i = 0; i < 3; i++)
                    {
                        // O servidor envia o pedido da pergunta.
                        // O cliente irá inserir a pergunta que será enviada ao servidor.
                        separador();
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        // O servidor envia o pedido da resposta a,
                        // o cliente responderá.
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        // O servidor envia o pedido da resposta b,
                        // o cliente responderá.
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        // O servidor envia o pedido da resposta c,
                        // o cliente responderá.
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        // O servidor envia o pedido da resposta d,
                        // o cliente responderá.
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_send, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));

                        // O servidor envia o pedido da resposta certa,
                        // o cliente responderá.
                        recv(new_sock_cliente_1, buffer_recv, BUFFER_SIZE, 0);
                        printf("%s", buffer_recv);
                        fgets(buffer_temp, BUFFER_SIZE, stdin);
                        __fpurge(stdin);
                        buffer_send[0] = tolower(buffer_temp[0]);
                        send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);

                        // Zerando todos os buffers.
                        bzero(buffer_send, sizeof(buffer_send));
                        bzero(buffer_recv, sizeof(buffer_recv));
                        bzero(buffer_temp, sizeof(buffer_temp));
                    }
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                }

                // Se o código enviado pelo servidor for "5",
                // significa que a primeira pergunta foi enviada e está pronta para ser respondida.
                if (buffer_recv[0] == '5')
                {
                    separador();
                    strcpy(buffer_temp, buffer_recv);

                    // Apagando o código "5" do buffer antes de mostrar a mensagem para o cliente 1.
                    buffer_temp[0] = '\n';

                    separador();

                    // Mostrando a pergunta para o cliente 1.
                    printf("\n%s", buffer_temp);

                    // Zerando os buffers.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));

                    // Recebendo a resposta da pergunta (inserida pelo cliente 1).
                    scanf("%s", buffer_send);
                    __fpurge(stdin);

                    // Enviando a resposta ao servidor.
                    send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);

                    // Recebendo a mensagem de acerto/erro e pontuação do servidor.
                    recv(new_sock_cliente_1, buffer_recv, sizeof(buffer_recv), 0);

                    // Mostrando a mensagem ao usuário.
                    printf("\n%s\n", buffer_recv);

                    // Enviando o código "1" ao servidor, confirmando o recebimento da mensagem.
                    send(new_sock_cliente_1, "1", strlen("1"), 0);

                    // Zerando os buffers.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    bzero(buffer_temp, sizeof(buffer_temp));
                }

                // Se o código enviado pelo servidor for "6",
                // significa que a segunda pergunta será mostrada pa o cliente 2.
                if (buffer_recv[0] == '6')
                {
                    // O comportamento interno da verificação segue o mesmo do código "5".
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                    recv(new_sock_cliente_1, buffer_recv, sizeof(buffer_recv), 0);
                    printf("\n%s\n", buffer_recv);
                    send(new_sock_cliente_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    bzero(buffer_temp, sizeof(buffer_temp));
                }

                // Se o código enviado pelo servidor for "7",
                // significa que a terceira pergunta será mostrada pa o cliente 2.
                if (buffer_recv[0] == '7')
                {
                     // O comportamento interno da verificação segue o mesmo do código "5" e "6".
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();
                    printf("\n%s", buffer_temp);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    scanf("%s", buffer_send);
                    __fpurge(stdin);
                    send(new_sock_cliente_1, buffer_send, strlen(buffer_send), 0);
                    recv(new_sock_cliente_1, buffer_recv, sizeof(buffer_recv), 0);
                    printf("\n%s\n", buffer_recv);
                    send(new_sock_cliente_1, "1", strlen("1"), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    bzero(buffer_temp, sizeof(buffer_temp));
                    aguarde();
                }

                // Se o código enviado pelo servidor for "8",
                // significa que o jogo acabou e que o servidor irá mostrar as pontuações.
                if (buffer_recv[0] == '8')
                {
                    strcpy(buffer_temp, buffer_recv);
                    buffer_temp[0] = '\n';
                    separador();

                    // Mostrando a mensagem de pontuação.
                    printf("\n%s", buffer_temp);

                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));

                    // Enviando um código "1", confirmando o recebimento da mensagem.
                    send(new_sock_cliente_1, "1", strlen("1"), 0);

                    // Zerando os buffers.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_recv, sizeof(buffer_recv));
                    bzero(buffer_temp, sizeof(buffer_temp));
                    separador();

                    // Mostrando mensagem de fim de jogo.
                    fim();

                    // Fechando o socket.
                    close(new_sock_cliente_1);
                    exit(0);
                }
            }
        }
    }

    // Fechando o socket inicial.
    close(sockid);
}