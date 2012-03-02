#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

short isPrime(long p);
void sigusr1fkt(int snr);
void sigusr2fkt(int snr);
void sigintfkt(int snr);
void sigquitfkt(int snr);
int usr2=0;
int zusr1=0,zusr2=0,zint=0,zquit=0;
long i;
pid_t apid;


int main(int argc,char **argv){
	
	int signr, rc;
	long n=atoi(argv[1]);
	int abst=atoi(argv[2]);
	long anz=0;
 
	apid=getpid();
	printf("\nChild 1 läuft\n");
	for(i=0;i<=SIGRTMIN-1;i++){
		sigset(i,SIG_IGN);
	}	
	sigset(SIGUSR1,sigusr1fkt);
	sigset(SIGUSR2,sigusr2fkt);
	sigset(SIGINT,sigintfkt);
	sigset(SIGQUIT,sigquitfkt);
	
	for ( i = 2; i <= n; i++ ){
		if ( isPrime(i) ) {
			anz++;
		}
		if(usr2==1){
			i=1;
			anz=0;
			usr2=0;
		}
	}
	printf("\n %ld Primzahlen\n", anz);
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

void sigusr1fkt(int snr){
	printf("\n%ld, %d\n", i, apid);
	zusr1++;
}

void sigusr2fkt(int snr){
	usr2=1;
	printf("\nBerechnung zurückgesetzt\n");
	zusr2++;
}

void sigintfkt(int snr){
	zint++;
	printf("\nSIGUSR1: %d\n",zusr1);
	printf("SIGUSR2: %d\n",zusr2);
	printf("SIGINT: %d\n",zint);
	printf("SIGQUIT: %d\n",zquit);
}

void sigquitfkt(int snr){
	exit(0);
}

