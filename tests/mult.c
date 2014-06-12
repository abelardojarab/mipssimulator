/* mult.c */

#include <stdio.h>


int main()
{
    int box = 4;
    int i, j, temp;
    int var = 0;
    int A[box][box];
    int B[box][box];
    int C[box][box];
   
    for(i=0; i<box; i++){
	for(j=0; j<box; j++){
	    A[i][j] = i;
	    B[i][j] = j;
	}
    }
    
    for(i=0; i<box; i++){
	for(j=0; j<box; j++){
	    var = 0;
	    for(temp=0; temp<box; temp++){
		var = var + (A[i][temp]*B[temp][j]);
	    }
	    C[i][j] = var;
	}
    }

    for (i=0; i<box; i++) {
      for (j=0; j<box; j++)
		printf("C[%d][%d]=%d\t",i,j, C[i][j]);
	  printf("\n");
    }

    return 0;
}



