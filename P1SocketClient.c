#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>

int main() {

    //Call socket()
    int socketServer;
    int socketClient;

    struct sockaddr_un server;

    socketServer = socket(AF_UNIX, SOCK_STREAM, 0);

    //Set struct attributes
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, "OSSocket");

    //accept incoming connection
    int temp = sizeof(server);
    socketClient = connect(socketServer, (struct sockaddr*) &server, temp);

    printf("Connected to server\n");

    char strings[100];
    char test[100] = "hello";

    while (fgets(strings, 100, stdin), !feof(stdin)) {
        if (recv(socketServer, strings, 100, 0) > 0 ) {
            printf("%s\n", strings);
        }

        if (send(socketServer, test, 100, 0) == -1) {
            exit(1);
        }

        else {
            exit(1);
        }
    }
        
    return 0;
}
