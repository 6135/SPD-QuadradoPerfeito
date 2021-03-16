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
#include <windows.h>


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

#define llong unsigned long long
#define MAX_ARRAY_SIDE 100000
#define THREAD_COUNT 8 //This may be doubled up according to strategy

int is_valid = 1;

typedef struct {
    llong **array;
    int array_size;
    int side_size;
    llong cols_sum;
    llong lines_sum;
    llong diag_sum;
} magicSquare;

typedef struct {
    magicSquare *magic_square;
    int from, to,splitNum;
    llong lines_sum, cols_sum;
} magicSquareSplit;

void array_copy(llong* destination, llong *source, int size ){
    destination = (llong *)malloc(size*sizeof(llong));
    memcpy(destination,source,sizeof(llong)*size);
}
bool isEven(llong number){
    return number%2;
}

void result_out(magicSquareSplit*data, size_t size){
    printf("From: ");
    for (size_t i = 0; i < size; i++)
        printf("%d ",data[i].from);
    printf("\n");
    printf("  To: ");
    for (size_t i = 0; i < size; i++) 
        printf("%d ",data[i].to);
    printf("\n");
    printf("line: ");
    for (size_t i = 0; i < size; i++) 
        printf("%lld ",data[i].lines_sum);
    printf("\n");
    printf("Colm: ");
    for (size_t i = 0; i < size; i++) 
        printf("%lld ",data[i].cols_sum);    
    printf("\n");
}

magicSquare MagicSquare(llong **array, int array_size){
    magicSquare result;
    result.array_size = array_size;
    result.side_size = sqrt(array_size);

    result.cols_sum = 0;
    result.lines_sum = 0;
    result.diag_sum = 0;
    
    result.array = (llong**)malloc(result.side_size*sizeof(llong*));
    for (int i = 0; i < result.side_size; i++)
        result.array[i] =(llong*) malloc(result.side_size*sizeof(llong));
        
    for (size_t i = 0; i < result.side_size; i++)
        memcpy(result.array[i],array[i],sizeof(llong)*result.side_size);
    
    return result;

}
int round_up(double val){
    return val gt ((int)val) ? (val+1) : val;
}

magicSquareSplit *MagicSquareSplit(magicSquare *data){
    int splitNum = THREAD_COUNT;
    int splitSize = THREAD_COUNT;
    if(THREAD_COUNT gt data->side_size){ //then instead we split into 'data->side_size' intervals
        splitNum = data->side_size;
        splitSize = 1;
    } else splitSize = round_up((double)data->side_size / (double)THREAD_COUNT);

    //now we create the ranges for the threads
    magicSquareSplit *result = (magicSquareSplit *)malloc(splitNum*sizeof(magicSquareSplit));
    /**
     *  from -> 0 1 2 3 4
     *    to -> 1 2 3 4 5(x)
     */

    for (size_t i = 0; i < splitNum; i++){
        result[i].cols_sum=0;
        result[i].lines_sum=0;
        result[i].magic_square = data;
        result[i].from = (i*splitSize);
        if((i+1)*splitSize gt data->side_size)
            result[i].to = data->side_size;
        else 
            result[i].to = ((i+1)*splitSize);
        result[i].splitNum = splitNum;
    }
    
    // printf("Thread Count to use: %d\n",splitNum*2);
    return result;
    
}

int data_size_count(FILE* fp){
    llong n = 0;
    llong temp;
    while(fscanf(fp,"%lld",&temp) isnot EOF){
        n++;
    }
    return n;
}

void fetch_data_to_array(FILE* fp,const int n,const int sqrN, llong **array){
    for (int i = 0; i < sqrN; i++)
        for (int j = 0; j < sqrN; j++){
            fscanf(fp,"%lld",&array[i][j]);
            }
}

magicSquare fetch_data(const char *filepath) {
    FILE *fp = fopen(filepath,"r");
    // printf("here");
    llong size_array = data_size_count(fp); //count size
    llong sqrN = sqrtl(size_array);
    // printf("size: %lld, squareRoot: %lld\n",size_array, sqrN);
    // int **array; bi_dem_array_init((void**)array,sizeof(int),sqrN);
    llong **array = (llong **)malloc(sqrN * sizeof(llong *)); 
    for (int i=0; i<sqrN; i++) 
         array[i] = (llong *)malloc(sqrN * sizeof(llong)); 
    // printf("Size counted\n");
    fp = fopen(filepath,"r");    
    fetch_data_to_array(fp,size_array,sqrN,array);
    // printf("Fill Array\n");


    return MagicSquare(array,size_array);
}


llong sum_line(llong *array ,size_t size){
    llong sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum+= array[i];
    }
    return sum;
}

llong sum_column(int j,llong **array ,int size){
    llong sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i][j];
    return sum;
}

llong sum_diag(int i, int j, llong **array, int size){
    llong sum = 0;
    if(i is 0 and j is (size-1))
        for(int k = 0; k < size; k++) sum+= array[i+k][j-k];
    else if (i is j and i is 0)
        for(int k = 0; k < size; k++) sum+= array[i+k][j+k];

    return sum;
    
}

llong check_all_lines_equals(llong **array, int size){
    llong sum_first_line = sum_line(array[0],size);
    for (int i = 1; i < size; i++){ if( sum_line(array[i],size) isnot sum_first_line ) return LLONG_MIN; }
    return sum_first_line;
}

llong check_all_columns_equals(llong **array, int size){
    llong sum_first_column = sum_column(0,array,size);
    for (int j = 1; j < size; j++) if(sum_column(j, array,size) isnot sum_first_column ) return LLONG_MIN;
    return sum_first_column;
}

llong check_all_diags_equals(llong **array, int size){
    llong diag_right_left = sum_diag(0,size-1,array,size);
    llong diag_left_right = sum_diag(0,0,array,size);
    if(diag_left_right is diag_right_left)
        return diag_left_right;
    else return LLONG_MIN;
}

int any_equals(int* args, int size,llong exception){
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
    // printf("Sequential\n");
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //*******************//
    
    //line 1
    // printf("Checking rules...\n");
    llong check_all_lines = check_all_lines_equals(data->array,data->side_size);
    llong check_all_columns = check_all_columns_equals(data->array,data->side_size);
    llong check_all_diags = check_all_diags_equals(data->array,data->side_size);
    
    int rules[3] = {check_all_lines, check_all_columns,check_all_diags};
    
    int matches=any_equals(rules,3,LLONG_MIN);
    // printf("Line Sum: %lld\n",check_all_lines);
    // printf("Column Sum: %lld\n",check_all_columns);
    // printf("Diag Sum: %lld\n",check_all_diags);
    // if(matches is 3)
    //     printf("Quadrado magico\n");
    // else if(matches is 1)
    //     printf("Quadrado magico imperfeito\n");
    // else if(matches is 0) printf("Nao e quadrado magico\n");
    /**
     * Stop counting
     */
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    // printf("%lf\n", cpu_time_used); 
    //close

}

int main(int argc, char *argv[]){
    char file_name[100];
    if(argc > 1){
        strcpy(file_name,argv[1]);
    } else {
        scanf("%s",&file_name);
    }
    
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    magicSquare data = fetch_data(file_name);
    perfect_square_sequential(&data);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    int time_int = (int)cpu_time_used;
    int int_as_decimal = (int)((cpu_time_used-time_int)*1000000);
    printf("%d,%d\n",time_int,int_as_decimal); 
      
    return 0;   
}