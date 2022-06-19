// Importação das bibliotecas que serão utilizadas.
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <sys/time.h>

// Definição dos tamanhos dos buffers e strings que serão usados na aplicação.
#define tamanho_buffer 4096
#define tamanho_mensagem 25000

// Definição da porta do servidor.
#define SERVER_PORT 5100

// Struct que guardará as perguntas do quiz que serão enviadas pelos clientes.
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

// Função que irá iniciar o contato com o cliente 2 a partir do contato do cliente 1.
void connect_client2(char *ip, char *port, struct pergunta *pcliente2, struct pergunta pcliente1[3], int *pontos_cliente1, int *pontos_cliente2, int *is_game_end)
{
    // Declaração e preenchimento da struct que guarda os dados do socket.
    struct sockaddr_in sock;
    int con, sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&sock, sizeof(sock));
    sock.sin_family = AF_INET;
    sock.sin_port = htons(atoi(port));
    inet_pton(AF_INET, ip, &sock.sin_addr);

    // Declaração e inicialização do buffer que irá enviar mensagens ao cliente 2.
    char buffer_send[tamanho_mensagem] = {0};

    // Declaração e inicialização do buffer que irá receber mensagens do cliente 2.
    char buffer_receive[tamanho_mensagem] = {0};

    // Variável que irá guardar o pontos do cliente 2.
    int contagem_pontos_cliente2 = 0;

    // Zerando os buffers antes de receber/enviar qualquer mensagem;
    bzero(buffer_send, sizeof(buffer_send));
    bzero(buffer_receive, sizeof(buffer_receive));

    // Iniciando a conexão com o ciente 2. Se a conexão falhar, o socket é fechado.
    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));
    if (con < 0)
    {
        printf("\nNão foi possível conectar.\n");
        close(sockid);
    }

    // A variável is_game_end, passada por parâmetro, será igual a 1 se o cliente 1 já
    // terminou o seu jogo, assim será possível comparar as pontuações.
    if (*is_game_end != 0)
    {
        bzero(buffer_send, sizeof(buffer_send));
        bzero(buffer_receive, sizeof(buffer_receive));

        // As variáveis *pontos_cliente1 e *pontos_cliente2 são passadas para o cliente 2
        // por parâmetro. Nesta parte do código é checado quem fez mais pontos e, denpendendo
        // do resultado, mensagens diferentes são enviadas.
        // Caso o cliente 2 tenha feito mais pontos que o cliente 1, o cliente 2 ganha.
        if (*pontos_cliente2 > *pontos_cliente1)
        {
            // A função snprint serve para concatenar as strings com os valores desejados.
            snprintf(buffer_send, sizeof(buffer_send), "8Você venceu! :D O player 1 fez %d pontos\n", *pontos_cliente1);
            
            // A string é enviada para o cliente 2.
            send(sockid, buffer_send, strlen(buffer_send), 0);

            // O cliente 2 irá enviar uma mensagem para o servidor confirmando o recebimento
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

            // Ambaos buffers são zerados.
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));

            // O socket é fechado pois o jogo acabou.
            // O mesmo é repetido para as outras duas opções abaixo.
            close(sockid);
        }

        // Caso o cliente 2 tenha feito menos pontos que o cliente 1, o cliente 1 ganha.
        else if (*pontos_cliente2 < *pontos_cliente1)
        {
            snprintf(buffer_send, sizeof(buffer_send), "8Você perdeu! :( O player 1 fez %d pontos\n", *pontos_cliente1);
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));
            close(sockid);
        }

        // Se não for nenhum dos casos acima, é enviada uma mensagem de empate.
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
        // Nesta seção é realizado o envio das perguntas do servidor para o cliente 2 após a conexão com o cliente 2.
        // Todas as mensgens possuem um número da frente, abaixo, por exemplo, possui o número 3.
        // Esta abordagem foi escolhida para evitar comparações com grandes strings do lado do cliente.
        strcpy(buffer_send, "3Deseja inicializar o jogo? (s/n): ");
        send(sockid, buffer_send, strlen(buffer_send), 0);
        recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

        // Se o caracter recebido for 's', significa que o cliente 2 deseja iniciar o jogo.
        if (buffer_receive[0] == 's')
        {
            // Zerando os buffers.
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_receive, sizeof(buffer_receive));

            // O número 4 é enviado para o cliente 2, significando que o jogo foi iniciado, 
            // e o cliente 2 irá responder com a palavra "ready", significando que o cliente 2 está pronto para jogar.
            // O número 4 significa que o servidor irá começar a pedir que o cliente 2 insira as perguntas.
            strcpy(buffer_send, "4");
            send(sockid, buffer_send, strlen(buffer_send), 0);
            recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

            if (strcmp(buffer_receive, "ready") == 0)
            {
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));

                // São 3 perguntas no total, por isso a struct é populada dentro de uma iteração que vai de 0 a 2.
                for (int i = 0; i < 3; i++)
                {
                    // É enviado um comando ao cliente 2 pedindo que ele insira a pergunta e mostra qual 
                    // é a pergunta atual.
                    sprintf(buffer_send, "\nInsira a %dª pergunta: ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);

                    // Será recebida uma string com a pergunta do cliente 2.
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    
                    // O valor do buffer de recebimento é então copiado para a struct de perguntas na posição apropriada.
                    strcpy(pcliente2[i].pergunta,buffer_receive);
                    // Ambos os buffers são zerados.
                    
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // Após receber a pergunta, o servidor irá pedir pelas alternativas (respostas).
                    // O send/receive é o mesmo descrito acima.
                    // Abaixo, solicitação da resposta a.
                    sprintf(buffer_send, "Insira a resposta a (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_1, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // Abaixo, solicitação da resposta b.
                    sprintf(buffer_send, "Insira a resposta b (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_2, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // Abaixo, solicitação da resposta c.
                    sprintf(buffer_send, "Insira a resposta c (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_3, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // Abaixo, solicitação da resposta d.
                    sprintf(buffer_send, "Insira a resposta d (pergunta %d): ", i + 1);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_4, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // O servidor irá solicitar qual a alternativa da resposta certa e guardar na struct.
                    sprintf(buffer_send, "Insira a resposta certa (a/b/c/d): ");
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    strcpy(pcliente2[i].resp_certa, buffer_receive);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                } 

                // Zerando ambos os buffers novamente.
                bzero(buffer_send, sizeof(buffer_send));
                bzero(buffer_receive, sizeof(buffer_receive));

                // Após o cliente 2 inserir todas as perguntas, ele começará a responder as perguntas do cliente 1.
                // A struct de perguntas do cliente 1 é passada para o cliente 2 via parâmetro da função.
                sprintf(buffer_send, "5Pergunta 1: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente1[0].pergunta, pcliente1[0].resp_1, pcliente1[0].resp_2, pcliente1[0].resp_3, pcliente1[0].resp_4);
               
                // A primeira perguntoa é enviada com o código 5.
                send(sockid, buffer_send, strlen(buffer_send), 0);

                // O servidor recebe a resposta do cliente 2.
                recv(sockid, buffer_receive, sizeof(buffer_receive), 0);

                // O servidor compara a resposta do cliente 2 com a resposta certa.
                if (buffer_receive[0] == pcliente1[0].resp_certa[0])
                {   
                    // Se a resposta for certa, o servidor enviada uma mensagem ao cliente 2 alertando-o,
                    // junto à contagem de pontos.
                    // Após isso, o cliente 2 ainda envia uma código de confirmação ao servidor.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // É somada à variável de contagem de pontos 50 pontos,
                    // e posteriormente é copiada ao campo da struct para passar para o cliente 1.
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
                    // Se a resposta estiver errada, o cliente 2 receberá uma mensagem de alerta e sua pontuação.
                    // Após o envio da mensagem, o cliente 2 irá enviar um código de confirmação ao servidor.
                    // Neste caso, nenhum ponto é somado.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                    sprintf(buffer_send, "Resposta errada!\nA resposta certa é: %s \nVocê tem %d pontos!", pcliente1[0].resp_certa, contagem_pontos_cliente2);
                    send(sockid, buffer_send, strlen(buffer_send), 0);
                    recv(sockid, buffer_receive, sizeof(buffer_receive), 0);
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));
                }

                // O que foi descrito para a primeira pergunta, se repete exatamente da mesma forma
                // para as duas seguintes.
                // Abaixo, a segunda pergunta sendo enviada e corrigida.
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

                // Abaixo, a terceira pergunta sendo enviada e corrigida.
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

                // Após o envio e correção de perguntas, o socket é fechado.
                close(sockid);
            }
        }

        // Se a resposta for diferente de 's', quer dizer que o cliente 2 não deseja iniciar o jogo.
        else
        {
            strcpy(buffer_send, "\n\nJogo cancelado.\n\n");
            send(sockid, buffer_send, strlen(buffer_send), 0);
        }

        // O socket é fechado.
        close(sockid);
    }
}

int main(void)
{
    // Declaração da struct de socket do servidor.
    int socket_con;
    struct sockaddr_in servidor;
    socket_con = socket(AF_INET, SOCK_STREAM, 0);

    // Se houver um erro ao criar o socket, o programa para.
    if (socket_con < 0)
    {
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    // Uso da função para reutilizar a porta do servidor a cada execução.
    int opcao = 1;
    setsockopt(socket_con, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    // Zerando a struct do socket do servidor.
    bzero(&servidor, sizeof(servidor));

    // Populando a struct do socket do servidor.
    servidor.sin_family = AF_INET;

    // Definindo em qual porta o servidor irá escutar.
    servidor.sin_port = htons(SERVER_PORT);

    // E quais são os endereços IP do servidor.
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);


    // Reliza o bind do socket do servidor com endereço e porta.
    if (bind(socket_con, (struct sockaddr *)&servidor, sizeof(servidor)) < 0)
    {
        // Se ocorrer um erro, o socket é fechado e o programa para.
        printf("Erro ao fazer o bind.\n");
        close(socket_con);
        exit(1);
    }

    // O socket do servidor é configurado para ouvir até 10 conexões.
    listen(socket_con, 10);

    // Inicia o loop de escuta.
    while (1)
    {
        // Cria um socket para cada cliente que se conectar.
        int socket_cliente1;
        struct sockaddr_in cliente1;
        bzero(&cliente1, sizeof(cliente1));
        int c1 = sizeof(struct sockaddr_in);

        // A função accept permite que o servidor aceite uma conexão com o cliente.
        socket_cliente1 = accept(socket_con, (struct sockaddr *)&cliente1, (socklen_t *)&c1);

        // Cria um processo filho para cada cliente.
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
            // Se o processo filho foi criado com sucesso, então o servidor irá enviar mensagens ao cliente 1.

            // Declarando a struct de perguntas do cliente 1 e cliente 2.
            struct pergunta pcliente1[3] = {0};
            struct pergunta pcliente2[3] = {0};

            // Declarando as variáveis que irão armazenar as mensagens enviadas e recebidas pelo cliente 1.
            char buffer_send[tamanho_mensagem] = {0};
            char buffer_receive[tamanho_mensagem] = {0};

            // Declarando as variáveis que irão receber o endereço IP e porta do cliente 2.
            char ip_cliente2[20];
            char port_cliente2[20];

            // Variável de contagem de pontos do cliente 1.
            int pontos_cliente1 = 0;

            // Variável que irá indicar se o jogo acabou após o cliente 1 terminar de responder o quiz.
            int is_game_end = 0;

            // Fechando o primeiro socket criado pois um novo socket foi criado
            // para se comunicar com os clientes que iniciarem a conexão.
            close(socket_con);

            // Zerando os buffers.
            bzero(buffer_send, sizeof(buffer_send));
            bzero(buffer_receive, sizeof(buffer_receive));

            // Envia para o cliente 1 o código "1", que significa que está pronto para
            // receber o endereço IP do cliente 2 com quem o cliente 1 deseja jogar.
            strcpy(buffer_send, "1");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);

            // Recebe o endereço IP do cliente 2.
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

            // Copia o endereço para um variável para ser usado porteriormente.
            strcpy(ip_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));

            // Envia para o cliente 1 o código "2", que significa que está pronto para
            // receber a porta do cliente 2 com quem o cliente 1 deseja jogar
            strcpy(buffer_send, "2");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);

            // Recebe o endereço IP do cliente 2.
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

            // Copia o endereço para um variável para ser usado porteriormente.
            strcpy(port_cliente2, buffer_receive);
            bzero(buffer_receive, sizeof(buffer_receive));
            bzero(buffer_send, sizeof(buffer_send));

            // O mesmo que é descrito para o cliente 2 é feito para o cliente 1.
            // A perguntas são enviadas com os mesmos códigos e se espera as mesmas respostas.
            // Nestas comunicações de mensagens recebidas e enviadas,
            // se diferenciam apenas o socket do cliente 1 e o socket do cliente 2 usados.
            strcpy(buffer_send, "3Deseja inicializar o jogo? (s/n): ");
            send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

            // Na iteração abaixo, após receber a confirmação que o cliente 1 deseja iniciar o jogo,
            // é iniciada a iteração que irá popular a struct de perguntas do cliente 1 de acordo
            // com as posições apropriadas.
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
                        // O servidor envia uma mensagem pedidndo a pergunta 1.
                        sprintf(buffer_send, "\nInsira a %dª pergunta: ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);

                        // Recebe o buffer com a pergunta 1 e o copia para a struct de perguntas do cliente 1.
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].pergunta, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        // O servidor envia uma mensagem pedidndo a resposta a.
                        sprintf(buffer_send, "Insira a resposta a (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_1, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        // O servidor envia uma mensagem pedidndo a resposta b.
                        sprintf(buffer_send, "Insira a resposta b (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_2, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));


                        // O servidor envia uma mensagem pedidndo a resposta c.
                        sprintf(buffer_send, "Insira a resposta c (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_3, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        // O servidor envia uma mensagem pedidndo a resposta d.
                        sprintf(buffer_send, "Insira a resposta d (pergunta %d): ", i + 1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_4, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));

                        // O servidor envia uma mensagem pedidndo a resposta certa.
                        sprintf(buffer_send, "Insira a resposta certa (a/b/c/d): ");
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        strcpy(pcliente1[i].resp_certa, buffer_receive);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    
                    // Zerando ambos os buffers.
                    bzero(buffer_send, sizeof(buffer_send));
                    bzero(buffer_receive, sizeof(buffer_receive));

                    // Nesta linha é chamada a conexão com o cliente 2.
                    // Por parâmetros são enviados o endereço IP do cliente 2 com quem o cliente 1 deseja se conectar;
                    // a porta do cliente 2; a struct de perguntas do cliente 2 e cliente 1 e as suas respectivas pontuações.
                    // Por fim, é passada a variável que indica se o jogo já acabou ou não.
                    connect_client2(ip_cliente2, port_cliente2, pcliente2, pcliente1, &pcliente1->pontos, &pcliente2->pontos, &is_game_end);

                    // Após a função acima realizar todas as suas rotinas, as perguntas
                    // do cliente 2 são enviadas para o cliente 1.                   
                    sprintf(buffer_send, "5Pergunta 1: %s \na) %s \nb) %s \nc) %s \nd) %s \nDigite a sua resposta (a, b, c ou d): ", pcliente2[0].pergunta, pcliente2[0].resp_1, pcliente2[0].resp_2, pcliente2[0].resp_3, pcliente2[0].resp_4);
                    send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                    
                    // A resposta da pergunta é recebida.
                    recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);

                    // Reliza-se a comparação para verificar se a resposta está correta.
                    if (buffer_receive[0] == pcliente2[0].resp_certa[0])
                    {
                        // Se a resposta for certa, o servidor enviada uma mensagem ao cliente 1 alertando-o,
                        // junto à contagem de pontos.
                        // Após isso, o cliente 1 ainda envia uma código de confirmação ao servidor.
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        
                        // Soma-se 50 pontos ao total de pontos do cliente 1 e
                        // a pontução é salva em sua struct de perguntas.
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
                        // Se a resposta estiver errada, o cliente 1 receberá uma mensagem de alerta e sua pontuação.
                        // Após o envio da mensagem, o cliente 1 irá enviar um código de confirmação ao servidor.
                        // Neste caso, nenhum ponto é somado.
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                        sprintf(buffer_send, "Resposta errada!\nA resposta certa é: %s \nVocê tem %d pontos!", pcliente2[0].resp_certa, pontos_cliente1);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }

                    // Zerando ambos os buffers antes de enviar a próxima pergunta.
                    bzero(buffer_receive, sizeof(buffer_receive));
                    bzero(buffer_send, sizeof(buffer_send));

                    // O que foi descrito para a primeira pergunta, se repete exatamente da mesma forma
                    // para as duas seguintes.
                    // Abaixo, a segunda pergunta sendo enviada e corrigida.
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
                    
                    // Abaixo, a terceira pergunta sendo enviada e corrigida.
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

                    // Zerando todos os buffers antes de enviar e receber novas mensagem.
                    bzero(buffer_receive, sizeof(buffer_receive));
                    bzero(buffer_send, sizeof(buffer_send));

                    // Realiza-se a comporação de pontos entre os dois clientes.
                    if (pcliente1[0].pontos > pcliente2[0].pontos)
                    {
                        // Se a contagem de pontos na struct do primeiro cliente for maior, o primeiro cliente ganha.
                        // Uma mensagem é enviada para o primeiro cliente dizendo que ele ganhou.
                        snprintf(buffer_send, sizeof(buffer_send), "8Você venceu! :D O player 2 fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        
                        // O cliente 1 ainda envia um código de confirmação.
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    else if (pcliente1[0].pontos < pcliente2[0].pontos)
                    {
                        // Se a contagem de pontos do cliente 1 for menor que a do cliente 2, o cliente 2 ganha.
                        snprintf(buffer_send, sizeof(buffer_send), "8Você perdeu! :( O player 2 fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }
                    else
                    {
                        // Se nenhuma das condições acima for verdadeira, o jogo é empatado.
                        // Uma mensagem de empate é enviada ao cliente 1
                        snprintf(buffer_send, sizeof(buffer_send), "8Empate! :D O player 2 também fez %d pontos\n", pcliente2[0].pontos);
                        send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
                        recv(socket_cliente1, buffer_receive, sizeof(buffer_receive), 0);
                        bzero(buffer_receive, sizeof(buffer_receive));
                        bzero(buffer_send, sizeof(buffer_send));
                    }

                    // Após o cliente 1 terminar o jogo,
                    // a variável is_game_end torna-se verdadeira.
                    // Portato, é chamada a conexão novamente com o cliente 2 (no mesmo IṔ e porta).
                    // E, na sua função, é enviada a conclusão da contagem de pontos.
                    is_game_end = 1;
                    connect_client2(ip_cliente2, port_cliente2, pcliente2, pcliente1, &pcliente1->pontos, &pcliente2->pontos, &is_game_end);
                }
            }
            else
            {
                // Se a resposta do cliente 1 for diferente de 's', é enviada uma mensagem de jogo cancelado.
                strcpy(buffer_send, "\n\nJogo cancelado.\n\n");
                send(socket_cliente1, buffer_send, strlen(buffer_send), 0);
            }
            // Após a execução de toda a rotina, o cliente 1 é desconectado e o socket fechado.
            close(socket_cliente1);
            exit(0);
        } 
        // Caso a criação do processo filho não tenha sucesso, o socket é fechado. 
        else
        {
            close(socket_cliente1);
        }
    }
}