#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

//randomly generating a float number in a range
float float_rand( float min, float max )
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}


int main(int argc, char** argv) {
    int sockfd, rc,i;
    struct sockaddr_in serverAddr;
    u_int32_t number;

    if(argc<2){
        perror("Provide port as a command line argument!");
        exit(1);
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(atoi(argv[1]));
    serverAddr.sin_addr.s_addr = inet_addr("192.168.100.20");

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //TCP Socket

    if(sockfd < 0){
        perror("Cannot open socket");
        return -1;
    }

		//connect TCP Socket to the server
    rc = connect(sockfd, (struct sockaddr*)& serverAddr, sizeof(serverAddr));

    if(rc<0) {
        perror("cannot connect to the server");
        exit(1);
    }

    int count = 10;
    srand(time(0));
   // printf("hello, connected to server??\n");

//count used if we want to send a finite number of numbers
    while(1){
        number = rand()%1000;
        printf("Client sent %d to the server\n", number);

        number = htonl(number);

        if(send(sockfd, &number, sizeof(number), 0)<0){
            perror("Error on sending number to server");
            exit(2);
        }

        int time = rand() % 15;
//make the client wait between 0 and 14 seconds before sending the next number
        sleep((unsigned int) time);
        count --;
    }

    return 0;
}
