#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

short isPrime(long p);

int main(){
    
	pid_t pid;	
	int status;
	long i, n, anz=0;
	pid = fork();
	if (pid==-1){
		perror("konnte child nicht einrichten");
		return 1;
	}
	if (pid==0){
		n=5000000;
		printf("\npid = %d",getpid());
		printf("\nparent pid = %d",getppid());
	}
	else{ 
		n=4000000;
		printf("\npid = %d",getpid());
	}
	
	for ( i = 2; i <= n; i++ )
		if ( isPrime(i) ) {
			anz++;
			if(anz%2000==0){		
				printf ("\n%ld, %d", i, getpid());
			}
	}
	printf ("\n%ld Primzahlen\n", anz);
	
	if (pid!=0){
		pid=wait(&status);
		if (pid==-1){
			perror("Childfehler");
			return 1;
		}
		printf("\n\nChild-PID=%d, status |%x|%x|\n",pid,(status>>8)&0xFF,status&0x7F);
	}
	else
		exit(1);
	return 0;
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
