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

#define BUFFER_SIZE 1024

/*1. Faça um programa cliente para se conectar e enviar/receber mensagens de um serividor
HTTP. O programa deverá ler do teclado o nome do host que deseja se conectar (localhost) 
e o nome do objeto (Ex: index.html) que o usuário deseja receber do servidor.

Após o estabelecimento da conexão com o servidor, o programa cliente deverá enviar um HTTP
Request (GET) do objeto digirado pelo usuário e mostrar na tela a resposta do servidor.

Verifique a sintaxe do comando GET na documento do protocolo HTTP.

OBS: Para esse exercício o servidor apache deve estar rodando no ambiente Linux. Crie uma
página html (index.html) teste e coloque dentro do servidor.
*/

int main(void){
    struct sockaddr_in sock;
    int con, sockid;
    char ende[20];
    char pag[100];
    int port;
    char buffer[BUFFER_SIZE];
    char get_concat[500];
    struct in_addr addr;

    printf("\nInsira um endereco ip: \n");
    scanf("%s", ende);
    __fpurge(stdin);
    printf("\nInsira a página: \n");
    scanf("%s", pag);
    __fpurge(stdin);

    sockid = socket(AF_INET, SOCK_STREAM, 0); //criando o socket
    
    bzero(&(sock), sizeof(sock));    
    sock.sin_family = AF_INET;
    sock.sin_port = htons(80);
    inet_pton(AF_INET, ende, &sock.sin_addr);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0){
        printf("\nErro ao conectar\n");
    }
    else{
        printf("\nConectado\n");
    }
    bzero(buffer, BUFFER_SIZE);
    snprintf(get_concat, sizeof(get_concat), "GET /%s\r\n", pag);
    printf("\n%s\n", get_concat);

    if(send(sockid, get_concat, strlen(get_concat), 0)==-1){
        printf("\nErro ao enviar\n");
    }
    else{
        printf("\nEnviado\n");
    }

    bzero(buffer, BUFFER_SIZE);
    while(recv(sockid, buffer, BUFFER_SIZE-1, 0) != 0)
    {
        printf("%s", buffer);
       
    }
    close(sockid);
}