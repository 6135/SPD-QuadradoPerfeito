#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
#define llong unsigned long long

llong magic_sum = -1;
long order;


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

void read_first_line(llong *sum_cols,llong *lrd_sum, llong *rld_sum, FILE *fp){
    llong value_read;
    fscanf(fp,"%lld",&value_read);
    llong sum_line = value_read;
    lrd_sum[0] = value_read;
    sum_cols[0] = value_read;
    for (size_t i = 1; i < order; i++) {
        fscanf(fp,"%lld",&value_read);
        sum_line += value_read;
        sum_cols[i] = value_read;
        if(i is order-1){
            rld_sum[0]+=value_read;
        }
    }
    magic_sum = sum_line;
    
}

int calc(llong *sum_cols, FILE *fp){
    /**
     * we read first value so we save on conditions later on
     */
    llong size = pow(order,2);
    llong lrd_sum = 0, rld_sum = 0;
    read_first_line(sum_cols,&lrd_sum,&rld_sum,fp);
    llong value_read;
    llong sum_line = magic_sum;
    int lrd_ptr = order+1, rld_ptr = (order-1)*2, column_index;

    for(size_t i = order; i < size; i++){

        if( i%(order) is 0) {
            if(sum_line isnot magic_sum){
                return -1;
            } else 
                sum_line = 0;
        }

        fscanf(fp,"%lld",&value_read);
        sum_line += value_read;
        column_index = i%order;
        sum_cols[column_index] += value_read;

        if(i is lrd_ptr){
            lrd_ptr += (order+1);
            lrd_sum+=value_read;
        }

        if(i is rld_ptr){
            rld_ptr += (order-1);
            rld_sum+=value_read;
        }

    }
    for (size_t i = 0; i < order; i++){
        if(sum_cols[i] isnot magic_sum)
            return -1;
    }
    rld_sum -= value_read;
    if(lrd_sum isnot rld_sum)
        return 0;
    return 1;

}

void execute_sequential(char *filepath){
    order = parse_order(filepath);
    FILE *fp = fopen(filepath,"r");
    llong *sum_cols = malloc(order*sizeof(llong));
    int res = calc(sum_cols,fp);
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
        printf("Nao introduzio argumento, indique o caminho da input: ");
        scanf("%s",&file_path);
    }
    execute_sequential(file_path);
}