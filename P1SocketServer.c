#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


#define SOCKET_PATH "/tmp/OSSocket"

int main() {

  struct timespec start;
  struct timespec end;

  char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
  char arr[50][6];
  for (int i=0; i<50; i++) {
      for (int j=0; j<5; j++) {
          arr[i][j] = chars[(rand() % (61-0+1)) + 0];
      }
      arr[i][5] = '\0';
  }
  

  int randInd = 0;
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
  
  
  int maxInd;
  
  int startTime = clock_gettime(CLOCK_REALTIME, &start);
  while (1) {
    for (int i=randInd; i<randInd + 5; i++) {

      if (send(clientfd, arr[i], strlen(arr[i]) + 1, 0) < 0) {
        perror("Error while writing string to client socket");
        exit(1);
      }

      if (send(clientfd, &i, sizeof(int), 0) < 0) {
        perror("Error while sending index to client\n");
        exit(1);
      }
    } 


    if (recv(clientfd, &maxInd, sizeof(int), 0) < 0) {
      perror("Error while reading index from client\n");
      exit(1);
    }
    
    printf("[SERVER] Received %d\n", maxInd);

    if (maxInd==49) {
      break;
    }
    randInd += 5;
  }

  int endTime = clock_gettime(CLOCK_REALTIME, &end);
  double runTime = (end.tv_sec + 1.0e-9*end.tv_nsec - (start.tv_sec + 1.0e-9*start.tv_nsec));

  printf("[SERVER] Time taken to receive 50 acknowledgements: %lfs\n", runTime);

  // Close the socket
  close(clientfd);
  close(sockfd);

  return 0;
}