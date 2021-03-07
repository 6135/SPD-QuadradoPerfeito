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
#define MAX_ARRAY_SIDE 100000
typedef struct {
    long long **array;
    int array_size;
    int side_size;
    long long cols_sum;
    long long lines_sum;
    long long diag_sum;
} magicSquare;


magicSquare MagicSquare(long long **array, int array_size){
    magicSquare result;
    result.cols_sum = 0;
    result.lines_sum = 0;
    result.diag_sum = 0;
    result.side_size = sqrt(array_size);
    result.array_size = array_size;
    
    result.array = (long long**)malloc(result.side_size*sizeof(long long*));
    for (int i = 0; i < result.side_size; i++)
        result.array[i] =(long long*) malloc(result.side_size*sizeof(long long));
        
    for (size_t i = 0; i < result.side_size; i++)
        memcpy(result.array[i],array[i],sizeof(long long)*result.side_size);
    
    return result;

}

int data_size_count(FILE* fp){
    long long n = 0;
    long long temp;
    while(fscanf(fp,"%lld",&temp) isnot EOF){
        n++;
    }
    return n;
}

void fetch_data_to_array(FILE* fp,const int n,const int sqrN, long long **array){
    for (int i = 0; i < sqrN; i++)
        for (int j = 0; j < sqrN; j++){
            fscanf(fp,"%lld",&array[i][j]);
            }
}

magicSquare fetch_data(const char *filepath) {
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    FILE *fp = fopen(filepath,"r");

    long long size_array = data_size_count(fp); //count size
    long long sqrN = sqrtl(size_array);
    printf("size: %lld, squareRoot: %lld\n",size_array, sqrN);
    // int **array; bi_dem_array_init((void**)array,sizeof(int),sqrN);
    long long **array = (long long **)malloc(sqrN * sizeof(long long *)); 
    for (int i=0; i<sqrN; i++) 
         array[i] = (long long *)malloc(sqrN * sizeof(long long)); 
    // printf("Size counted\n");
    fp = fopen(filepath,"r");    
    fetch_data_to_array(fp,size_array,sqrN,array);
    // printf("Fill Array\n");
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Reading data took %lf seconds to execute \n", cpu_time_used); 

    return MagicSquare(array,size_array);
}


long long sum_line(long long *array ,size_t size){
    long long sum = 0;
    // FILE *fp = fopen("testSum.txt","w+");
    for (size_t i = 0; i < size; i++) {
        sum+= array[i];
        // fprintf(fp,"%lld +",array[i]);
    }
    return sum;
}

long long sum_column(int j,long long **array ,int size){
    long long sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i][j];
    return sum;
}

long long sum_diag(int i, int j, long long **array, int size){
    long long sum = 0;
    if(i is 0 and j is (size-1))
        for(int k = 0; k < size; k++) sum+= array[i+k][j-k];
    else if (i is j and i is 0)
        for(int k = 0; k < size; k++) sum+= array[i+k][j+k];

    return sum;
    
}

long long check_all_lines_equals(long long **array, int size){
    long long sum_first_line = sum_line(array[0],size);
    for (int i = 1; i < size; i++){ if( sum_line(array[i],size) isnot sum_first_line ) return LLONG_MIN; }
    return sum_first_line;
}

long long check_all_columns_equals(long long **array, int size){
    long long sum_first_column = sum_column(0,array,size);
    for (int j = 1; j < size; j++) if(sum_column(j, array,size) isnot sum_first_column ) return LLONG_MIN;
    return sum_first_column;
}

long long check_all_diags_equals(long long **array, int size){
    long long diag_right_left = sum_diag(0,size-1,array,size);
    long long diag_left_right = sum_diag(0,0,array,size);
    if(diag_left_right is diag_right_left)
        return diag_left_right;
    else return LLONG_MIN;
}

int any_equals(int* args, int size,long long exception){
    int num_matches = 0;
    for (int i = 0; i < size; i++) {
        int arg = args[i];
        for (int j = i; j < size; j++){
            if(i isnot j and arg isnot exception and args[j] is arg)
                num_matches++;
        }
    }
    return num_matches;
    
}

void perfect_square_sequential(magicSquare *data){ 
    /**
     * start counting time 
     */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //*******************//
    printf("Sequential\n");
    //line 1
    // printf("Checking rules...\n");
    long long check_all_lines = check_all_lines_equals(data->array,data->side_size);
    long long check_all_columns = check_all_columns_equals(data->array,data->side_size);
    long long check_all_diags = check_all_diags_equals(data->array,data->side_size);
    
    int rules[3] = {check_all_lines, check_all_columns,check_all_diags};
    
    int matches=any_equals(rules,3,LLONG_MIN);
    printf("Line Sum: %lld\n",check_all_lines);
    printf("Column Sum: %lld\n",check_all_columns);
    printf("Diag Sum: %lld\n",check_all_diags);
    if(matches is 3)
        printf("Quadrado magico\n");
    else if(matches is 1)
        printf("Quadrado magico imperfeito\n");
    else if(matches is 0) printf("Nao e quadrado magico\n");
    /**
     * Stop counting
     */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Took %lf seconds to execute \n", cpu_time_used); 
    //close

}

void* check_all_lines_equals_threaded(void* param){
    magicSquare *magic_square = (magicSquare *) param;
    long long sum_first_line = sum_line(magic_square->array[0],magic_square->side_size);
    for (int i = 1; i < magic_square->side_size; i++){
        if( (sum_line(magic_square->array[i],magic_square->side_size)) isnot sum_first_line ) {
            magic_square->lines_sum = LLONG_MIN;
            pthread_exit(NULL);
        }
    }
    magic_square->lines_sum = sum_first_line;
    pthread_exit(NULL);
    return NULL;
}

void* check_all_columns_equals_threaded(void* param){
    magicSquare *magic_square = (magicSquare *)param;
    long long sum_first_column = sum_column(0,magic_square->array,magic_square->side_size);
    for (int j = 1; j < magic_square->side_size; j++) 
        if(sum_column(j, magic_square->array,magic_square->side_size) isnot sum_first_column ) {
            magic_square->cols_sum = LLONG_MIN;
            pthread_exit(NULL);
        }
    magic_square->cols_sum = sum_first_column;
    pthread_exit(NULL);
    return NULL;
}

void* check_all_diags_equals_threaded(void* param){
    magicSquare *magic_square = (magicSquare *)param;
    long long diag_right_left = sum_diag(0,magic_square->side_size-1,magic_square->array,magic_square->side_size);
    long long diag_left_right = sum_diag(0,0,magic_square->array,magic_square->side_size);
    if(diag_left_right is diag_right_left) {
        magic_square->diag_sum = diag_left_right;
    }
    else  magic_square->diag_sum = LLONG_MIN;
    //pthread_exit(NULL);
    return NULL;
}

void *perfect_square_threaded(magicSquare *data){

    /**
     * start counting time 
     */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //****************//
    printf("Threaded\n");
    
    pthread_t tid[3];
    int thread_num = 0;

    // printf("testStruct: %d\n",data->array[86][99]);
    pthread_create(&tid[thread_num++],NULL,check_all_lines_equals_threaded,(void*)data);
    pthread_create(&tid[thread_num++],NULL,check_all_columns_equals_threaded,(void*)data);
    check_all_diags_equals(data->array,data->side_size);
    
    for(int i = 0; i < thread_num; i++)
        pthread_join(tid[i],NULL);
   
    printf("Line Sum: %lld\n",data->lines_sum);
    printf("Column Sum: %lld\n",data->cols_sum);
    printf("Diag Sum: %lld\n",data->diag_sum);

    int rules[3] = {data->lines_sum, data->cols_sum,data->diag_sum};
    int matches=any_equals(rules,3,LLONG_MIN);
    
    if(matches is 3)
        printf("Quadrado magico\n");
    else if(matches is 1)
        printf("Quadrado magico imperfeito\n");
    else if(matches is 0) printf("Nao e quadrado magico\n");
    /**
     * Stop counting
     */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Took %lf seconds to execute \n", cpu_time_used); 
    return NULL;
}

int main(){
    magicSquare data = fetch_data("inputs/input5000x5000.txt");
    
    // printf("lineSum with %d sides",data.side_size);
    // printf("%lld",sum_line(data.array[0],data.side_size));
    perfect_square_sequential(&data);
    perfect_square_threaded(&data);
    return 0;
}