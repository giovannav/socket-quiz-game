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

// Definindo o tamnho do buffer de recebimento/envio.
#define BUFFER_SIZE 4096

// Definindo a porta do servidor (5100).
#define SERVER_PORT 5100

// Definindo a porta padrão em que o cliente 2 receberá conexões.
#define CLIENTE2_PORT "10100"

// Criando uma struct para guardar as informações de conexão do cliente 2.
struct cliente2
{
    char cliente2_ip[20], cliente2_port[20];
};
typedef struct cliente2 Cliente2;


// Função que retorna um separador (apenas para organizar o que será mostrado para o cliente 1).
void separador()
{
    printf("\n =^..^= =^..^= =^..^= =^..^= =^..^= =^..^= \n");
}

// Função para pedir que o cliente 1 aguarde enquando o cliente 2 joga.
void aguarde()
{
    printf("\n\n");
    printf("\nAguarde o player 2 :)\n");
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


// Função que passa instruções ao cliente 1, assim como coleta informações de conexão
// com o cliente 2.
Cliente2 welcome()
{

    // Declarando a struct que será retornada.
    Cliente2 c2 = {0};

    // Declarando e inicializando a variável que irá guardar o IP.
    char get_ip[20] = {0};
    bzero(get_ip, 20);

    // Mensage de boas vindas.
    separador();
    printf("\n ____  ____  __  __    _  _  ____  _  _  ____  _____");
    printf("\n(  _ \\( ___)(  \\/  )  ( \\/ )(_  _)( \\( )(  _ \\(  _  )");
    printf("\n ) _ < )__)  )    (    \\  /  _)(_  )  (  )(_) ))(_)(");
    printf("\n(____/(____)(_/\\/\\_)    \\/  (____)(_)\\_)(____/(_____)");

    // Instruções para o cliente 1.
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

    // Espera o entrer do cliente 1 para continuar.
    getchar();

    // Pede e recebe o endereço IP do cliente 2.
    printf("\n");
    printf("\nDigite o IP com que deseja se conectar (ex. 127.0.0.1): ");
    scanf("%s", get_ip);
    __fpurge(stdin);
    // Copia para a struct do cliente 2.
    strcpy(c2.cliente2_ip, get_ip);

    // A porta do cliente 2 sempre será a 10100.
    // Copia a porta para o cliente 2.
    strcpy(c2.cliente2_port, CLIENTE2_PORT);
    
    // Retrona a struct com os valores preenchidos.
    return c2;
}

int main(void)
{

    // Declarando, zerando e populando a struct do socket.
    struct sockaddr_in sock;
    int con, sockid;
    struct in_addr addr;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&(sock), sizeof(sock));
    sock.sin_family = AF_INET;

    // Passando a porta padrão do servidor.
    sock.sin_port = htons(SERVER_PORT);

    // O servidor estará rodando localmente, por isso o endereço IP é o localhost.
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);

    // Chamando a função que pede as informações do cliente 2.
    Cliente2 c2_main = welcome();

    // Declarando e zerando as variáveis que serão usadas para enviar/receber dados.
    char buffer_recv[BUFFER_SIZE] = {0};
    char buffer_send[BUFFER_SIZE] = {0};
    char buffer_temp[BUFFER_SIZE] = {0};
    bzero(buffer_recv, sizeof(buffer_recv));
    bzero(buffer_send, sizeof(buffer_send));
    bzero(buffer_temp, sizeof(buffer_temp));

    // Abrindo a conexão com o servidor.
    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0)
    {
        // Se a conexão falhar, mostra mensagem de erro e fechar o socket.
        printf("\nErro ao conectar com servidor. Tente novamente.\n");
        close(sockid);
        exit(1);
    }

    // Entra em loop caso o servidor esteja enviando mensagem ao cliente.
    while (recv(sockid, buffer_recv, BUFFER_SIZE, 0) != 0)
    {
        // Se o código enviado pelo servidor for "1", o cliente envia o
        // endereço IP do cliente 2 com quem deseja se conectar.
        if (strcmp(buffer_recv, "1") == 0)
        {
            strcpy(buffer_send, c2_main.cliente2_ip);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }

        // Se o código enviado pelo servidor for "2", o cliente envia a porta
        // do cliente 2 com quem deseja se conectar.
        if (strcmp(buffer_recv, "2") == 0)
        {
            strcpy(buffer_send, c2_main.cliente2_port);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
        }

        // Se o código enviado pelo servidor for "3", o cliente responde se
        // deseja iniciar o jogo.
        if (buffer_recv[0] == '3')
        {
            // Antes de enviar, o caracter é passado para minúsculo.
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

        // Ao enviar o código "4", o servidor indica que iniciou o jogo.
        // Como resosta, o cliente envia a palavra "ready", indicando que está pronto para jogar.
        if (buffer_recv[0] == '4')
        {   
            bzero(buffer_send, sizeof(buffer_send));
            send(sockid, "ready", strlen("ready"), 0);
            // Como as perguntas estão sendo enviadas em um loop de 3 iterações
            // do lado do servidor, o cliente responderá através de um loop de 3 iterações.
            for (int i = 0; i < 3; i++)
            {
                // O servidor envia o pedido da pergunta.
                // O cliente irá inserir a pergunta que será enviada ao servidor.
                separador();
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                // O servidor envia o pedido da resposta a,
                // o cliente responderá.
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                // O servidor envia o pedido da resposta b,
                // o cliente responderá.
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                // O servidor envia o pedido da resposta c,
                // o cliente responderá.
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                // O servidor envia o pedido da resposta d,
                // o cliente responderá.
                recv(sockid, buffer_recv, BUFFER_SIZE, 0);
                printf("%s", buffer_recv);
                fgets(buffer_send, BUFFER_SIZE, stdin);
                __fpurge(stdin);
                send(sockid, buffer_send, strlen(buffer_send), 0);
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_recv, sizeof(buffer_recv));

                // O servidor envia o pedido da resposta certa,
                // o cliente responderá.
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

            // Nesta parte do código, do lado do servidor, o servidor irá iniciar o contato
            // com o cliente 2. Por isso, o cliente 1 fica esperando a interação do cliente 2.
            aguarde();
        }

        // Se o código enviado pelo servidor for "5",
        // significa que a primeira pergunta foi enviada e está pronta para ser respondida.
        if (buffer_recv[0] == '5')
        {
            // Checando se não houve um erro e a pergunta não está vazia.
            if(strlen(buffer_recv) <= 73){
                close(sockid);
                break;
                exit(0);
            }
            separador();
            strcpy(buffer_temp, buffer_recv);

            // Apagando o código "5" do buffer antes de mostrar a mensagem para o cliente 1.
            buffer_temp[0] = '\n';

            separador();

            // Mostrando a pergunta para o cliente 1.
            printf("\n%s", buffer_temp);

            // Zerando as variáveis.
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            
            // Recebendo a resposta da pergunta (inserida pelo cliente 1).
            scanf("%s", buffer_send);
            __fpurge(stdin);

            // Enviando a resposta ao servidor.
            send(sockid, buffer_send, strlen(buffer_send), 0);

            // Recebendo a mensagem de acerto/erro e pontuação do servidor.
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);

            // Mostrando a mensagem ao usuário.
            printf("\n%s\n", buffer_recv);

            // Enviando o código "1", confirmando o recebimento da mensagem.
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            bzero(buffer_temp, sizeof(buffer_temp));
        }

        // Se o código enviado pelo servidor for "6",
        // significa que a segunda pergunta será mostrada pa o cliente 1.
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
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            bzero(buffer_temp, sizeof(buffer_temp));
        }

        // Se o código enviado pelo servidor for "7",
        // significa que a terceira pergunta será mostrada pa o cliente 1.
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
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_recv, sizeof(buffer_recv), 0);
            printf("\n%s\n", buffer_recv);
            send(sockid, "1", strlen("1"), 0);
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
            send(sockid, "1", strlen("1"), 0);
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_recv, sizeof(buffer_recv));
            bzero(buffer_temp, sizeof(buffer_temp));
            separador();
        }
        // Zerando os buffers dentro do while.
        bzero(buffer_recv, sizeof(buffer_recv));
        bzero(buffer_send, sizeof(buffer_send));
    }
    // Quando o buffer estiver vazio, significa que o jogo acabou.
    // Portanto, é mostrada uma mensagem de fim de jogo e o socket é fechado.
    fim();
    close(sockid);
}