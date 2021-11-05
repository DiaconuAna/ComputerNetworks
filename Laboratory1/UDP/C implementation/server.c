#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>

void error(char* msg){
    perror(msg);
    exit(1);
}

int main(int argc, char** argv){
    int sockfd, n;
    struct sockaddr_in server, client;


    if(argc < 2){
        fprintf(stderr, "Error, no port provided!\n");
        exit(1);
    }

    sockfd = socket(AF_INET,  SOCK_DGRAM, 0);

    if(sockfd < 0){
        error("Error when opening server socket!");
    }

    bzero(&server, sizeof(server));

    server.sin_port = htons(atoi(argv[1]));
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;

    if (bind(sockfd, (struct sockaddr*)&server, sizeof(server))<0){
        error("Error when binding");
    }

    int tmp;
    int fromlen = sizeof(struct sockaddr_in);

    tmp = recvfrom(sockfd, &n, sizeof(int), 0 ,(struct sockaddr*)&client, &fromlen);

    if(tmp<0)
        error("Error on sending information to server");
    printf("%d\n", n);

    int numbers[20];
    tmp = recvfrom(sockfd, numbers, sizeof(numbers), 0, (struct sockaddr*)&client, &fromlen);

    if(tmp<0)
        error("Error on sending information to server");

    printf("array received from server\n");
    int s = 0;

    for(int i=0;i<n;i++)
        s += numbers[i];

    //send s to client

    tmp = sendto(sockfd,  &s, sizeof(s), 0, (struct sockaddr*)&client, fromlen);
    if(tmp<0)
        error("Error on sending information to server");


    return 0;
}
