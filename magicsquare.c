#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <time.h>
#define llong long long
#define max_length 400041001



llong int_readfile(FILE *f, llong *a)
{
	llong result = 0;
	llong number= 0;
	while (fscanf(f, "%lld", &number) != EOF){
	//   printf("read item");
	  a[result++] = number;}
	return result;
}


void ints_println_arraybi(int **a, int n)
{
	printf("%d", n);
	printf("\n" );
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			printf("%d ", a[i][j]);
		}
		printf("\n");
	}
	
}


void square(llong **sq,llong *a, int n)
{
	int count=0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			sq[i][j] = a[count++];
		}
	}	
}

llong *ints_new(int n)
{
  //  return (int *) calloc(n, sizeof (int));
  return (llong *) malloc (n * sizeof(llong));
}




llong sumRows(llong **sq, int n){
	llong firstRow=0;
	for (int i = 0; i < n; i++)
	{
		firstRow+= sq[0][i];
	}
	printf("%lld ",firstRow);
	for (int i =0 ; i < n; i++)
	{
		llong sumRow=0;
		for (int j = 0; j < n; j++)
		{
			sumRow += sq[i][j];
		}
		if (firstRow!= sumRow)
		{
			firstRow=-1;
		}
		
	}
	
	return firstRow;
}


llong sumCols(llong **sq, int n){
	llong firstCol=0;
	for (int i = 0; i < n; i++)
	{
		firstCol+= sq[i][0];
	}
	printf("%lld ",firstCol);
	for (int j =0 ; j < n; j++)
	{
		llong sumCol=0;
		for (int i = 0; i < n; i++)
		{
			sumCol += sq[i][j];
		}
		if (firstCol!= sumCol)
		{
			firstCol=-1;
			break;
		}
		
	}
	
	return firstCol;
}


llong sumDigs(llong **sq, int n){
	llong sumDig=0;
	llong sumAntiDig=0;
	for (int i =0 ; i < n; i++)
	{
		sumDig+=sq[i][i]; 
		sumAntiDig += sq[n-1-i][i];
	}
	if (sumDig!=sumAntiDig)
	{
		sumDig=-1;
	}
	return sumDig;
}


int isMagicSquare(llong **sq, int n){
	llong dig= sumDigs(sq, n);
	llong row= sumRows(sq, n);
	llong col= sumCols(sq, n);
	int result=-1;
	if (dig==row && dig==col && row!=-1)
	{
		result=0;
	}
	else if (row == col && row!=-1)
	{
		result=1;
	}
	return result;
}

void task_a()
{
	llong *a= ints_new(max_length);
	FILE *f = fopen("inputs/p20001.txt", "r");
	
	llong size = int_readfile(f, a);
	size = int_readfile(f, a);
	printf("read");
	llong squareSize = sqrt(size);
	llong **sq = (llong **)malloc(squareSize * sizeof(llong *));
	for (int i = 0; i < squareSize; i++)
	{
		sq[i] = (llong *)malloc(squareSize * sizeof(llong));
	}
	square(sq, a, squareSize);
	free(a);
	//ints_println_arraybi(sq,squareSize);
	int finish = isMagicSquare(sq, squareSize);
	if (finish==0)
	{
		printf("Quadrado magico");
	}
	else if (finish==1)
	{
		printf("Quadrado magico imperfeito");
	}
	else
	{
		printf("Nao e quadrado magico.");
	}
}

int main(int argc, const char **argv)
{	
	clock_t begin = clock();
	task_a();
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("\n%lf\n", time_spent);

}