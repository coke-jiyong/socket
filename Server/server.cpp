#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>

using namespace std;
#define MAX_BUF 1024
int main()
{
    int sockfd, newsockfd;
    struct sockaddr_in server_addr , client_addr;
    socklen_t client_len;
    char buffer[MAX_BUF];

    sockfd = socket(AF_INET,SOCK_STREAM, 0); //AF_INET : IPv4 / SOCK_STREAM : TCP socket / 0 : 기본프로토콜
    if ( sockfd == -1 ) {
        perror("open socket failed!");
        close(sockfd);
        return 0;
    }

    bzero((char*)&server_addr , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(6440);

    if( bind(sockfd, (struct sockaddr*)&server_addr , sizeof(server_addr)) == -1 ) {
        perror("bind error!");
        close(sockfd);
        return 0;
    }
    
    if( listen(sockfd,3) == -1 ) {
        perror("listen error!");
        close(sockfd);
        return 0;
    }
    
    client_len = sizeof(client_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
    if( newsockfd == -1 ) {
        perror("accept newsockfd failed!");
        close(sockfd);
        close(newsockfd);
        return 0;
    }
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_addr.sin_addr , client_ip, sizeof(client_ip));
    cout << "accept client ! " << "(" << client_ip << ")" << endl;

    //read(newsockfd, buffer , 255);
    int result = 0;
    while( (result = recv(newsockfd ,buffer,sizeof(buffer),0)) != 0 ) {
        if( result == -1 ) {
            perror("recv() failed!");
            close(sockfd);
            close(newsockfd);
            return 0;
        }
        if( send(newsockfd , buffer , result , 0) == -1) {
            perror("send() failed!");
            close(sockfd);
            close(newsockfd);
            return 0;
        }
    }
    
    
    close(sockfd);
    close(newsockfd);
    return 0;   
}