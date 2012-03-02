#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

short isPrime(long p);

int main(int argc,char **argv){
	
	long n=atoi(argv[1]);
	int abst=atoi(argv[2]);
	long i,anz=0;
	for(i=0;i<argc;i++){	
		printf("argv[%ld] = %s",i,argv[i]);
	}
	
	for ( i = 2; i <= n; i++ )
		if ( isPrime(i) ) {
			anz++;
			if(anz%abst==0){		
				printf("\n%ld, %d", i, getpid());
			}
	}
	printf ("\n%ld Primzahlen\n", anz);
	exit(0);

}

short isPrime(long p){
	long s, d;
	s = (long) sqrt(p) + 1;
	if ( p <  2 ) return 0;
	if ( p == 2 ) return 1;
	if ( p % 2 == 0 ) return 0;
	for ( d = 3; d <= s; d+=2 )
		if ( p % d == 0 ) return 0;
	return 1;
}
