#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/socket"
#define BUFFER_SIZE 8
int main(int argc, char *argv[]) {
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
  char buffer[BUFFER_SIZE + 1];
  int bytes_read;

  // while ((bytes_read = read(sockfd, buffer, BUFFER_SIZE)) > 0) {
  //   buffer[bytes_read] = '\0';
  //   printf("%s ", buffer);
  // }
  
  char highestInd[15];

  // while ((bytes_read = read(sockfd, buffer, BUFFER_SIZE)) > 0) {
  //   buffer[bytes_read] = '\0';
  //   strcpy(highestInd, buffer);
  //   printf("[CLIENT] Received string: %s\n", highestInd);
  //   // sleep(2);
  // }

  
  // if (write(sockfd, highestInd, 8) < 0) {
  //   perror("Error while trying to write to server\n");
  //   exit(1);
  // }

  // // Check for read errors
  // if (bytes_read < 0) {
  //   perror("read");
  //   exit(EXIT_FAILURE);
  // }

  char recvStr[25];
  int ind;
  
  while (1) {

    for (int i=0; i<5; i++) {
      if (recv(sockfd, recvStr, sizeof(recvStr) + 1, 0) < 0) {
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

  }

  // Close the socket
  close(sockfd);

  return 0;
}