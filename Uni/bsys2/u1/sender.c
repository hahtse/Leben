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
	int rc=0, flag;
	char errstring[100], *endstring;

	endstring="\0";
	errno=0;
	msqkey=1337;
	msqflag=IPC_CREAT|0644;
	msqid=msgget(msqkey,msqflag);
	msg.nachricht.spid=getpid();
	msg.nachricht.flag=2;
	while(rc>=0){
		if(!fgets(msg.nachricht.mtext,sizeof(msg.nachricht.mtext),stdin)){
			sprintf(msg.nachricht.mtext,"");
			msg.nachricht.flag=0;
			msg.mtype=1;
			len=sizeof(msg.nachricht);
			printf("%d",len);
			flag=0;
			rc=msgsnd(msqid,&msg,len,flag);
			if(rc<0){
				break;
			} 
			printf("\nSender %d beendet\n",getpid());
			return 0;
		}
		msg.mtype=1;
		len=sizeof(msg.nachricht);
		flag=0;
		rc=msgsnd(msqid,&msg,len,flag);
		msg.nachricht.flag=1;
		printf("gesendet\n");
	}
	sprintf(errstring,"\nsender %d ",getpid());
	perror(errstring);
	printf("\nerrno: %d\n",errno);	
}
	
