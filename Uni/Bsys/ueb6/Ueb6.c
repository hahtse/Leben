#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argumente){
    
	pid_t pid1, pid2,p1=0,p2=0;
	char filename[100];
	char *argv[4], arg1[100],arg2[100];	
	int status,abst=0,option,flag1=0,flag2=0;
	long i, n, n1=0, n2=0, anz=0;
	if(argc!=4){
		printf("Bitte n für Child 1 eingeben: ");
		scanf("%ld",&n1);
		getchar();
		printf("Bitte n für Child 2 eineben: ");
		scanf("%ld",&n2);
		getchar();
		printf("Bitte Abstand eingeben: ");
		scanf("%d",&abst);
		getchar();
	}  else {
		n1=atoi(argumente[1]);
		n2=atoi(argumente[2]);
		abst=atoi(argumente[3]);
	}
	

	pid1 = fork();
	if (pid1==-1){
		perror("konnte child1 nicht einrichten");
		return 1;
	}
	if (pid1==0){
		printf("\nChild 1 PID = %d\n",getpid());
		pid2=getppid();
		printf("Parent PID = %d\n",pid2);
	}
	else{
		pid2=fork();
		if(pid2==-1){
			perror("konnte child2 nicht einrichten");
			return 1;
		}
		if(pid2==0){
			printf("Child 2 PID = %d\n",getpid());
			printf("Parent pid = %d\n",getppid());
		}
		else{
			printf("\nParent PID = %d\n",getpid());
		}
	}

	if(pid1==0){
		n=n1;
		//abst=1000;
	}

	if(pid2==0){
		n=n2;
		//abst=1000;
	}
	
	if(pid1==0||pid2==0){	
		strcpy(filename,"Prim");
		argv[0]=filename;
		sprintf(arg1,"%ld",n);
		sprintf(arg2,"%d",abst);
		argv[1]=arg1;
		argv[2]=arg2;
		argv[3]=NULL;
	
		execv(filename,argv);
	}
	if(pid1==0)
		perror("Argh1");
	if(pid2==0)
		perror("Argh2");

	option=WNOHANG;
	while(p1==0||p2==0){
		if(p1==0)	
			p1=waitpid(pid1,&status,option);	
		if(p2==0)		
			p2=waitpid(pid2,&status,option);
	}

	if(p1>0)
		printf("Child-1-PID=%d, status |%x|%x|\n",pid1,(status>>8)&0xFF,status&0x7F);
	if(p1<0)
		perror("Child1 hat Fehler");
	if(p2>0)
		printf("Child-2-PID=%d, status |%x|%x|\n",pid2,(status>>8)&0xFF,status&0x7F);
	if(p2<0)
		perror("Child2 hat Fehler");



	return 0;
}
