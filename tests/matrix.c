// matrix.c

#include <stdio.h>

int main()
{
    int i, j;
    int A[10][10], B[10][10];
    

    for( i=0; i<=9; i++){
	for( j=0; j<=9; j++){
	    A[i][j] = i;
	    B[i][j] = j;
		printf("A[%d][%d]=%d  ",i,j,i);
	}
	printf("\n");
    }
	printf("\n");
    for( i=0; i <=9; i++){
	for ( j=0; j <=9; j++){
		printf("B[%d][%d]=%d  ",i, j, B[i][j]);
	} 
	printf("\n");
    }
	printf("\n");
    
    for( i=1; i <9; i++){
	for ( j=1; j <9; j++){
	    B[i][j] = (A[i-1][j] + A[i+1][j] + A[i][j+1] + A[i][j-1]) / 4;
	} 
    }
    for( i=0; i <=9; i++){
	for ( j=0; j <=9; j++){
		printf("B[%d][%d]=%d  ",i, j, B[i][j]);
	} 
	printf("\n");
    }
	return 0;
}


