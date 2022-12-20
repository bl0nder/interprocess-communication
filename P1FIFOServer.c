#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define FIFO_PATH "/tmp/OSFifo"

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

  //Create fifo
  if (mkfifo(FIFO_PATH, 666) < 0) {
    perror("[SERVER] Error is creating a FIFO\n");
  }


  int startTime = clock_gettime(CLOCK_REALTIME, &start);
  while (1) {
    //Open fifo in write mode so that server can send info to client
    int fifo;
    if ((fifo = open(FIFO_PATH, O_WRONLY)) < 0) {
        perror("[SERVER] Error in opening the FIFO in write-only mode\n");
        exit(1);
    }

    for (int i=randInd; i<randInd+5; i++) {
      if (write(fifo, arr[i], strlen(arr[i])+1) < 0) {
        perror("[SERVER] Error in writing string to FIFO\n");
        exit(1);
      }

      if (write(fifo, &i, sizeof(int)) < 0) {
        perror("[SERVER] Error in writing index to FIFO\n");
        exit(1);
      }
    }

    close(fifo);

    int maxInd;

    //Now open fifo in read-only mode to read largest index sent by client
    if ((fifo = open(FIFO_PATH, O_RDONLY)) < 0) {
      perror("[SERVER] Error in opening the FIFO in read-only mode\n");
    }

    if (read(fifo, &maxInd, sizeof(int))<0) {
      perror ("[SERVER] Error in reading maximum index from client\n");
      exit(1);
    }
    close(fifo);

    printf("[SERVER] Maximum index received: %d\n", maxInd);

    if (maxInd == 49) {
      break;
    }

    randInd += 5;

  }
  int endTime = clock_gettime(CLOCK_REALTIME, &end);
  double runTime = (end.tv_sec + 1.0e-9*end.tv_nsec - (start.tv_sec + 1.0e-9*start.tv_nsec));

  printf("[SERVER] Time taken to receive 50 acknowledgements: %lfs\n", runTime);
  return 0;
}