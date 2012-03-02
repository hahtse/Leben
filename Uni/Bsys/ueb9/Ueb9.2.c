#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

int main(int argc,char *argv[],int *envp[]) {
	char String[100];
	int fd;
	pid_t pid;

	printf("Child2: Childprozess 2 ist bereit.\n");

	sscanf(argv[2],"%d",&pid);
	sscanf(argv[1],"%d",&fd);
	printf("Bitte Eintra(e)g(e) in die Datei machen.\n");
	
	while( fgets(String,100,stdin) != NULL){
		write(fd, String, 100);
	}
	
	close(fd);
		
	kill(pid,SIGUSR1);
	printf("CP2: Signal gesendet\n");

	exit(0);
}
