#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {

    int sd, rc, i;
    struct sockaddr_in servAddr;
    struct hostent* h;
    char msg[300];

    if(argc < 3){
        printf("<server> <domain_name>");
        exit(1);
    }


    servAddr.sin_addr.s_addr = inet_addr("192.168.100.20");
    servAddr.sin_port = htons(atoi(argv[1]));
    servAddr.sin_family = AF_INET;


    sd = socket(AF_INET, SOCK_STREAM, 0);
    if(sd < 0){
        perror("cannot open socket");
        exit(2);
    }
   // printf("sending data to server\n");

    rc = connect(sd, (struct sockaddr*) &servAddr, sizeof(servAddr));

    printf("sending data to server\n");

    if(rc<0){
        perror("cannot connect to the server");
        exit(1);
    }

    int msg_len = strlen(argv[2]);
    msg_len = htons(msg_len);
    send(sd, &msg_len, sizeof(msg_len), 0);


    strcpy(msg, argv[2]);
    printf("%s\n", msg);
    int cod = send(sd, msg, strlen(msg)+1, 0);
    if (cod != strlen(msg) + 1){
        perror("Could not send data to server");
        exit(3);
    }

    char msg1[256];

    while(read(sd, msg1, 256-1)){
        printf("%s", msg1);
        bzero(msg1, 256);
    }

    return 0;
}
