#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	
	int i,j,n,ch;
	char filename[100];
	FILE *fp;	
	char wahl;
	pid_t pid1=1,pid2=1;
	int status1,status2;
	char azffile[100], filedeskript[100], *argvektor[4];
	
	printf("\n");
	if(argc>1){
		strcpy(filename,argv[1]);
	}
	else{
		printf("Bitte Filenamen eingeben: ");
		fgets(filename,100,stdin);
		filename[strlen(filename)-1]='\0';
		printf("\n");
	}


	fp = fopen(filename,"r");
	if(fp==NULL){
		fp = fopen(filename,"w+");
	}
	else
	{
		printf("\nFile schon vorhanden. (N)eu einrichten oder neuen Inhalt hinten (A)nhängen?  ");
		while((wahl=getchar())!='N'&&wahl!='n'&&wahl!='A'&&wahl!='a');
		if(wahl == 'N' || wahl == 'n'){
			fclose(fp);
			fp = fopen(filename,"w+");
		}
		if(wahl == 'A' || wahl == 'a'){
			fclose(fp);
			fp = fopen(filename,"a+");
		}
	}
	
	while ((ch = getchar()) != '\n' && ch != EOF);

	strcpy(azffile,"ueb7.1");
	sprintf(filedeskript,"%d",fileno(fp));
	printf("filedeskriptor: %s\n",filedeskript);
 	printf("File-Deskriptoren zu stdin, stdout und stderr : ");
 	printf("%d, %d und %d\n",fileno(stdin),fileno(stdout),fileno(stderr));


	argvektor[0]=azffile;
	argvektor[1]=filename;
	argvektor[2]=filedeskript;
	argvektor[3]=NULL;

	
	pid1 = fork();
	if (pid1==-1){
		perror("konnte child1 nicht einrichten");
		return 1;
	}
	if (pid1==0){
		printf("\nChild 1 PID = %d\n",getpid());
		printf("Parent PID = %d\n",getppid());
		execv(azffile,argvektor);
		perror("Wenn du das hier siehst dann laeuft was schief");
		return 1;
	}
	if (pid1!=0){
		printf("\nParent mit PID %d wartet\n",getpid());
		pid1=wait(&status1);
		printf("\nParent läuft weiter\n");
		if (pid1==-1){
			perror("Childfehler");
			return 1;
		}
	}
	
	fclose(fp);
	fp=fopen(filename,"r");
	sprintf(filedeskript,"%d",fileno(fp));
	strcpy(azffile,"ueb7.2");
	argvektor[0]=azffile;
	argvektor[1]=filename;
	argvektor[2]=filedeskript;
	argvektor[3]=NULL;
	
	pid2=fork();
	if (pid2==-1){
		perror("konnte child2 nicht einrichten");
		return 1;
	}
	if (pid2==0){
		printf("\nChild 2 PID = %d\n",getpid());
		printf("Parent PID = %d\n",getppid());
		execv(azffile,argvektor);	
	}
	if (pid2!=0){
		printf("Parent wartet\n");
		pid2=wait(&status2);
		printf("Parent läuft weiter\n");
		if (pid2==-1){
			perror("Childfehler");
			return 1;
		}
		
	}

	printf("Child1-PID=%d, status |%x|%x|\n",pid1,(status1>>8)&0xFF,status1&0x7F);
	printf("Child2-PID=%d, status |%x|%x|\n",pid2,(status2>>8)&0xFF,status2&0x7F);
	fclose(fp);
	return 0;
}

	

	




			
		
	
	
	
