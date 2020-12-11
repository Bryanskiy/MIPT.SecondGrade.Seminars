#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <pthread.h>

int server_setup() {
    int sockfd;
    struct sockaddr_in servaddr;
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }

    if(bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    return sockfd;
}

int main() {
    struct sockaddr_in cliaddr;
    int sockfd = server_setup();

    while(1) {
       int clilen = sizeof(cliaddr);
        int arr[2];
        if(recvfrom(sockfd, arr, sizeof(arr), 0, (struct sockaddr*)&cliaddr, &clilen) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);            
        }

        int ans = arr[0] * arr[1];
        printf("%d\n", ans);

        if(sendto(sockfd, &ans, sizeof(int), 0, (struct sockaddr*)&cliaddr, clilen) < 0) {
            perror(NULL);
            close(sockfd);
            exit(1);
        }
    }

    return 0;
}