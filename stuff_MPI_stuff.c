#include <stdio.h>
// #include <stdlib.h>
// #include <stdbool.h>
// #include <math.h>
// #include <time.h>
// #include <ctype.h>
// #include<string.h> 
#include <mpi.h>
// #include <unistd.h>

// #define ullong unsigned long long
// #define llong long long
// #define MPI_ullong MPI_UNSIGNED_LONG_LONG
// int r = 0;
// ullong size;
// ullong *cols_sum;
// int world_size = 0;
// int world_rank = 0;

// //------------------------------
// // Create a dinamically allocated array with capacity `n`
// ullong *ullongs_new(ullong n)
// {
//   //  return (ullong *) calloc(n, sizeof (ullong));
//   return (ullong *) calloc (n , sizeof(ullong));
// }
// //-------------------------------

// //obter o tamanho a ordem da matriz a partir do nome do ficheiro
// void get_size(char *filename){
//     char *f = filename;
//     while(*f){
//         if (isdigit(*f))  size = strtol(f,&f,10);
//         else f++;   
//     }
//     //magic_const= size * ( size * size + 1 ) / 2; //calcular o valor da constante magica
//     cols_sum = ullongs_new(size); //alocar memoria para a matriz que guarda a soma das colunas
// }


// void check_magic_square(ullong n, ullong position, ullong *values,ullong mc, ullong *data){
//     ullong row_temp = 0;
    
//     for(ullong i = 0; i < n && r < 2 ;i++){

//         ullong row = (ullong)((i+position)/size); 
//         ullong col = (ullong)((i+position)%size); 
        
//         cols_sum[col] += values[i];
//         row_temp += values[i];

//         if(row == col) data[0] += values[i];
//         if(col == ( size - 1 ) - row ) data[1] += values[i];
//         if(col == size - 1){
//             if( mc == row_temp) row_temp = 0;
//             else r = 2;
//         } 
//     } 
    
// }

// void child(){
//     ullong mc = 0;
//     ullong position = 0;
//     ullong n = 0;
//     ullong *data = ullongs_new(3);

//     MPI_Recv(&position, 1, MPI_ullong, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     MPI_Recv(&mc, 1, MPI_ullong, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     MPI_Recv(&n, 1, MPI_ullong, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     ullong *values = ullongs_new(n);
//     MPI_Recv(values, n, MPI_ullong, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

//     check_magic_square(n, position, values, mc, data);

//     data[2] = r;
//     MPI_Request request1, request2;

//     MPI_Send(data, 3, MPI_ullong, 0, 4, MPI_COMM_WORLD);
//     MPI_Send(cols_sum, size, MPI_ullong, 0, 5, MPI_COMM_WORLD);
    
//     free(data);
//     free(values);
// }


// void read_file_parent(char *file){
//     MPI_Comm_size(MPI_COMM_WORLD, &world_size);
//     printf("Rstart size %d\n",world_size);
    
//     ullong *n_per_pro= ullongs_new(world_size);

//     for (ullong i = 0; i < world_size-1; i++){
//         n_per_pro[i] = (size/world_size);
//         n_per_pro[i]*= size;
//     }
        
//     n_per_pro[world_size-1] = (size*size)-(n_per_pro[0] * (world_size-1));

//     FILE *fp = fopen(file,"r");
//     ullong n;
//     ullong p=1;
//     ullong *data = ullongs_new(n_per_pro[world_size-1]);
//     ullong start=0;
//     ullong msg=0;
//     ullong mc = 0;
//     ullong dlr_sum = 0;
//     ullong drl_sum = 0;
//     ullong *temp_results = ullongs_new(3);
//     ullong *temp_cols_sum = ullongs_new(size);
//     printf("r0\n");
//     for(ullong i = 0; i <  size*size; i++){
//         fscanf(fp,"%d", &n);
//         data [i-start] = n;
//         mc += data[i-start] * (i < size); 
        
//         if(i == (n_per_pro[p-1]-1)+start && p < world_size){
//             // printf("Sending to p %d\n",p);
//             MPI_Send(&start, 1, MPI_ullong, p, 0, MPI_COMM_WORLD);
//             MPI_Send(&mc,1,MPI_ullong, p, 1, MPI_COMM_WORLD);
//             MPI_Send(&n_per_pro[p-1], 1, MPI_ullong, p, 2, MPI_COMM_WORLD);
//             MPI_Send(data, n_per_pro[p-1], MPI_ullong, p, 3, MPI_COMM_WORLD);
//             // printf("Done sending to p %d\n",p);
//             start += n_per_pro[p-1];
//             p++;     
//         }    
//     }
//     fclose(fp);   
//     printf("r1\n");
//     for (ullong i = 1; i < world_size; i++){
//         // printf("Recv data %d\n",i);
//         MPI_Recv(temp_results, 3, MPI_ullong, MPI_ANY_SOURCE, 4,MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//         MPI_Recv(temp_cols_sum, size, MPI_ullong, MPI_ANY_SOURCE, 5,MPI_COMM_WORLD, MPI_STATUS_IGNORE); 
//         // printf("Done recv data %d\n",i);
//         if( r < 2) {
//             for (ullong j = 0; j < size; j++)
//                 cols_sum[j] += temp_cols_sum[j];
            
//             dlr_sum += temp_results[0];
//             drl_sum += temp_results[1];
//             r = temp_results[2];
//         }
//     }
    
//     if (r < 2) {
//         memset(temp_results, 0, sizeof(temp_results));
//         check_magic_square(n_per_pro[world_size-1], start, data, mc, temp_results);
//         dlr_sum += temp_results[0];
//         drl_sum += temp_results[1];
//         for (ullong i = 0; i < size; ){
//             r = (cols_sum[i] != mc)*2;
//             i = size*(r==2)+i+1;
//         }
//     }

//     r = 1*(dlr_sum != mc || drl_sum != mc) + 2*(r==2);
//     char* result[] = {"Quadrado magico\n","Quadrado imperfeito\n","Nao e quadrado magico\n","Nao e quadrado magico\n"};
//     printf("%s",result[r]); 
// }


// int main(int argc, char *argv[])
// {   
//     // char *filename;
//     // if (argc >= 2 )
//     //     filename = argv[1];
//     // else { printf("Filename not specified!\n Exiting...\n"); exit(0); }
//     // get_size(filename);
    
//     MPI_Init(NULL,NULL);
//     MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

//     // printf("World rank %d\n",world_rank);
//     if(world_rank == 0)
//         { printf("stuff parent\n"); }
//     else { printf("sutff child\n"); }

//     MPI_Finalize();
//     return 0;
// }
int main(int argc, char *argv[]){
    MPI_Init(NULL, NULL);
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    // char file_path[256];
    // if(argc > 1){
    //     strcpy(file_path,argv[1]);
    // } else {
    //     printf("%s Nao foi introduzido o argumento, indique o caminho da input\n",processor_name);
    //     return 1;
    // }
    printf("stuff child\n");
    // init_struct_type_magicsquare();
    // execute_MPI(file_path);
    MPI_Finalize();
    return 0;
}