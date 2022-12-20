#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>


#define SOCKET_PATH "/tmp/OSSocket"

int main() {
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

  // Connect to the server
  if (connect(sockfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) < 0) {
    perror("connect");
    exit(EXIT_FAILURE);
  }

  // Read and print the strings sent by the server
  char highestInd[15];
  char recvStr[6];
  int ind;
  
  while (1) {

    for (int i=0; i<5; i++) {
      if (recv(sockfd, recvStr, 6, 0) < 0) {
        perror("Error in receiving string from server\n");
        exit(1);
      }

      if (recv(sockfd, &ind, sizeof(int), 0) < 0) {
        perror("Error in receiving index from server\n");
        exit(1);
      }
    }

    if (send(sockfd, &ind, sizeof(int), 0) < 0) {
      perror("Error in sending index to server\n");
      exit(1);
    }

    if (ind == 49) {
      break;
    }

  }

  // Close the socket
  close(sockfd);

  return 0;
}