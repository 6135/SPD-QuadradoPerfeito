#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>
#include <time.h>

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
typedef struct {
    int **array;
    int array_size;
    int side_size;
    long long cols_sum;
    long long lines_sum;
    long long diag_sum;
} magicSquare;

// void array_init(void* destination,size_t size,size_t size_of_type){
//     destination = (void *)malloc(size*sizeof(size*size_of_type));
// }

// void array_copy(void *destination, void *source, size_t size_of_type, size_t size){
//     printf("test");
//     array_init(destination,size,size_of_type);
//     memcpy(destination,source,size_of_type*size);

// }

// void bi_dem_array_init(void** destination,size_t size_of_type,size_t size){

//     destination = (void **)malloc(size*sizeof(void *));
//     for (size_t i = 0; i < size; i++)
//         array_init(destination[i],size,size_of_type);
// }

// void bi_dem_array_copy(void **destination, void **source, size_t size_of_type, size_t size){
//     bi_dem_array_init(destination,size,size_of_type);
//     // printf("test1 %g",size,size_of_type);
//     for (int i = 0; i < size; i++)
//         array_copy(destination[i],source[i],size_of_type,size);
    
// }



magicSquare MagicSquare(int **array, int array_size){
    int sqrN = sqrt(array_size);
    magicSquare result;
    result.cols_sum = 0;
    result.lines_sum = 0;
    result.diag_sum = 0;
    result.side_size = sqrN;
    result.array_size = array_size;
    
    result.array = (int**)malloc(sqrN*sizeof(int*));
    for (int i = 0; i < sqrN; i++)
        result.array[i] =(int*) malloc(sqrN*sizeof(int));
        
    for (size_t i = 0; i < sqrN; i++)
        memcpy(result.array[i],array[i],sizeof(int)*sqrN);
    // bi_dem_array_copy( ((void**) result.array),(void**)array,sizeof(int),sqrN);
    
    return result;

}

int array_size_count(FILE* fp){
    int n = 0;
    int temp;
    while(fscanf(fp,"%d",&temp) isnot EOF)
        n++;
    return n;
}
void array_fill(FILE* fp,const int n,const int sqrN, int **array){
    for (int i = 0; i < sqrN; i++)
        for (int j = 0; j < sqrN; j++)
            fscanf(fp,"%d",&array[i][j]);
}

long long sum_line(int *array ,int size){
    long long sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i];
    return sum;
}

long long sum_column(int j,int **array ,int size){
    long long sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i][j];
    return sum;
}

long long sum_diag(int i, int j, int **array, int size){
    long long sum = 0;
    if(i is 0 and j is (size-1))
        for(int k = 0; k < size; k++) sum+= array[i+k][j-k];
    else if (i is j and i is 0)
        for(int k = 0; k < size; k++) sum+= array[i+k][j+k];

    return sum;
    
}

long long check_all_lines_equals(int **array, int size){
    long long sum_first_line = sum_line(array[0],size);
    for (int i = 1; i < size; i++){ if( sum_line(array[i],size) isnot sum_first_line ) return LLONG_MIN; }
    return sum_first_line;
}

long long check_all_columns_equals(int **array, int size){
    long long sum_first_column = sum_column(0,array,size);
    for (int j = 1; j < size; j++) if(sum_column(j, array,size) isnot sum_first_column ) return LLONG_MIN;
    return sum_first_column;
}

long long check_all_diags_equals(int **array, int size){
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

void perfect_square_sequential(const char* file_path){ 
    /**
     * start counting time 
     */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //*******************//
    printf("Sequential\n");
    FILE *fp;
    fp = fopen(file_path,"r");
    int size_array = array_size_count(fp); //count size
    // printf("Number of elements: %d\n", size_array);

    int sqrN = sqrt(size_array);
    int **array = (int **)malloc(sqrN * sizeof(int *)); 
    for (int i=0; i<sqrN; i++) 
         array[i] = (int *)malloc(sqrN * sizeof(int)); 

    // printf("File read\n");
    fp = fopen(file_path,"r");
    array_fill(fp,size_array,sqrN,array);
    magicSquare magic_square = MagicSquare(array,size_array);

    //line 1
    // printf("Checking rules...\n");
    long long check_all_lines = check_all_lines_equals(magic_square.array,sqrN);
    printf("Line Sum: %lld\n",check_all_lines);
    long long check_all_columns = check_all_columns_equals(magic_square.array,sqrN);
    printf("Column Sum: %lld\n",check_all_columns);
    long long check_all_diags = check_all_diags_equals(magic_square.array,sqrN);
    printf("Diag Sum: %lld\n",check_all_diags);
    int rules[3] = {check_all_lines, check_all_columns,check_all_diags};
    
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
    printf("Took %f seconds to execute \n", cpu_time_used); 
    //close
    fclose(fp);
    for(int i = 0; i < sqrN; i++)
        free(array[i]);
    free(array);

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
}

void* check_all_diags_equals_threaded(void* param){
    magicSquare *magic_square = (magicSquare *)param;
    long long diag_right_left = sum_diag(0,magic_square->side_size-1,magic_square->array,magic_square->side_size);
    long long diag_left_right = sum_diag(0,0,magic_square->array,magic_square->side_size);
    if(diag_left_right is diag_right_left) {
        magic_square->diag_sum = diag_left_right;
    }
    else  magic_square->diag_sum = LLONG_MIN;
    pthread_exit(NULL);
}

void *perfect_square_threaded(const char *filep){

    /**
     * start counting time 
     */
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //****************//
    // char *filep = (char *)file_path;
    printf("Threaded\n");
    FILE *fp;
    fp = fopen(filep,"r");
    int size_array = array_size_count(fp); //count size

    int sqrN = sqrt(size_array);
    // int **array; bi_dem_array_init((void**)array,sizeof(int),sqrN);
    int **array = (int **)malloc(sqrN * sizeof(int *)); 
    for (int i=0; i<sqrN; i++) 
         array[i] = (int *)malloc(sqrN * sizeof(int)); 

    fp = fopen(filep,"r");
    
    array_fill(fp,size_array,sqrN,array);
    
    magicSquare magic_square = MagicSquare(array,size_array);
    
    pthread_t tid[3];
    int thread_num = 0;

    // printf("testStruct: %d\n",magic_square.array[86][99]);
    pthread_create(&tid[thread_num++],NULL,check_all_lines_equals_threaded,(void*)&magic_square);
    pthread_create(&tid[thread_num++],NULL,check_all_columns_equals_threaded,(void*)&magic_square);
    pthread_create(&tid[thread_num++],NULL,check_all_diags_equals_threaded,(void *)&magic_square);
    
    for(int i = 0; i < thread_num; i++){
        pthread_join(tid[i],NULL);
        // printf("threadNu: %d\n", i);
    }
        
    printf("Line Sum: %lld\n",magic_square.lines_sum);
    printf("Column Sum: %lld\n",magic_square.cols_sum);
    printf("Diag Sum: %lld\n",magic_square.diag_sum);
    int rules[3] = {magic_square.lines_sum, magic_square.cols_sum,magic_square.diag_sum};
    
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
    printf("Took %f seconds to execute \n", cpu_time_used); 
    // //close
    fclose(fp);
    for(int i = 0; i < sqrN; i++) free(array[i]);
    free(array);

}
int main(){
    
    perfect_square_threaded("input8.txt");
    perfect_square_sequential("input8.txt");
}