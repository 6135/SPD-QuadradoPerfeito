#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct {
    char **nome[1000];
    int data;
    int size_nome;
} Pessoa;


Pessoa read_csv(FILE *fp){
    Pessoa pessoa;
    pessoa.size_nome = 0;
    char c = ' ';
    while(c != ',') {//end of name
        c = fgetc(fp);
        pessoa.nome[pessoa.size_nome++]=c;
    }
    c = fgetc(fp);
    char *date[10];
    int size_date = 0;
    while(c != '\n' || c != EOF) { //end of line
        date[size_date++]=c;
    }
    pessoa.data = atoi(date);
    return pessoa;
}

int read_csv_full(Pessoa *pessoas, FILE *fp){
    int result = 0;

    
}


int main(){

    return 0;
}