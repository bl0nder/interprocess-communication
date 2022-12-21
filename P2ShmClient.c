#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

#define SHM_PATH "shm"
#define SIZE 1024

struct memRegion {
    char toSend[15];
    char toRecv[15];
};

int main() {
  int randInd = 0;

  int shm;
  if ((shm = shm_open(SHM_PATH, O_CREAT | O_RDWR, 666)) < 0) {
    perror ("[SERVER] Error in opening shared memory\n");
    exit(1);
  }

  // if (ftruncate(shm, 10*sizeof(struct memRegion)) < 0) {
  //   perror ("[SERVER] Error in truncating shared memory region\n");
  //   exit(1);
  // }

  char* region = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

  // char recvString[12];

  int maxInd;
  while (1) {

    // for (int i=randInd; i<randInd+5; i++) {
    //     sprintf(region -> toSend, "%s", arr[i]);
    //     sprintf(region -> ind, "%d", i);

    //     region += 5 + 4 + 1;
    // }
    char maxIndString[5];


    for (int i=0; i<5; i++) {
      // strcpy(recvString, region);

      char* recvString = (char *) region;

      // printf("Recvstring: %s\n", recvString);
      // printf("Strlen: %d\n", strlen(recvString));
      // sscanf(region -> toRecv, "%s %d", recvString, &maxInd);
      region += strlen(recvString) + 1;

      char* token = strtok(recvString, " ");
      token = strtok(NULL, " ");

      strcpy(maxIndString, token);
      printf("[CLIENT] Received String: %s, ID: %s\n", recvString, maxIndString);
    }

    // printf("[CLIENT] MaxIndString = %s\n", maxIndString);
    sprintf(region, "%s", maxIndString);
    region += strlen(maxIndString)+1;


    if (!strcmp(maxIndString, "49")) {
      break;
    }

    randInd += 5;

    // sprintf(region, "%s", "x");
    // while (!strcmp((char*) region, "x"));

    sleep(1);
  }

  munmap(region, SIZE);
  close(shm);
  shm_unlink(SHM_PATH);
  
  return 0;
}