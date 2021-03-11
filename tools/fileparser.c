#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>


int main(){

    FILE *fpIn = fopen("inputs/unparsed/inputs10000x10000.txt","r+");
    printf("files ready\n");
    FILE *fpOut = fopen("inputs/unparsed/inputs10000x10000.out.txt","w+");
    printf("files ready\n");
    char c;
    int spaceNumbers = 0;
    long long numbers = 1;
    while((c = fgetc(fpIn))!=EOF){
        // printf("%c",c);
        if(c == ',') {
            spaceNumbers=0;
        } else if( c == ' '){
            spaceNumbers++;
            if(spaceNumbers < 2)
                fputc(c,fpOut);
        } else {
            spaceNumbers = 0;
            fputc(c,fpOut);
            if(c=='\n'){
                numbers++;
                printf("Line read, line: %d\n",numbers);
                }
        }
    }
        

}