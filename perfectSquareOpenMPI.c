#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <limits.h>
#include <ctype.h>
#include <mpi.h>
#include <unistd.h>
#include <omp.h>    
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
#define MPI_ULLONG MPI_UNSIGNED_LONG_LONG
#define GREATER(X,Y) X*(X>=Y)+Y*(X<Y)

#define PROCC_SKIP -6135
#define structlen 6
int OMP_NUM_THREADS = 1;
short is_magic_quare = True;  //1 means perfect, 0 means imperfect -1 means no magic square
// ullong *sum_cols;
// ullong rld_sum=0, lrd_sum=0;
// ullong order = 0;
// const int OMP_NUM_THREADS = 8;

// ullong *data;
ullong order;

typedef struct {
    ullong order, value, start, size, lrd_sum, rld_sum;
} MagicSquare;

MagicSquare magicSquare(size_t size, size_t start){
    MagicSquare result;
    result.start = start;
    result.size = size;
    result.order = order;
    result.lrd_sum = 0;
    result.rld_sum = 0;
    result.value = 0;
    return result;
}

int blocklens[structlen];
MPI_Datatype types[structlen] = {MPI_ULLONG,MPI_ULLONG,MPI_ULLONG,MPI_ULLONG,MPI_ULLONG,MPI_ULLONG};
MPI_Aint offsets[structlen];
MPI_Datatype MPI_MAGICSQUARE;

void init_struct_type_magicsquare(){

    blocklens[0] = blocklens[1] = blocklens[2] = blocklens[3] = blocklens[4] = blocklens[5] = 1;
    offsets[0] = offsetof(MagicSquare, order);
    offsets[1] = offsetof(MagicSquare, value);
    offsets[2] = offsetof(MagicSquare, start);
    offsets[3] = offsetof(MagicSquare, size);
    offsets[4] = offsetof(MagicSquare, lrd_sum);
    offsets[5] = offsetof(MagicSquare, rld_sum);
   
    MPI_Type_create_struct(structlen, blocklens, offsets, types, &MPI_MAGICSQUARE);
    MPI_Type_commit(&MPI_MAGICSQUARE);
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

void long_readfile(FILE *f, ullong *a)
{
    for (size_t i = 0; i < (order*order); i++)
        fscanf(f, "%lld", &a[i]);
}

void long_readfile_until(FILE *f, ullong *a, ullong size)
{
    for (size_t i = 0; i < size; i++)
        fscanf(f, "%lld", &a[i]);
    
}
ullong column_sum_check(ullong *sum_columns, int size){
    ullong col_sum = sum_columns[0];
    for (size_t i = 1; i < size; i++) {
        // printf("cols: %lld\n",sum_columns[i]);
        col_sum += -col_sum*(sum_columns[i] isnot col_sum);
    }
    
    return col_sum;
}
ullong values_check(MagicSquare *msq, int count){
    ullong value = msq[0].value;
    for (int i = 0; i < count; i++)
        value += -value*(msq[i].value isnot value);
        
    return value;
    
}

ullong lrd_check(MagicSquare *msq, int count){

}

void operate(ullong *sum_cols, ullong *data, MagicSquare *msq){
    order = msq->order;
    ullong small_order = (msq->size)/msq->order;
    // printf("small order: %lld\n",small_order);
    int interval= floor( (double)small_order /OMP_NUM_THREADS);
    int THREAD_NUM = OMP_NUM_THREADS;
    if(small_order < OMP_NUM_THREADS) {
        THREAD_NUM = small_order;
        omp_set_num_threads(small_order);
    } else { 
        omp_set_num_threads(OMP_NUM_THREADS);
    }

    MagicSquare *msq_omp = malloc(THREAD_NUM*sizeof(MagicSquare));
    int lines_in_thread = interval;
    long start=0, size=lines_in_thread*msq->order;
    for (size_t i = 0; i < THREAD_NUM; i++) {
        if(i is THREAD_NUM-1) {
            lines_in_thread = small_order-(lines_in_thread*i);
            size = lines_in_thread*msq->order;
        }
        // printf("msq_omp[%d]: size: %lld start %lld\n",i,size,start);
        msq_omp[i] = magicSquare(size,start);
        start += size;
    }

    ullong lrd_sum, rld_sum; 
    //private
    ullong value;
    int row, column, row_offset = msq->start/msq->order;


    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    #pragma omp parallel for shared(lrd_sum,rld_sum,msq_omp,msq,sum_cols) private(row,column)
        for (size_t i = 0; i < THREAD_NUM; i++) {
            if(is_magic_quare is True){
                MagicSquare *ms = &msq_omp[i];
                ullong sum_line = 0,value = 0, constant = 0;
                for (size_t j = ms->start; j < ms->size+ms->start; j++) {
                    
                    if(is_magic_quare is True) {
                        value = data[j];
                        row = j/msq->order;
                        column = j%msq->order;
                        sum_line+=value;
                        if(row is ms->start/msq->order and column is msq->order-1) constant = sum_line;
                        if(sum_line is constant and column is msq->order-1) sum_line = 0;
                        else if (column is msq->order-1) { is_magic_quare = False; constant = PROCC_SKIP;}
                        if((row+row_offset) is column) msq->lrd_sum += value;
                        if(column is (msq->order-1-(row+row_offset))) msq->rld_sum += value;
                        sum_cols[column]+=value;
                    }
                    
                }
                
                ms->value = constant;
            }
        }

    ullong result = values_check(msq_omp,THREAD_NUM);
    msq->value = result*(result gt 0) + PROCC_SKIP*(result is 0);
}



int masterMachine(FILE *fp){
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int interval= floor( (double)order /world_size);
    int PROC_NUM = world_size*(order>world_size) + order*(order<=world_size); //branchless condition

    MagicSquare *msq = malloc(PROC_NUM*sizeof(MagicSquare));

    int lines_per_proc = interval;
    long start=0, size=lines_per_proc*order;

    for (size_t i = 0; i < PROC_NUM; i++) {
        if(i is PROC_NUM-1) {
            lines_per_proc = order-(lines_per_proc*i);
            size = lines_per_proc*order;
        }
        msq[i] = magicSquare(size,start);
        start += size;
    }

    ullong *data = malloc( GREATER(msq[0].size,msq[1].size) *sizeof(ullong)) ;
    MPI_Send(&msq[0],1,MPI_MAGICSQUARE,1,0,MPI_COMM_WORLD);
    long_readfile_until(fp,data,msq[0].size);
    MPI_Send(data,msq[0].size,MPI_ULLONG,1,1,MPI_COMM_WORLD);

    long_readfile_until(fp,data,msq[1].size);
    ullong *sum_cols_local = calloc(sizeof(ullong),order);
    operate(sum_cols_local,data,&msq[1]);
    free(data);

    /* gather data from slave */
    MPI_Recv(&msq[0],1,MPI_MAGICSQUARE,1,2,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    ullong *sum_cols_slave = calloc(sizeof(ullong),order);
    MPI_Recv(sum_cols_slave,order,MPI_ULLONG,1,3,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

    /* sum remainders */
    for (size_t i = 0; i < order; i++)
        sum_cols_local[i]+=sum_cols_slave[i];
    
    ullong rld_sum = msq[0].rld_sum + msq[1].rld_sum;
    ullong lrd_sum = msq[0].lrd_sum + msq[1].lrd_sum;

    ullong lines = msq[0].value*(msq[0].value is msq[1].value) + PROCC_SKIP*(msq[0].value isnot msq[1].value);
    
    ullong columns = column_sum_check(sum_cols_local,order);

    return 1*(lines is columns and rld_sum is lrd_sum and rld_sum is lines) +
           1*(lines is columns) +
           0;

}


int slaveMachine() {
    MagicSquare msq;
    MPI_Recv(&msq,1,MPI_MAGICSQUARE,0,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    ullong *data = malloc(sizeof(ullong)*msq.size);
    MPI_Recv(data,msq.size,MPI_ULLONG,0,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
    ullong *sum_cols = calloc(sizeof(ullong),msq.order);
    operate(sum_cols,data,&msq);
    free(data);
    /**Send data back to master**/
    MPI_Send(&msq,1,MPI_MAGICSQUARE,0,2,MPI_COMM_WORLD);
    MPI_Send(sum_cols,msq.order,MPI_ULLONG,0,3,MPI_COMM_WORLD);
}

void execute_MPI(char *filepath){
    order = parse_order(filepath);
    FILE *fp = fopen(filepath,"r");
    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int res = 0;
    if(world_rank is 0){
        FILE *fp = fopen(filepath,"r");
        res = masterMachine(fp);
        switch (res)
        {
        case 2:
            printf("Quadrado magico\n");
            break;
        case 1:
            printf("Quadrado imperfeito\n");
            break;
        case 0:
            printf("Quadrado nao magico\n");
            break;
        }
    } elif(world_rank > 0)
        slaveMachine();

}

char *double_to_floating_point_string_custom_separator(double d, char separator, char new_separator){
    char *string = malloc(256*sizeof(char));
    sprintf(string,"%lf",d);
    for(size_t i = 0; i < strlen(string); i++){
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

    MPI_Init(NULL, NULL);

    init_struct_type_magicsquare();
    execute_MPI(file_path);
    MPI_Finalize();
    return 0;
}

