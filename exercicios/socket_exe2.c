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
#include <stdlib.h>

#define BUFFER_SIZE 10000

/*2. Faça um programa cliente para se conectar a um servidor FTP (ProFTPD instalado na máquina
Linux).
O programa deverá mostrar na tela a primeira mensagem recebida desse servidor após a conexão, e,
após a mensagem de boas vindas enviada pelo servidor, permitir que o usuário envie comandos para
o servidor (a conexão entre cliente-servidor deverá permanecer ativa e aguardando por envio de
comando pelo cliente até que o usuário envie o comando quit).

Após a conexão, o servidor fica aguardando que o programa cliente envie os comando de autenticação:
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
    char user[30];
    char pass[30];
    int port;
    char cmd[500];
    char get_concat_user[500];
    char get_concat_pass[500];
    char get_concat[500];
    struct in_addr addr;
    char buffer[BUFFER_SIZE];
    int control = 1;

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

    while(control == 1){
        if (recv(sockid, buffer, BUFFER_SIZE-1, 0) != 0){
            printf("\n%s\r\n", buffer);
            bzero(buffer, BUFFER_SIZE);

            printf("\nInsira um usuário: \n");
            scanf("%s", cmd);
            __fpurge(stdin);
            snprintf(get_concat_user, sizeof(get_concat_user), "%s\n", user);
            send(sockid, get_concat, strlen(get_concat), 0);
            printf("\n%s\r\n", buffer);
            bzero(buffer, BUFFER_SIZE);

            printf("\nInsira uma senha: \n");
            scanf("%s", cmd);
            __fpurge(stdin);
            snprintf(get_concat_user, sizeof(get_concat_user), "%s\n", user);
            send(sockid, get_concat, strlen(get_concat), 0);
            printf("\n%s\r\n", buffer);
            bzero(buffer, BUFFER_SIZE);

            printf("\nInsira um comando: \n");
            scanf("%s", cmd);
            __fpurge(stdin);
            snprintf(get_concat, sizeof(get_concat), "%s\n", cmd);
            send(sockid, get_concat, strlen(get_concat), 0);
            printf("\n%s\r\n", buffer);
            bzero(buffer, BUFFER_SIZE);

            // TESTAR TUDO ISOS ACIMA!!!!!
        }
    }
    /*bzero(buffer, BUFFER_SIZE);
    while(control == 1)
    {   
        if (recv(sockid, buffer, BUFFER_SIZE-1, 0) != 0){
            printf("%s", buffer);
            bzero(buffer, BUFFER_SIZE);
            printf("\nDigite o usuário:\n");
            scanf("%s", user);
            __fpurge(stdin);
            if(send(sockid, user, strlen(user), 0)==-1){
                printf("\nErro ao enviar usuário.\n");
            }
            else {
                recv(sockid, buffer, BUFFER_SIZE-1, 0);
                printf("\n%s\n", buffer);
                bzero(buffer, BUFFER_SIZE);
            }

            printf("\nDigite a senha:\n");
            scanf("%s", pass);
            __fpurge(stdin);

            if(send(sockid, pass, strlen(pass), 0)==-1){
                printf("\nErro ao enviar senha.\n");
            } 
            else{
                recv(sockid, buffer, BUFFER_SIZE-1, 0);
                printf("\n%s\n", buffer);
                bzero(buffer, BUFFER_SIZE);
            }

            printf("\nDigite o comando:\n");
            scanf("%s", cmd);
            __fpurge(stdin);
            if (strcmp(cmd, "quit") == 0){
                control = 0;
            }
            else{
                recv(sockid, buffer, BUFFER_SIZE-1, 0);
                printf("\n%s\n", buffer);
                bzero(buffer, BUFFER_SIZE);
            }
        
        }
        else{
            exit(0);
        }
        
    }*/

    //printf("\nInsira um usuario: \n");

    close(sockid);
}