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

struct memRegion {
    char toSend[6];
    char ind[5];
};

int main() {
  struct memRegion* region;
  struct timespec start;
  struct timespec end;
  

  int randInd = 0;
  char highestInd[10];

  int shm;
  if ((shm = shm_open(SHM_PATH, O_CREAT | O_RDWR, 666)) < 0) {
    perror ("[SERVER] Error in opening shared memory\n");
    exit(1);
  }

  // if (ftruncate(shm, 10*sizeof(struct memRegion)) < 0) {
  //   perror ("[SERVER] Error in truncating shared memory region\n");
  //   exit(1);
  // }

  region = mmap(NULL, 40*sizeof(struct memRegion), PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);

  char recvString[6];
  int maxInd;
  int startTime = clock_gettime(CLOCK_REALTIME, &start);
  while (1) {

    // for (int i=randInd; i<randInd+5; i++) {
    //     sprintf(region -> toSend, "%s", arr[i]);
    //     sprintf(region -> ind, "%d", i);

    //     region += 5 + 4 + 1;
    // }

    for (int i=0; i<5; i++) {
      sscanf(region -> toSend, "%s", recvString);
      sscanf(region -> ind, "%d", maxInd);
      region += 5+4+1+1;
    }

    sprintf(region -> ind, "%d", maxInd);
    region += 5+4+1+1;

    sleep(2);
  

    printf("[CLIENT] String: %s, ID: %d\n", recvString, maxInd);

    if (maxInd == 49) {
      break;
    }

    randInd += 5;
  }

  int endTime = clock_gettime(CLOCK_REALTIME, &end);
  double runTime = (end.tv_sec + 1.0e-9*end.tv_nsec - (start.tv_sec + 1.0e-9*start.tv_nsec));

  printf("[SERVER] Time taken to receive 50 acknowledgements: %lfs\n", runTime);

  munmap(region, 40*sizeof(struct memRegion));
  close(shm);
  
  return 0;
}