#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <string.h>
#include <arpa/inet.h>
#include <stdlib.h>

using namespace std;
int main()
{
    int sockfd , portno , n;
    struct sockaddr_in server_addr;
    struct hostent * server;
    string buffer;
    char ip[INET_ADDRSTRLEN];
    portno = 6440;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if (sockfd == -1) {
        perror("open socket failed!");
        close(sockfd);
        return 0;
    }
    
    server = gethostbyname("ubuntu");
    char ** addr_list = server->h_addr_list;
    for(int i = 0 ; addr_list[i] != NULL ; i++) {
        inet_ntop(server->h_addrtype , addr_list[i], ip, sizeof(ip));
        cout << ip << ':' << portno << endl;
    }
    


    bzero((char*) &server_addr , sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr , (char*)&server_addr.sin_addr.s_addr , server->h_length);
    server_addr.sin_port = htons(portno);


    if(connect(sockfd, (struct sockaddr *)&server_addr ,sizeof(server_addr)) == -1) {
        perror("connect failed!");
        close(sockfd);
        return 0;
    }

    while(1) {
        cout << "enter message('q' to quit)): ";
        getline(cin , buffer);
        if( !buffer.compare("q") || !buffer.compare("Q") ) break;
        int str_len = buffer.size()+1;
        char * char_buf=(char*)malloc(str_len);
        if(char_buf == NULL) {
            perror("malloc error!");
            free(char_buf);
            close(sockfd);
            return 0;
        }
        sprintf(char_buf, "%s", buffer.c_str()) ;
        int send_len = strlen(char_buf);
        int sent = 0;
        //send
        while(sent < send_len) {
            int result = send(sockfd, char_buf, send_len, 0);
            if (result == -1) {
                perror("send error!");
                free(char_buf);
                close(sockfd);
                return 0;
            }
            sent += result;
        }
        
        bzero(char_buf,send_len);
        int recv_res = 0;
        //eco
        while(1) {
            recv_res += recv(sockfd, char_buf , sent , 0);
            if(recv_res == -1) {
                perror("send error!");
                free(char_buf);
                close(sockfd);
                return 0;
            }
            cout << "eco: " << char_buf << endl;
            if(recv_res == send_len) break;
        }
        
        free(char_buf);   
    }
    
    
    close(sockfd);
    return 0;
}