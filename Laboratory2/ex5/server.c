#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>

void handle_client(int sock_client, struct sockaddr_in* client){

    int over = 0;
    while(!over) {
    int out, msg_len;
     recv(sock_client, &msg_len, sizeof(int), 0);
    msg_len = ntohs(msg_len);
    printf("received from client: %d\n", msg_len);
    over = 1;

    char* msg = malloc((msg_len+1)*sizeof (char));

    recv(sock_client, msg, msg_len+1, 0);
    printf("%s\n", msg);

    struct sockaddr_in new;
    int new_socket;

    new_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(new_socket < 0){
        perror("cannot open socket");
        exit(5);
    }

    struct hostent* h;
    h = gethostbyname(msg);

    if (h==NULL) {
        printf("%s: unknown host \n", msg);
        exit(1);
    }


    new.sin_family = AF_INET;
    new.sin_port = htons(80);
    memcpy((char*)&new.sin_addr.s_addr, h->h_addr_list[0], h->h_length);

    int rc = connect(new_socket, (struct sockaddr *) &new, sizeof(new));

    if(rc<0) {
        perror("cannot connect ");
        exit(1);
    }

    char get_req[] = "GET / HTTP/1.0\n\n";
    int byte_count = write(new_socket, get_req, strlen(get_req));
    if (byte_count < 0) perror("ERROR writing to socket");

    char* msg1 = malloc((256)*sizeof (char));


    while(read(new_socket, msg1, 256 - 1) != 0){
       // printf("%s", msg1);
        write(sock_client, msg1, strlen(msg1));
        bzero(msg1, 256);
    }


        free(msg);
        free(msg1);
    }
    close(sock_client);
    printf("Connection to the client %s closed\n", inet_ntoa(client->sin_addr));
}

int main(int argc, char** argv) {

    struct sockaddr_in server, client;
    int sock_client, len;
    int sock_server;

    sock_server = socket(AF_INET, SOCK_STREAM, 0);

    if(sock_server<0){
        printf("Could not create socket!\n");
        return -1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_port = htons(atoi(argv[1]));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;

    if(bind(sock_server, (struct sockaddr*)&server, sizeof(server))<0){
        perror("Error binding address to socket");
        return -2;
    }

    if(listen(sock_server,5)<0){
        perror("Error setting up the backing queue: ");
        return -3;
    }

    len  = sizeof(client);
    memset(&client, 0, sizeof(client));

    sock_client = accept(sock_server, (struct sockaddr_*)& client, (socklen_t*)&len);
    if(sock_client < 0){
        perror("Error on accepting the client\n");
        return -4;
    }


    handle_client(sock_client, &client);

    return 0;
}
