#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>



#define SOCKET_PATH "/tmp/socket"
#define BUFFER_SIZE 5

int main(int argc, char *argv[]) {

  char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
  char arr[50][6];
  for (int i=0; i<50; i++) {
      for (int j=0; j<5; j++) {
          arr[i][j] = chars[(rand() % (61-0+1)) + 0];
      }
      arr[i][5] = '\0';
  }
  

  int randInd = (rand()%(30-0+1))+0;
  char highestInd[10];

  // Create the socket
  int sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // Set up the socket address
  struct sockaddr_un addr;
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);
  unlink(addr.sun_path);

  // Bind the socket to the address
  if (bind(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(sockfd, 5) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // Accept a connection
  int clientfd = accept(sockfd, NULL, NULL);
  if (clientfd < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  
  // while(randInd<= 57) {
  //   for (int i=randInd; i<randInd+5; i++) {


  //     char sendInd[8];
  //     sprintf(sendInd, "%d", i);
      
  //     char toSend[25];
  //     strcpy(toSend, "");
  //     strcat(toSend, arr[i]);

  //     printf("[SERVER] Sending: %s\n", toSend);
      
  //     if (send(clientfd, toSend, sizeof(toSend) + 1) < 0) {
  //       perror("Error while writing string to client socket");
  //       exit(1);
  //     }

      

  //     // if (write(clientfd, sendInd, 8) < 0) {
  //     //   perror("Error while writing string index to client socket");
  //     //   exit(1);
  //     // }

  //     randInd++;
  //     // sleep(2);
  //   }
  // }
  int i= randInd;
  while (1) {

    char toSend[25];
    strcpy(toSend, "");
    strcat(toSend, arr[i]);

    int maxInd;
    
    printf("[SERVER] Sending %s\n", toSend);

    if (send(clientfd, toSend, sizeof(toSend) + 1, 0) < 0) {
      perror("Error while writing string to client socket");
      exit(1);
    }

    printf("[SERVER] Sending %d\n", i);

    if (send(clientfd, &i, sizeof(int), 0) < 0) {
      perror("Error while sending index to client\n");
      exit(1);
    }


    if (recv(clientfd, &maxInd, sizeof(int), 0) < 0) {
      perror("Error while reading index from client\n");
      exit(1);
    }
    
    printf("[SERVER] Received %d\n", maxInd);

    if (maxInd==61) {
      break;
    }

    i++;
  }

  if (read(clientfd, highestInd, 10) < 0) {
    perror("Error while trying to read from client socket");
    exit(1);
  }

  printf("Client socket sent me this: %s", highestInd);

  // Close the socket
  close(clientfd);
  close(sockfd);

  return 0;
}