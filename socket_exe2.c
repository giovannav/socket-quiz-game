#include <stdio.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>

/*2. Faça um programa cliente para se conectar a um servidor FTP (ProFTPD instalado na máquina
Linux).
O programa deverá mostrar na tela a primeira mensagem recebida desse servidor após a conexão, e,
após a mensagem de boas vindas enviada pelo servidor, permitir que o usuário envie comandos para
o servidor (a conexão entre cliente-servidor deverá permanecer ativa e aguardando por envio de
comando pelo cliente até que o usuário envie o comando quit).

Após a conexão, o servidor ifca aguardando que o programa cliente envie os comando de autenticação:
usuário e senha (nessa ordem).
O usuário deverá digitar seu usuário e senha e o programa cliente deve enviar os dados para o
servidor com a sintaxe correta.
Caso a senha digitada pelo usuário esteja incorreta, deverão ser enviados para o servidor um 
novo usuário e senha.

Observações:
Para a conexão, o usuário deverá entrar com o endereço IP e a porta do servidor que deseja
conectar. Para cada comando enviado para o servidor, o programa cliente deverá mostrar na tela
a resposta recebida do servidor.

Comandos para autenticação:

user nome_usuario --> nome do usuário que deseja autenticar
pass senha --> senha do usuário que deseja autenticar
*/

int main(void){
    struct sockaddr_in sock;
    int con, sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);
    char ende[20];
    int port;
    char get_concat[500];
    struct in_addr addr;

    printf("\nInsira um endereco ip: \n");
    scanf("%s", ende);
    __fpurge(stdin);

    bzero(&(sock), sizeof(sock));    
    sock.sin_family = AF_INET;
    sock.sin_port = htons(21);
    inet_pton(AF_INET, ende, &sock.sin_addr);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0){
        printf("\nErro ao conectar\n");
    }
    else{
        printf("\nConectado\n");
    }

    close(sockid);
}