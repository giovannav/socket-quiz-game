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

int main(){
    int sockid;
    sockid = socket(AF_INET, SOCK_STREAM, 0);

    if(sockid < 0){
        printf("Erro ao criar o socket.\n");
        exit(1);
    }

    int opcao = 1;
    setsockopt(sockid, SOL_SOCKET, SO_REUSEADDR, &opcao, sizeof(int));

    //Determinar qual porta o servidor irá atuar
    struct sockaddr_in servidor;
    bzero(&servidor, sizeof(servidor));

    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(5002);
    servidor.sin_addr.s_addr = htonl(INADDR_ANY);

    if(bind(sockid, (struct sockaddr *)&servidor, sizeof(servidor)) < 0){
        printf("Erro ao fazer o bind.\n");
        close(sockid);
        exit(1);
    }

    listen(sockid, 5);
    
    while(1){
    int newSocket;
    struct sockaddr_in cliente;
    bzero(&cliente, sizeof(cliente));
    int c = sizeof(struct sockaddr_in);

    //Nessa linha o servidor ficará parado esperando a conexão de um cliente
    newSocket = accept(sockid, (struct sockaddr *)&cliente, (socklen_t *)&c);
    if (newSocket < 0){
        printf("Erro ao aceitar conexão.\n");
    }

    //Criar processo filho
    int pid = fork();

    if (pid < 0){
        printf("\nErro ao criar processo filho\n");
    }
    if(pid == 0){
        close(sockid);
        send(newSocket, "Conectado com sucesso\n", 22, 0);
    
    //Servidor vai fornecer um tipo de serviço aos clientes
    //Caso o cliente envie uma mensgem para o servidor, o servidor irá retornar a hora

    //Criando em um protocolo da camada de aplicação
    //Vamos definir quais mensagem o servidor vai aceitar, qual o formato, etc

    send(newSocket, "\nOlá, eu sou o servidor :)))).\n", 30, 0);
    
    char buffer[300];
    bzero(buffer, sizeof(buffer));
    send(newSocket, "\n\nDigite a hora: ", strlen("\n\nDigite a hora: "), 0);
    recv(newSocket, buffer, sizeof(buffer), 0);

    if(!strcmp(buffer, "hora\n") ){
        struct tm *info;
        time_t atual;
        atual = time(NULL);
        info = localtime(&atual);
        
        char strHora[100];
        bzero(&strHora, sizeof(strHora));
        sprintf(strHora, "%d:%d", info->tm_hour, info->tm_min);
        send(newSocket, strHora, strlen(strHora), 0);
    }
    else{
        send(newSocket, "Comando desconhecido", 21, 0);
    }

    close(newSocket);
    exit(0);
    } //if do filho
    else{//Processo pai
        close(newSocket);
    }
}   

}