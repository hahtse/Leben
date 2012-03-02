#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include <math.h>

int flag=0;

short isPrime(unsigned long int p){
  long s, d;
  s = (long) sqrt(p) + 1;
  if ( p <  2 ) return 0;
  if ( p == 2 ) return 1;
  if ( p % 2 == 0 ) return 0;
  for ( d = 3; d <= s; d+=2 )
    if ( p % d == 0 ) return 0;
  return 1;
}

void sig(int s){
	flag=0;
	printf(".");
	fflush(stdout);
}

void sig2(int s){
	if(flag==1){
		flag=0;
		printf(".");
		fflush(stdout);
	}else{
		flag=1;
		printf("-");
		fflush(stdout);
	}
}

int main(){
	int i=0;
	int ranzahl[10]; //anzahl der durchläufe
	int vanzahl[10];
	double rmittel=0; //zeitmittelwerte
	double vmittel=0;	
	struct itimerval Vtimer; //zeitmessungselemente
	struct itimerval Rtimer;

	sigset(SIGALRM,sig); //Signalbehandlung
	sigset(SIGVTALRM,sig2);

	timerclear (&Vtimer.it_interval); //timer zurücksetzen
	timerclear (&Vtimer.it_value);
	timerclear (&Rtimer.it_interval);
	timerclear (&Rtimer.it_value);
	Vtimer.it_value.tv_usec=1; //timer setzen
	Rtimer.it_value.tv_usec=1;
	Vtimer.it_interval.tv_sec=1;
	Rtimer.it_interval.tv_sec=1;

	setitimer(ITIMER_REAL,&Rtimer,NULL);
	pause();
	for(i=0;i<10;i++){
		flag=1;
		ranzahl[i]=0;
		while(flag){
			isPrime(ranzahl[i]++);		
		}
		rmittel+=ranzahl[i];
	}
	timerclear (&Rtimer.it_interval);
	timerclear (&Rtimer.it_value);
	setitimer(ITIMER_REAL,&Rtimer,NULL);
	rmittel=rmittel/10;

	setitimer ( ITIMER_VIRTUAL , &Vtimer , NULL);
	while(flag!=1);
	for(i=0;i<10;i++){
		flag=1;
		vanzahl[i]=0;
		while(flag){
			isPrime(vanzahl[i]++);		
		}
		vmittel+=vanzahl[i];
	}
	timerclear (&Vtimer.it_value);
	timerclear (&Vtimer.it_interval);
	setitimer(ITIMER_VIRTUAL,&Vtimer,NULL);
	vmittel/=10;

	printf("\nAnzahl der erreichten Primzahlberechnungen in Echtzeit pro Sekunde:\n");
	printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",ranzahl[0],ranzahl[1],ranzahl[2],ranzahl[3],ranzahl[4],ranzahl[5],ranzahl[6],ranzahl[7],ranzahl[8],ranzahl[9]);
	printf("Im Mittel wurden %.0f Primzahlen berechnet.\n\n",rmittel);

	printf("Anzahl der erreichten Primzahlberechnungen in virtueller Zeit pro Sekunde:\n");
	printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t\n",vanzahl[0],vanzahl[1],vanzahl[2],vanzahl[3],vanzahl[4],vanzahl[5],vanzahl[6],vanzahl[7],vanzahl[8],vanzahl[9]);
	printf("Im Mittel wurden %.0f Primzahlen berechnet.\n\n",vmittel);
	return 0;
}
