#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>
#include <inttypes.h>
#include<stdint.h>

int p_grupo(int n, int *a){
    return 3;
}

int array_group(int sum, int *original, int size_of_original, int *destination){
    int size_b = 0, j = 0;
    while(j < size_of_original){
        destination[size_b]=p_grupo(sum,original+j);
        j+=destination[size_b++];
    }
    return size_b;
}
void ints_print(int n, int *a){
    for (int i = 0; i < n; i++)
    {
        printf("%d ",a[i]);
    }
    printf("\n");
    
}
void test_stuff(){
    int MAX_SUM = 10;
    int a[] = {7,5,12,8,25,7,1,7,8,12,5};
    printf("Print1: %d %d\n", sizeof(a), sizeof(a)/sizeof(int) );
    int *a_teste = (void *)a;
    printf("Print2: %d\n",sizeof(a_teste));
    int *b = malloc(sizeof(a));
    int size_b = array_group(MAX_SUM,a,sizeof(a)/sizeof(int),b);
    ints_print(size_b,b);

}
int main(){
    test_stuff();
}