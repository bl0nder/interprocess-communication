#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define FIFO_PATH "/tmp/OSFifo"

int main() {

  //Create fifo
  if (mkfifo(FIFO_PATH, 666) < 0) {
    perror("[CLIENT] Error is creating a FIFO\n");
  }
  
  char recvString[6];
  int ind;

  while (1) {
    //Open fifo in read-only mode so that client can read info from server
    int fifo;
    if ((fifo = open(FIFO_PATH, O_RDONLY)) < 0) {
        perror("[CLIENT] Error in opening the FIFO in read-only mode\n");
        exit(1);
    }


    for (int i=0; i<5; i++) {
      if (read(fifo, recvString, 6) < 0) {
        perror("[CLIENT] Error in reading string from FIFO\n");
        exit(1);
      }

      if (read(fifo, &ind, sizeof(int)) < 0) {
        perror("[CLIENT] Error in reading index from FIFO\n");
        exit(1);
      }
    }

    close(fifo);

    //Now open fifo in write-only mode to send largest index to server 
    if ((fifo = open(FIFO_PATH, O_WRONLY)) < 0) {
      perror("[CLIENT] Error in opening the FIFO in write-only mode\n");
    }

    if (write(fifo, &ind, sizeof(int))<0) {
      perror ("[CLIENT] Error in writing maximum index to FIFO\n");
      exit(1);
    }
    close(fifo);

    if (ind == 49) {
      break;
    }
  }
  return 0;
}