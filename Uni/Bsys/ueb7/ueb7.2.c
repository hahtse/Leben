#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){

	char towrite[100],entsch;
	int status=0;
	int fd;

	sscanf(argv[2],"%d",&fd);
	printf("\nChild 2 gestartet\n");
	printf("Ausgabe:\n");
	while((status=read(fd,towrite,100)) > 0){
		if(status<0){
			perror("Fehler bei read");
			exit(1);
		}
		printf("%s",towrite);	
	}
	exit(0);
}
