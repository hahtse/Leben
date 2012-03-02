#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>

int flag=1;

void siguser(){
	printf("Parent: Signal erhalten.\n");
	flag=0;
}

int main(int argc,char *argv[],int *envp[]) {
	int fd;
	char *argv2[4];
	char arg1[30];
	char arg2[30];
	char arg3[30];
	char filename [100];
	pid_t pid;
	pid_t pid2;
	int status;
	int status2;
	char c;
	FILE * file;

	sigset(SIGUSR1,siguser);


	if(argc!=2){
		printf("Geben sie den Namen des zu bearbeitenden Files an.\n");
		fgets(filename,100,stdin);
		filename[strlen(filename)-1]='\0';
	}else{
		strcpy(filename,argv[1]);
	}


	file = fopen (filename,"rb");
	if(file != 0){
		fclose(file);
		printf("Das angegebene File existiert bereits.\n Soll das File fortgesetzt werden? j/n.\n");
		c=getchar();
		if(c=='j'){
			file = fopen (filename,"ab");
		}else{
			file = fopen(filename,"wb");
		}
	}else{
		printf("Das angegebene File wurde nicht gefunden!\n Soll das File neu angelegt werden? j/n.\n");
		c=getchar();
		if(c=='j'){
			file = fopen (filename,"wb");
		}else{
			printf("Programm mit neuem Filenamen neu starten!\n"); 
			exit(1);
		}
	}
	if(file == 0)  {
		printf("Das File konnte nicht geoeffnet werden.\n");
		perror("fopen");
		exit(2);
	}

	pid = fork();
	switch(pid) {
		case -1:
			perror("1: Es wurde kein Cildprozess erzeugt.\n");
			exit(1);
		case 0:
			strcpy(arg1,"Ueb9.1");
			argv2[0]=arg1;
			argv2[1]=filename;
			argv2[2]=NULL;
			execv(arg1,argv2);
			perror("execv");
			exit(2);
		default:
			break;
	}
	
	while(flag==1){
		sleep(1);
	}
	printf("Parent: Der zweite Childprozess wird gestartet.\n");
	
	pid2 = fork();
	switch(pid2) {
		case -1:
			perror("2: Es wurde kein Cildprozess erzeugt.\n");
			kill(pid,SIGUSR2);
			pid = waitpid(pid,&status,0);
			exit(1);
		case 0:
			strcpy(arg1,"Ueb9.2");
			argv2[0]=arg1;
			fd = fileno(file);
			sprintf(arg2,"%d",fd);
			argv2[1]=arg2;
			sprintf(arg3,"%d",pid);
			argv2[2]=arg3;
			argv2[3]=NULL;
			execv(arg1,argv2);
			kill(pid,SIGUSR2);
			pid = waitpid(pid,&status,0);
			perror("execv");
			exit(2);
		default:
			break;
	}
	pid = waitpid(pid,&status,0);
	pid2= waitpid(pid2,&status2,0);

	printf("Child1: Child-PID = %d, status %x %x\n", pid, (status>>8) & 0xFF, status & 0x7F);
	printf("Child2: Child-PID = %d, status %x %x\n", pid2,(status2>>8) & 0xFF, status2 & 0x7F);
			
	printf("Parent-Prozess ist fertig!\n");
	return 0;
}
