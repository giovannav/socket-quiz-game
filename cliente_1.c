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

#define BUFFER_SIZE 1024

void welcome(){
    printf("******************************");
    printf("\nBem vindo ao cliente_1.c\n");
    printf("\nCertifique-se que o cliente 2 também está online.\n");
    printf("\nPressione qualquer tecla para continuar.\n");
    printf("******************************");
}

int main(void)
{

    welcome();
    getchar();

    struct sockaddr_in sock;
    int con, sockid;
    int port = 5002;
    char buffer_recv[BUFFER_SIZE];
    char buffer_send[BUFFER_SIZE];
    char get_ip[20];
    char get_port[20];
    struct in_addr addr;

    sockid = socket(AF_INET, SOCK_STREAM, 0); //criando o socket
    
    bzero(&(sock), sizeof(sock));    
    sock.sin_family = AF_INET;
    sock.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &sock.sin_addr);

    con = connect(sockid, (struct sockaddr *)&sock, sizeof(sock));

    if (con < 0){
        printf("\nErro ao conectar\n");
        close(sockid);
    }
    

    while(recv(sockid, buffer_recv, BUFFER_SIZE, 0) != 0)
    {   
        printf("Buffer antes: %s\n", buffer_recv);
        if(memcmp(buffer_recv, "\n*******\nDigite o ip com que deseja se conectar:\n*******\n", strlen("\n*******\nDigite o ip com que deseja se conectar:\n*******\n")) == 0)
        {
            printf("\nEntrou memc\n");
            scanf("%s", get_ip);
            __fpurge(stdin);
            send(sockid, get_ip, strlen(get_ip), 0);
            
        }
        
        printf("Antes da compraçao\n");
        printf("\nBuffer %s\n",buffer_recv);
        if (!strcmp(buffer_recv,"\nDigite a porta do cliente 2:\n"))
        //else if(memcmp(buffer_recv, "\nDigite a porta do cliente 2:\n\0", strlen("\nDigite a porta do cliente 2:\n\0")) == 0)
        {printf("\nentrou no str\n");
            printf("%s", buffer_recv);
            scanf("%s", get_port);
            __fpurge(stdin);
            send(sockid, get_port, strlen(get_port), 0);
            bzero(buffer_recv, sizeof(buffer_recv));
        }
        printf("\ndepois\n");
        bzero(buffer_recv, sizeof(buffer_recv));
    }
    close(sockid);
}