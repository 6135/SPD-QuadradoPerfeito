#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include <pthread.h>

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
    int cols_sum;
    int lines_sum;
    int diag_sum;
} magicSquare;

magicSquare MagicSquare(int **array, int array_size){
    magicSquare result;
    result.cols_sum = 0;
    result.lines_sum = 0;
    result.diag_sum = 0;
    result.side_size = sqrt(array_size);
    result.array_size = array_size;

    result.array = malloc(result.side_size*sizeof(int));
    for (int i = 0; i < result.side_size; i++)
        result.array[i] = malloc(result.side_size*sizeof(int));
    for (size_t i = 0; i < result.side_size; i++)
        memcpy(result.array,array,sizeof(int)*array_size);

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

int sum_line(int *array ,int size){
    int sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i];
    return sum;
}

int sum_column(int j,int **array ,int size){
    int sum = 0;
    for (int i = 0; i < size; i++) sum+= array[i][j];
    return sum;
}

int sum_diag(int i, int j, int **array, int size){
    int sum = 0;
    if(i is 0 and j is (size-1))
        for(int k = 0; k < size; k++) sum+= array[i+k][j-k];
    else if (i is j and i is 0)
        for(int k = 0; k < size; k++) sum+= array[i+k][j+k];

    return sum;
    
}

int check_all_lines_equals(int **array, int size){
    int sum_first_line = sum_line(array[0],size);
    for (int i = 1; i < size; i++){ if( sum_line(array[i],size) isnot sum_first_line ) return INT_MIN; }
    return sum_first_line;
}

int check_all_columns_equals(int **array, int size){
    int sum_first_column = sum_column(0,array,size);
    for (int j = 1; j < size; j++) if(sum_column(j, array,size) isnot sum_first_column ) return INT_MIN;
    return sum_first_column;
}

int check_all_diags_equals(int **array, int size){
    int diag_right_left = sum_diag(0,size-1,array,size);
    int diag_left_right = sum_diag(0,0,array,size);
    if(diag_left_right is diag_right_left)
        return diag_left_right;
    else return INT_MIN;
}

int any_equals(int* args, int size,int exception){
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
    printf("Sequential\n");
    FILE *fp;
    fp = fopen(file_path,"r");
    int size_array = array_size_count(fp); //count size
    // printf("Number of elements: %d\n", size_array);

    int sqrN = sqrt(size_array);
    int **array = malloc(sqrN*sizeof(int));
    for (int i = 0; i < sqrN; i++)
        array[i] = malloc(sqrN*sizeof(int)); //allocate array

    // printf("File read\n");
    fp = fopen(file_path,"r");
    array_fill(fp,size_array,sqrN,array);

    //line 1
    // printf("Checking rules...\n");
    int check_all_lines = check_all_lines_equals(array,sqrN);
    printf("Lines: %d\n",check_all_lines);
    int check_all_columns = check_all_columns_equals(array,sqrN);
    printf("Columns: %d\n",check_all_columns);
    int check_all_diags = check_all_diags_equals(array,sqrN);
    printf("Diags: %d\n",check_all_diags);
    int rules[3] = {check_all_lines, check_all_columns,check_all_diags};
    
    int matches=any_equals(rules,3,INT_MIN);
    
    if(matches is 3)
        printf("Quadrado magico\n");
    else if(matches is 1)
        printf("Quadrado magico imperfeito\n");
    else if(matches is 0) printf("Nao e quadrado magico\n");

    //close
    fclose(fp);
    for(int i = 0; i < sqrN; i++)
        free(array[i]);
    free(array);

}

void* check_all_lines_equals_seq(void* param){
    magicSquare *magic_square = (magicSquare *) param;
    int sum_first_line = sum_line(magic_square->array[0],magic_square->side_size);
    printf("firstLine %d, numLines: %d\n",sum_first_line,magic_square->side_size);
    for (int i = 1; i < magic_square->side_size; i++){
        printf("%d ",i);
        if( sum_line(magic_square->array[i],magic_square->side_size) isnot sum_first_line ) {
            magic_square->lines_sum = INT_MIN;
            pthread_exit(NULL);
        }
    }
    magic_square->lines_sum = sum_first_line;
    pthread_exit(NULL);
}

void* check_all_columns_equals_seq(void* param){
    magicSquare *magic_square = (magicSquare *)param;
    int sum_first_column = sum_column(0,magic_square->array,magic_square->side_size);
    for (int j = 1; j < magic_square->side_size; j++) 
        if(sum_column(j, magic_square->array,magic_square->side_size) isnot sum_first_column ) {
            magic_square->cols_sum = INT_MIN;
            pthread_exit(NULL);
        }
    magic_square->cols_sum = sum_first_column;
    pthread_exit(NULL);
}

void* check_all_diags_equals_seq(void* param){
    magicSquare *magic_square = (magicSquare *)param;
    int diag_right_left = sum_diag(0,magic_square->side_size-1,magic_square->array,magic_square->side_size);
    int diag_left_right = sum_diag(0,0,magic_square->array,magic_square->side_size);
    if(diag_left_right is diag_right_left) {
        magic_square->diag_sum = diag_left_right;
    }
    else  magic_square->diag_sum = INT_MIN;
    pthread_exit(NULL);
}

void perfect_square_threaded(const char* file_path){
    printf("Threaded\n");
    FILE *fp;
    fp = fopen(file_path,"r");
    int size_array = array_size_count(fp); //count size

    int sqrN = sqrt(size_array);
    int **array = malloc(sqrN*sizeof(int));
    for (int i = 0; i < sqrN; i++)
        array[i] = malloc(sqrN*sizeof(int)); //allocate array

    fp = fopen(file_path,"r");
    array_fill(fp,size_array,sqrN,array);

    magicSquare magic_square = MagicSquare(array,size_array);

    pthread_t tid[3];
    pthread_create(&tid[0],NULL,check_all_lines_equals_seq,(void*)&magic_square);
    pthread_create(&tid[1],NULL,check_all_columns_equals_seq,(void*)&magic_square);
    pthread_create(&tid[2],NULL,check_all_diags_equals_seq,(void *)&magic_square);
    
    for(int i = 0; i < 3; i++){
        pthread_join(tid[i],NULL);
        printf("threadNu: %d\n", i);
    }
        
    
    printf("Lines: %d\n",magic_square.lines_sum);
    printf("Columns: %d\n",magic_square.cols_sum);
    printf("Diags: %d\n",magic_square.diag_sum);
    int rules[3] = {magic_square.lines_sum, magic_square.cols_sum,magic_square.diag_sum};
    
    int matches=any_equals(rules,3,INT_MIN);
    
    if(matches is 3)
        printf("Quadrado magico\n");
    else if(matches is 1)
        printf("Quadrado magico imperfeito\n");
    else if(matches is 0) printf("Nao e quadrado magico\n");

    //close
    fclose(fp);
    for(int i = 0; i < sqrN; i++)
        free(array[i]);
    free(array);

}
int main(){
    perfect_square_threaded("input3.txt");

}