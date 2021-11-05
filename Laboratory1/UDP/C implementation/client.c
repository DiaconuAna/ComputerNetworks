#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>

void error(char* msg){
    perror(msg);
    exit(1);
}

int main(int argc, char** argv) {

    int numbers[20];
    int i;
    int sockfd, length;
    struct sockaddr_in server, client;

    if(argc < 2){
        printf("Usage: %s <port> \n", argv[0]);
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
        error("Error on opening client socket");

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr-("192.168.100.40");
    server.sin_port = htons(atoi(argv[1]));

    int n;
    printf("How many numbers dooes the array have?\n");
    scanf("%d",&n);

    for(i=0;i<n;i++){
        printf("a[%d] = ",i);
        scanf("%d",&numbers[i]);
        printf("\n");
    }

    int tmp;

    tmp = sendto(sockfd, &n, sizeof(n), 0, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

    if(tmp < 0){
        error("Error when sending array  length to server");
    }

    tmp = sendto(sockfd, numbers, sizeof(numbers), 0, (struct sockaddr*)&server, sizeof(struct sockaddr_in));

    if(tmp < 0){
        error("Error when sending array to server");
    }

    printf("Array sent to server\n");
    //receive sum from server

    int sum;
    int fromlen = sizeof(struct sockaddr_in);
    tmp = recvfrom(sockfd, &sum, sizeof(sum),0, (struct sockaddr*)&server, &fromlen);

    printf("Sum is: %d\n", sum);

    return 0;
}
