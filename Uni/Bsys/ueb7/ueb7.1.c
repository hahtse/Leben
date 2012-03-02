#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){

	char towrite[100],entsch;
	int fd;

	printf("\nChild 1 gestartet\n");
	sscanf(argv[2],"%d",&fd);
	printf("Jetzt bitte zeilenweise eingeben: \n");
	while(1){
		fgets(towrite,100,stdin);
		if(write(fd,towrite,100) == -1){
			perror("Fehler bei write");
			exit(1);
		}
		printf("Wollen sie noch eine Zeile eingeben? j/n:  ");
		while((entsch=getchar())!='j'&&entsch!='n');
		if(entsch=='n'){
			printf("\nChild 1 endet hier\n");
			exit(0);
		}
		getchar();
	}
	perror("komischer fehler");
	exit(1);
}

