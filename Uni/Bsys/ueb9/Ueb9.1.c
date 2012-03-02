#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

int flag=1;

void sig1(int s) {
	printf("CP1: Signal erhalten.\n");
	flag=0;
}

void sig2(int s) {
	exit(1);
}

int main(int argc,char *argv[],int *envp[]) {
	char String[100];
	int i=0;
	int fd;
	pid_t ppid;
	FILE* file;

	sigset(SIGUSR1,sig1);
	sigset(SIGUSR2,sig2);
	
	ppid=getppid();
	printf("Child1: Childprozess 1 ist bereit.\n");
	sleep(1);
	kill(ppid,SIGUSR1);

	file=fopen(argv[1],"rb");
	fd=fileno(file);
	if(file == 0)  {
		printf("Das File konnte nicht geoeffnet werden.");
		perror("fopen");
		exit(2);
	}

	while(flag){
		sleep(1);
	}

	sscanf(argv[1],"%d",&fd);
	while(read(fd,String,sizeof(String))>0){
		i++;
		printf("%d: %s",i,String);
	}

	exit(0);
}
