#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

int main() {
    
    char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=[];',./";
    char arr[50][5];

    for (int i=0; i<50; i++) {
        for (int j=0; j<5; j++) {
            arr[i][j] = chars[(rand() % (72-0+1)) + 0];
        }
    }

    //Call socket()
    int socketServer;
    int socketClient;

    struct sockaddr_un server;
    struct sockaddr_un client;

    socketServer = socket(AF_UNIX, SOCK_STREAM, 0);

    //Set struct attributes
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "OSSocket");

    //bind socket
    bind(socketServer, (const sockaddr*) &server, sizeof(server));

    //listen for messages
    listen(server, 3);

    //accept incoming connection
    socketClient = accept(server, &client, sizeof(struct sockaddr_un));

    int randomInd = (rand() % (72-0+1)) + 0;
    int ind = randomInd;


    while (ind < 50) {
        char str[50];

        for (int i=0; i<5; i++) {
            strcat(str, arr[ind+i]);
            strcat(str, ind+i);
            send(client, str, 50, 0);
            printf("[SENT] Index: %d, String: %s\n", ind+i, arr[ind+i]);
        }

        char ans[100];

        for (int i=0; i<5; i++) {
            recv(client, ans, 100, 0);
        }
        
        ind = atoi(ans)+1;
    }
        
    return 0;
}
