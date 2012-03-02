#include <sys/msg.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct nachricht{
	char mtext[100];
	pid_t spid;
	int flag;
};

struct mymsg{
	long mtype;
	struct nachricht nachricht;
};

int main(void){
	int msqid,msqflag;
	key_t msqkey;
	struct mymsg msg;
	size_t len;
	int l, flag;
	char errstring[100];
	long msgtyp;
	int anzsender=0;
	FILE *out;
	char fname[10];
	int rc;

	errno=0;
	msqkey=1337;
	msqflag=IPC_CREAT|0644;
	msqid=msgget(msqkey,msqflag);
	msg.nachricht.spid=getpid();
	printf("\nPID: %d\n",msg.nachricht.spid);
	printf("\n");
	
	len=sizeof(msg.nachricht);
	msgtyp=0;
	flag=0;

	do{
		l=msgrcv(msqid,&msg,len,msgtyp,flag);
		if(l<0){
			printf("\n\nEmpfangsfehler\n");
			break;	
		} else {
			sprintf(fname,"%d.txt",msg.nachricht.spid);
			printf("Nachricht empfangen: %s\n", msg.nachricht.mtext);
			out=fopen(fname,"a");
			if(msg.nachricht.flag==2){
				printf("\nNeuer Sender %d\n",msg.nachricht.spid);
				anzsender++;
			}
			if(msg.nachricht.flag==0){
				printf("\nSender %d Terminiert\n",msg.nachricht.spid);
				anzsender--;
			}
			if(out!=NULL){
				fputs(msg.nachricht.mtext,out);
			} else {
				printf("\n\nFehler beim Schreiben\n");
				break;
			}
		}
	} while(anzsender>=1);



	printf("\nENDE\n");
	rc=msgctl(msqid,IPC_RMID,0);
	sprintf(errstring,"\nempfaenger %d ",getpid());
	perror(errstring);
	printf("\nerrno: %d\n",errno);
	return 0;
}
	
