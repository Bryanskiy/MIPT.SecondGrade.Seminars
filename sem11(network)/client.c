#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

struct sockaddr_in client_setup(int sockfd) {
    struct sockaddr_in cliaddr;

    bzero(&cliaddr, sizeof(cliaddr));
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sockfd, (struct sockaddr*)&cliaddr, sizeof(cliaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    return cliaddr;
}

struct sockaddr_in server_setup(int sockfd, char* ip) {
    struct sockaddr_in servaddr;

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    if(inet_aton(ip, &servaddr.sin_addr) == 0) {
        printf("Invalid IP addres\n");
        exit(1);
    }

    return servaddr;
}

int create_socket() {
    int sockfd;

     if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
        perror(NULL);
        exit(1);
    }

    return sockfd;
}

int main(int argc, char** argv) {
    if(argc != 2) {
        printf("Usage: a.out <IP addres\n>");
        exit(1);
    }
    
    int sockfd = create_socket();
    struct sockaddr_in cliaddr = client_setup(sockfd);
    struct sockaddr_in servaddr = server_setup(sockfd, argv[1]);

    printf("Please, enter 2 numbers: ");
    int arr[2];
    scanf("%d %d", &arr[0], &arr[1]);
    if(sendto(sockfd, arr, sizeof(arr), 0, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    int ans;
    if(recvfrom(sockfd, &ans, sizeof(int), 0, (struct sockaddr*)NULL, NULL) < 0) {
        perror(NULL);
        close(sockfd);
        exit(1);
    }

    printf("ans = %d\n", ans);
    close(sockfd);
    return 0;
}