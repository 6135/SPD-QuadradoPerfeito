#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>

int main(){

    FILE *fpIn = fopen("timeLog.txt","r+");
    printf("files ready\n");
    double c[30];
    size_t i = 0;
    while((fscanf(fpIn,"%lf",&c[i++]))!=EOF){

    }
    double sum = 0.0;
    for (size_t j = 0; j < i; j++)
        sum+=c[j];
    
    fprintf(fpIn,"\nAverage: %lf",sum/30.0);

}