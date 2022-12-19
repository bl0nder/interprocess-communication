#include <stdio.h>
#include <stdlib.h>


int main() {
    
    char* chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890-=[];',./";
    char arr[50][5];

    for (int i=0; i<50; i++) {
        for (int j=0; j<5; j++) {
            arr[i][j] = chars[(rand() % (72-0+1)) + 0];
            printf("%c", arr[i][j]);
        }
        printf(" ");
    }



    return 0;
}
