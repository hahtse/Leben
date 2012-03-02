#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
	pid_t pid;
	int signr=0,rc;
	int squit=0;


	sscanf(argv[1],"%d",&pid);
	printf("\nChild 2 läuft, PID = %d\n",pid);
	while(squit==0){
		printf("\nBitte Signalnr. eingeben!  ");
		scanf("%d",&signr);
		if(signr!=0){
			switch(signr){
				case 1:
					rc = kill(pid,SIGUSR1);
					if(rc<0){
						printf("Signalsende-Error");
					}
					break;
				case 2:
					rc = kill(pid,SIGUSR2);
					if(rc<0){
						printf("Signalsende-Error");
					}
					break;
				case 3:
					rc = kill(pid,SIGINT);
					if(rc<0){
						printf("Signalsende-Error");
					}
					break;
				case 4:
					rc = kill(pid,SIGQUIT);
					if(rc<0){
						printf("Signalsende-Error");
					} else {
						exit(0);
					}					
					break;
				default:
					printf("\nUnbekanntes Signal!\n");
					break;
			}
		}

	}
	exit(1);	
}
