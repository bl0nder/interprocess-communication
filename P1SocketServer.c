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

  // Generate and send 50 random strings of size 5
  int i;
  char buffer[BUFFER_SIZE + 1];
  for (i = 0; i < 50; i++) {
    int j;
    for (j = 0; j < BUFFER_SIZE; j++) {
      buffer[j] = 'A' + rand() % 26;
    }
    buffer[BUFFER_SIZE] = '\0';

    if (write(clientfd, buffer, BUFFER_SIZE) < 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  // Close the socket
  close(clientfd);
  close(sockfd);

  return 0;
}