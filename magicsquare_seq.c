#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <ctype.h>
#include <time.h>

int order=0;
long long magicNumber=0;
long long sumDig=0;
long long sumAntiDig=0;

long long  *ints_new(int n)
{
    //  return (int *) calloc(n, sizeof (int));
    return (long long *)malloc(n * sizeof(long long));
}


void int_readfile(FILE *f, long long *a)
{
    int result=0;
	long long number= 0;
    for (int i = 0; i < order; i++)
    {
        fscanf(f, "%lld", &a[i]);
    }
}

// Alterado do https://stackoverflow.com/questions/13399594/how-to-extract-numbers-from-string-in-c;
int get_order(char *s)
{
    char *p = s;
    while (*p) { // While there are more characters to process...
        if ( isdigit(*p)) {
            return strtol(p, &p, 10); // Read number
        } else {
            p++;
        }
    }
}


long long sumRows(long long * row){
    long long result=0;
    for (int i = 0; i < order; i++)
    {
        result+= row[i];
    }
    return result;
}


void add_col(long long * row, long long * cols){
    for (int i = 0; i < order; i++)
    {
        cols[i]+= row[i];
    }
}

void add_col_first(long long * row, long long * cols){
    for (int i = 0; i < order; i++)
    {
        cols[i]= row[i];
    }
}



int check_cols(long long * cols){
    for (int i = 0; i < order; i++)
    {
        if (magicNumber != cols[i])
        {
            return -2;
        }  
    }
    return 0;
}


int check_dig(){
    if (sumDig != magicNumber || sumDig != sumAntiDig)
    {
       return -1;
    }
    return 0;
}


int test(FILE *f){
    
    long long *row= ints_new(order);
    long long *cols= ints_new(order);
    int check=0;
    for (int i = 0; i < order; i++)
    {
        int_readfile(f,row);
        if (i==0)
        {
           magicNumber= sumRows(row);
           add_col_first(row, cols);
           sumDig+= row[i];
           sumAntiDig+= row[order-1-i];
        }
        else
        {
            long long sumRow= sumRows(row);
            if (magicNumber == sumRow)
            {
                add_col(row, cols);
                sumDig+= row[i];
                sumAntiDig+= row[order-1-i];
            }
            else
            {
                check=-2;
                break;
            }
        }
    }
    if (check==0)
    {
        check = check_cols(cols);
    }
    if (check==0)
    {
        check=check_dig();
    }
    return check;
}


int main(int argc, const char **argv)
{
    struct timeval start, end; 
    mingw_gettimeofday(&start, NULL); 
    char namefile[50];
    strcpy(namefile, argv[1]);
    order= get_order(namefile);
    FILE *f = fopen(argv[1], "r");
    int finish= test(f);
    // if (finish == 0)
	// {
	// 	printf("Quadrado magico\n");
	// }
	// else if (finish == -1)
	// {
	// 	printf("Quadrado magico imperfeito\n");
	// }
	// else
	// {
	// 	printf("Nao e quadrado magico.\n");
	// }
    mingw_gettimeofday(&end, NULL);
    double time_taken = (end.tv_sec - start.tv_sec) * 1e6; 
    time_taken = (time_taken + (end.tv_usec -  start.tv_usec)) * 1e-6; 
    printf("%lf\n",time_taken);
}