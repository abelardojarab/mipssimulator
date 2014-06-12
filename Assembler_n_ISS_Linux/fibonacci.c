#include<stdio.h>
#include<stdlib.h>
int main()
{
	int i=3,x=1,y=1,n=0,result=0,temp=0;
	i = 3;
	x = 1;
	y = 1;
	printf("\nInput the nth term of fibonacci series to calculate:\t");
	scanf("%d",&n);
	printf("n=%d\n", n);
	while (i <= n)
	{
	  temp = x+y;
	  x = y;
	  y = temp;
	  i ++;
	}
	result = y;
	printf("The %dth term of fibonacci series is %d \n", n, result);
	return 0;
}

