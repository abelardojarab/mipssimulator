/* gcd.c */

/* calculates the gcd(x,y) */

/*---------------------------------------------------------------------------*/

#include <stdio.h>

/*---------------------------------------------------------------------------*/

int main() {
    
    int x= 12;
    int y= 10;
    
    while( x != y ) {
        
        if( x > y ) {
            x = x - y;
        }
        else {
            y = y - x;
        }
    }
    printf("gcd is %d\n", x);
	return 0;
    
}

/*---------------------------------------------------------------------------*/

