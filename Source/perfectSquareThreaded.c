#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <limits.h>
#include <ctype.h>

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

#define elif else if
#define llong long long
#define ullong unsigned long long
#define THREAD_NUM 16

short square_type = 1;  //1 means perfect, 0 means imperfect -1 means no magic square
ullong *sum_cols;
ullong lrd_sum = 0;
ullong rld_sum = 0;
long order = 0;


typedef struct {

    ullong *linesData;
    ullong value;       // LLONG_MIN if not magic
    ullong start;
    ullong size;

} MagicSquare;

MagicSquare magicSquare(ullong *array, size_t array_size, size_t start){
    MagicSquare result;
    result.start = start;
    result.size = array_size;
    result.linesData = (ullong*)malloc(result.size*sizeof(ullong));
    memcpy(result.linesData,array,sizeof(ullong)*result.size);
    result.value = 0;
    return result;
}

long parse_order(char *path){
    size_t path_size = strlen(path)-4;//i.e lets work backwards 
    size_t digits_size=0;
    for(size_t i = path_size-1; i >= 0; i--){
        char c = path[i];
        if(isdigit(c))
            digits_size++;
        else return strtol(path+(path_size-digits_size),&path,10);
    }
    return -1;
}

void long_readfile(FILE *f, ullong *a, int end)
{
    for (size_t i = 0; i < end; i++)
        fscanf(f, "%lld", &a[i]);
}

void *thread_fuction(void *args){
    // printf("thread init: %d\n",pthread_self());
    MagicSquare *msq = ( MagicSquare *) args;
    int row = msq->start/order;
    int lrd_ptr = row;
    int rld_ptr = order-row-1;
    ullong value = 0, sum_line = 0, constant=0;

    for (size_t i = 0; i < msq->size; i++) {
        if(square_type is -1 ) { msq->value = LLONG_MIN; pthread_exit(NULL);}
        value = msq->linesData[i];
        sum_line+=value;
        sum_cols[i%order] += value;
        if(i is lrd_ptr){
            lrd_sum+=value;
            lrd_ptr+=(order+1);
        }
        if(i is rld_ptr and rld_ptr isnot msq->size-1) {
            rld_sum+=value;
            rld_ptr+=(order-1);
        }
        // printf("sum_line: %lld\n",sum_line);
        if(i is order-1) { /*printf("thread: %d sum_line: %lld\n",pthread_self(),sum_line);*/ constant = sum_line;}
        if( (i+1)%order is 0) { 
            // printf("line end\n");
            if(sum_line is constant) { sum_line = 0;}
            else { 
                // printf("exit %lld\n",constant);
                square_type = -1; 
                msq->value = LLONG_MIN;
                free(msq->linesData);
                pthread_exit(NULL);
            }
        }
    }
    free(msq->linesData);
    msq->value = constant;
    pthread_exit(NULL);
    return 0;
}

int thread_number(){
    if(THREAD_NUM > order){
        return order;
    } else return THREAD_NUM;
}
int calc(FILE *fp){

    int line_per_thread = ceil( (double)order/THREAD_NUM );
    MagicSquare *msq = calloc(THREAD_NUM,sizeof(MagicSquare));
    pthread_t *thread_id = calloc(THREAD_NUM, sizeof(pthread_t));

    ullong *linesData = malloc(order*line_per_thread*sizeof(ullong));

    int struct_index = 0, thread_count = 0;
    size_t size=0, start=0;

    for(size_t i = 0; i < order; i++){
        
        if( (((i+1)%(line_per_thread) is 0 and i isnot 0) or i is order-1 )and square_type != -1){
            long_readfile(fp,linesData,line_per_thread*order);
            size = ((i+1)-(line_per_thread*struct_index))*order;
            msq[struct_index] = magicSquare(linesData,size,start);
            pthread_create(&thread_id[thread_count++],NULL,thread_fuction,&msq[struct_index++]);
            start=(i+1)*order;
        }

    }
    for (size_t i = 0; i < struct_index; i++){
        // printf("waiting for... %d\n",thread_id[i]);
        pthread_join(thread_id[i],NULL);
    }


    ullong constant = 0;
    for (size_t i = 0; i < struct_index; i++) {
        // printf("msq[%lld].value = %lld .start %d\n", i, msq[i].value, msq[i].start);
        if(i is 0) { constant = msq[i].value; }
        if(msq[i].value isnot constant) { return -1; }
    }
    
    for (size_t i = 0; i < order; i++){
        if(sum_cols[i] isnot constant) {
            
            return -1;
        }
    }
    if(lrd_sum isnot rld_sum)
        return 0;
    return 1;

}

void execute_threaded(char *filepath){
    order = parse_order(filepath);
    FILE *fp = fopen(filepath,"r");
    sum_cols = calloc(order,sizeof(ullong));
    int res = calc(fp);
    if(res is 1) 
       printf("Quadrado magico\n");
    elif (res is 0) 
        printf("Quadrado imperfeito\n");
    else printf("Quadrado nao magico\n");

}

char *double_to_floating_point_string_custom_separator(double d, char separator, char new_separator){
    char *string = malloc(50*sizeof(char));
    sprintf(string,"%lf",d);
    for(size_t i = 0; i < strlen(string); i++){
        if(string[i] is separator)
            string[i] = new_separator;
    }
    return string;
}

//time src https://www.cplusplus.com/reference/ctime/localtime/
int main(int argc, char *argv[]){
    char file_path[300];
    if(argc > 1){
        strcpy(file_path,argv[1]);
    } else {
        printf("Nao introduzio argumento, indique o caminho da input");
        return 1;
    }
    execute_threaded(file_path);
    return 1;
}