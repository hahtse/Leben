#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <signal.h>

void singal(int s){
	exit(9);
}

void weiter(int s){
}

int main(int argc,char *argv[],int *envp[]) {
	int fd11[2],fd12[2], fd21[2],fd22[2];
	int rc;
	pid_t pid1, pid2;
	int status1, status2;
	char String1[100],String2[100];
	int ende1=0,ende2=0;
	int flag=0;
	char ch;

	sigset(SIGUSR2,singal);
	sigset(SIGUSR1,weiter);
	
	rc = pipe(fd11);
	if(rc<0){
		perror("pipe");
		exit(1);
	}
	rc = pipe(fd12);
	if(rc<0){
		perror("pipe");
		exit(1);
	}

	pid1 = fork();
	switch(pid1) {
		case -1:
			perror("1: Es wurde kein Cildprozess erzeugt.\n");
			exit(1);
		case 0:
			close(fd11[0]); //child 1 schreibt nach fd11
			close(fd12[1]); //child 1 liest aus fd12
			/*close(fd21[1]); //child 1 interessiert fd21 und fd22 nicht
			close(fd22[1]);
			close(fd21[0]);
			close(fd22[0]);*/
			while(ende1==0){
				rc = read(fd12[0],String1,sizeof(String1));
				if(rc<0){
					perror("Child 1 Lesefehler:");
					close(fd11[1]); //im fehlerfall gehen beide zu
					close(fd12[0]);
					exit(5);
				}
				printf("%s,%d\n",String1,rc);
				printf("CP1: Bitte eine Eingabe machen oder mit Ctrl-D Beenden.\n");
				if(fgets(String1,100,stdin)==NULL){
					close(fd11[1]); //bei ctrl+d gehen beide zu
					close(fd12[0]);
					ende1=1;
				}else{
					write(fd11[1],String1,sizeof(String1));
				}
			}
			exit(0);
		default:
			close(fd11[1]); //parent liest aus fd11
			close(fd12[0]);	//parent schreibt nach fd12
			break;
	}
	
	rc = pipe(fd21);
	if(rc<0){
		perror("pipe");
		exit(1);
	}
	rc = pipe(fd22);
	if(rc<0){
		perror("pipe");
		exit(1);
	}

	pid2 = fork();
	switch(pid2) {
		case -1:
			perror("2: Es wurde kein Cildprozess erzeugt.\n");
			exit(1);
		case 0:
			close(fd21[0]); //child 2 schreibt nach fd21
			close(fd22[1]); //child 2 liest aus fd22		//child 2 interessiert fd11 und fd12 nicht
			close(fd12[1]);
			close(fd11[0]);
			
			while(ende2==0){
				rc = read(fd22[0],String2,sizeof(String2));
				if(rc<0){
					perror("Child2 : Lesefehler");
					close(fd21[1]); //im fehlerfall gehen beide zu
					close(fd22[0]);
					exit(5);
				}
				printf("%s,%d\n",String2,rc);
				printf("CP2: Bitte eine Eingabe machen oder mit Ctrl-D Beenden.\n");
				if(fgets(String2,100,stdin)==NULL){
					close(fd21[1]); //bei ctrl+d gehen beide zu
					close(fd22[0]);
					ende2=1;
				}else{
					write(fd21[1],String2,sizeof(String2));
				}
			}
			exit(0);
		default:
			close(fd21[1]); //parent liest aus fd21
			close(fd22[0]);	//parent schreibt nach fd22
			break;
	}


	while(ende1 == 0 || ende2 == 0){
		if(ende1==0){
			printf("Child 1 abfragen\n");
			rc = write(fd12[1],"anfang1",8);
			if(rc<=0){
				printf("write 1,%d,%d",pid1,pid2);
				perror("write");
				kill(pid1,SIGUSR2);
				pid1 = waitpid(pid1,&status1,0);
				kill(pid2,SIGUSR2);
				pid2 = waitpid(pid2,&status2,0);
				exit(3);
			}
			rc = read(fd11[0],String1,sizeof(String1));
			switch(rc){
				case -1:
					perror("read");		
					kill(pid1,SIGUSR2);
					pid1 = waitpid(pid1,&status1,0);
					kill(pid2,SIGUSR2);
					pid2 = waitpid(pid2,&status2,0);
					exit(4);
				case 0:
					printf("1 zu\n");
					close(fd12[1]); //leerer String: parent macht pipes zu
					close(fd11[0]);
					ende1=1;
					break;
				default: 
					printf("PP1: %s\n",String1);
					break;
			}
		}
		if(ende2==0){
			printf("%d\n",ende2);
			printf("Child 2 abfragen\n");
			rc = write(fd22[1],"anfang2",8);
			if(rc<=0){
				printf("write 2,%d,%d",pid1,pid2);
				perror("write");
				kill(pid1,SIGUSR2);
				pid1 = waitpid(pid1,&status1,0);
				kill(pid2,SIGUSR2);
				pid2 = waitpid(pid2,&status2,0);
				exit(3);
			}
			rc = read(fd21[0],String2,sizeof(String2));
			switch(rc){
				case -1:
					perror("read");		
					kill(pid1,SIGUSR2);
					pid1 = waitpid(pid1,&status1,0);
					kill(pid2,SIGUSR2);
					pid2 = waitpid(pid2,&status2,0);
					exit(4);
				case 0:
					printf("2 zu\n");
					printf("%d\n",ende2);
					close(fd22[1]); //leerer String: parent macht pipes zu
					close(fd21[0]);
					ende2=1;
					printf("%d\n",ende2);
					break;
				default: 
					printf("PP2: %s\n",String2);
					break;
			}
		}
	}

	pid1 = waitpid(pid1,&status1,0);
	pid2 = waitpid(pid2,&status2,0);

	printf("CP1: Child-PID = %d, status %x %x\n", pid1,(status1>>8) & 0xFF, status1 & 0x7F);
	printf("CP2: Child-PID = %d, status %x %x\n", pid2,(status2>>8) & 0xFF, status2 & 0x7F);
			
	printf("Parent-Prozess ist fertig!\n");
	return 0;
}


