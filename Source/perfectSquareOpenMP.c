#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <limits.h>
#include <ctype.h>
#include <omp.h>
#include <Windows.h>
#define AND &&
#define and AND
#define ISNOT !=
#define isnot ISNOT
#define IS ==
#define is IS
#define GTE >=
#define gte GTE
#define LTE <=
#define lte LTE
#define GT >
#define gt GT
#define LT <
#define lt LT
#define OR ||
#define or OR
#define NOT !
#define not NOT
#define True 1
#define False 0
#define Undef -1

#define elif else if
#define llong long long
#define ullong unsigned long long
#define THREAD_SKIP -6135

short is_magic_quare = True;  //1 means perfect, 0 means imperfect -1 means no magic square
ullong *data;
ullong order = 0;

const int OMP_NUM_THREADS = 8;

typedef struct {

    ullong value;       // LLONG_MIN if not magic
    ullong start;
    ullong size;
    ullong *sum_cols;
    ullong lrd_sum;
    ullong rld_sum;
} MagicSquare;

MagicSquare *msq;

MagicSquare magicSquare(int size, int start){
    MagicSquare result;
    result.start = start;
    result.size = size;
    result.sum_cols = calloc(order,sizeof(ullong));
    result.lrd_sum = result.rld_sum = result.value = 0;
    return result;
}

long parse_order(char *path){
    int path_size = strlen(path)-4;//i.e lets work backwards 
    int digits_size=0;
    for(int i = path_size-1; i >= 0; i--){
        char c = path[i];
        if(isdigit(c))
            digits_size++;
        else return strtol(path+(path_size-digits_size),&path,10);
    }
    return -1;
}

void long_readfile(FILE *f, ullong *a)
{
    for (int i = 0; i < (order*order); i++)
        fscanf(f, "%lld", &a[i]);
}


void print_magic_square(MagicSquare ms){
    printf("ms.value = %lld ms.size = %lld ms.start = %lld thread id: %d\n",ms.value,ms.size,ms.start, omp_get_thread_num());
}

ullong *sum_all(MagicSquare *ms, int size, ullong *sum_cols){
    ullong *result = calloc(2,sizeof(ullong));
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < order; j++)
            sum_cols[j] += ms[i].sum_cols[j];
        result[0] += ms[i].lrd_sum;
        result[1] += ms[i].rld_sum;
        
    }
    return result;
}

int calc(FILE *fp){

    
	int interval= (double)order/OMP_NUM_THREADS;
    int THREAD_NUM = OMP_NUM_THREADS;
    if(order < OMP_NUM_THREADS) {
        THREAD_NUM = order;
        omp_set_num_threads(order);
    } else { 
        omp_set_num_threads(OMP_NUM_THREADS);
    }

    data = calloc(pow(order,2),sizeof(ullong));
    long_readfile(fp,data);
    MagicSquare *msq = calloc(THREAD_NUM,sizeof(MagicSquare));
    int lines_in_thread = interval;
    long start=0, size=lines_in_thread*order;

    for (int i = 0; i < THREAD_NUM; i++) {
        if(i is THREAD_NUM-1) {
            lines_in_thread = order-(lines_in_thread*i);
            size = lines_in_thread*order;
        }
        msq[i] = magicSquare(size,start);
        start += size;
    }

    
    //shared
    
        #pragma omp parallel
        {
            MagicSquare *ms = &msq[omp_get_thread_num()];
            if(is_magic_quare is True){
                ullong sum_line = 0,value = 0, constant = 0;
                for (int j = ms->start; j < ms->size+ms->start; j++) {
                    if(is_magic_quare is True) {
                        value = data[j];
                        int row = j/order;
                        int column = j%order;
                        ms->sum_cols[column]+=value;
                        sum_line+=value;
                        if(row is ms->start/order and column is order-1) constant = sum_line;
                        if(sum_line is constant and column is order-1) sum_line = 0;
                        else if (column is order-1) is_magic_quare = False;
                        if(row is column) ms->lrd_sum += value;
                        if(column is (order-1-row)) ms->rld_sum += value;
                        
                    }
                }
                
                ms->value = constant;
                
            }

        }
    ullong *sum_cols = (ullong *)calloc(order,sizeof(ullong));
    ullong *result = sum_all(msq,THREAD_NUM,sum_cols);
    ullong lrd_sum = result[0], rld_sum = result[1]; 
    ullong constant = msq[0].value;
    for (int i = 0; i < THREAD_NUM; i++)
        if(msq[i].value isnot constant or sum_cols[i] isnot constant) 
            return -1;
        

    return (1*(rld_sum is lrd_sum and rld_sum is constant)) + 0;
    
}


void execute_threaded(char *filepath){
    order = parse_order(filepath);
    FILE *fp = fopen(filepath,"r");
    
    int res = calc(fp);
    switch (res)
    {
    case 1:
        printf("Quadrado magico\n");
        break;
    case 0:
        printf("Quadrado imperfeito\n");
        break;
    default:
        printf("Quadrado nao magico\n");
        break;
    }

}

char *double_to_floating_point_string_custom_separator(double d, char separator, char new_separator){
    char *string = malloc(256*sizeof(char));
    sprintf(string,"%lf",d);
    for(int i = 0; i < strlen(string); i++){
        if(string[i] is separator)
            string[i] = new_separator;
    }
    return string;
}

int main(int argc, char *argv[]){
    char file_path[256];
    if(argc > 1){
        strcpy(file_path,argv[1]);
    } else {
        printf("Nao introduzio argumento, indique o caminho da input");
        return 1;
    }
    execute_threaded(file_path);
    return 1;
}