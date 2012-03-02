#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	int sockd;
	int l, sl, flags=0, rc;
	struct sockaddr_un srvadr, cladr;
	char eab[100];

	srvadr.sun_family = AF_UNIX;						//Adressstrukturen initialisieren
	cladr.sun_family = AF_UNIX;
	strcpy(srvadr.sun_path, "sockdgram1");
	//client socket file
	char fname[50], *p;
	strcpy(fname, "sockXXXXXX");
	p=mktemp(fname);
	printf("%s\n",fname);
	strcpy(cladr.sun_path, fname);
	
	sockd = socket(AF_UNIX, SOCK_DGRAM, 0);					//Socket erzeugen
	if(sockd<0){
		perror("sockd");
		exit(1);
	}
	
	rc=bind(sockd, (struct sockaddr *)&cladr, sizeof(cladr));		//Adressstruktur und Socket verbinden
	if(rc<0){
		perror("bind");
		exit(1);
	}
	
	while(fgets(eab,sizeof(eab),stdin)!=NULL){				//Solange strg+D nicht kam
		sl = sizeof(srvadr);						//Anforderung an Server senden
		l=sendto(sockd, eab, strlen(eab)+1, flags, (struct sockaddr *)&srvadr,sl);
		l=recvfrom(sockd,eab,sizeof(eab),flags, NULL, NULL);
		if(strcmp(eab,"exit")==0){					//alternative endmöglichkeit: "exit" vom server kriegen
			break;
		}
	}

	strcpy(eab,"exit");
	l=sendto(sockd, eab, strlen(eab)+1, flags, (struct sockaddr *)&srvadr,sl);	//mit senden von "exit" bei server abmelden
	close(sockd);									//und socket schließen
	unlink(cladr.sun_path);
}
