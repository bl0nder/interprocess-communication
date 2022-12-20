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

// struct memRegion {
//     char toSend[15];
//     char toRecv[15];
// };

int main() {
  // struct memRegion* region;  

  char toSend[15];

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

  int shm;
  if ((shm = shm_open(SHM_PATH, O_CREAT | O_RDWR, 666)) < 0) {
    perror ("[SERVER] Error in creating shared memory\n");
    exit(1);
  }

  if (ftruncate(shm, SIZE) < 0) {
    perror ("[SERVER] Error in truncating shared memory region\n");
    exit(1);
  }

  char* region = mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm, 0);
  
  int startTime = clock_gettime(CLOCK_REALTIME, &start);
  while (1) {

    for (int i=randInd; i<randInd+5; i++) {
      
      char num[5];
      sprintf(num, "%d", i);

      char temp[12];
      sprintf(temp, "%s ", arr[i]);

      strcat(temp, num);
      strcat(temp, "\0");

      sprintf(region, "%s", temp);
      region += strlen(temp) + 1;
    }

    char tempMax[5];
    sprintf(tempMax, "%d", randInd + 5);

    while (strcmp(region, tempMax)) {
      continue;
    }

    // int maxInd;


    // sscanf(region -> toRecv, "%d", &maxInd);
    char maxIndStr[5];
    strcpy(maxIndStr, region);

    int maxInd = atoi(maxIndStr);
    printf("[SERVER] Maximum index received: %d\n", maxInd);

    if (maxInd == 49) {
      break;
    }

    randInd += 5;
  }
  int endTime = clock_gettime(CLOCK_REALTIME, &end);
  double runTime = (end.tv_sec + 1.0e-9*end.tv_nsec - (start.tv_sec + 1.0e-9*start.tv_nsec));

  printf("[SERVER] Time taken to receive 50 acknowledgements: %lfs\n", runTime);

  munmap(region, SIZE);
  close(shm);
  shm_unlink(SHM_PATH);
  return 0;
}