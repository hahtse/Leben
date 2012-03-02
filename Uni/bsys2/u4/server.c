#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>

int ende = 0;

void signalBehandlung(int i){
	signal(2, SIG_IGN);					//ctrl + c ignorieren
	ende=1;							//...und das Ende einläuten
}



int main(){
	FILE  *f;
	int counter=0;
	int sockd;
	int l, sl, flags=0, rc;
	struct sockaddr_un srvadr, cladr;
	char eab[100];
	char fname[100];
	char dbuf[100];
	srvadr.sun_family = AF_UNIX;				//Socketfiles für Server festlegen
	strcpy(srvadr.sun_path, "sockdgram1");			//Adressstruktur initialisiert
	unlink(srvadr.sun_path);
	signal(2,signalBehandlung);				//signalbehandlung anmelden

	sockd = socket(AF_UNIX, SOCK_DGRAM, 0);			//Socket einrichten
	if(sockd<0){
		perror("socket");
		exit(1);
	}
	rc=bind(sockd, (struct sockaddr *)&srvadr, sizeof(srvadr));		//Adressstruktur und Socket verbinden
	if(rc < 0){
		perror("bind");
		exit(1);
	}

	//empfangen
	do{
		sl=sizeof(cladr);		
		l=recvfrom(sockd, eab, sizeof(eab), flags, (struct sockaddr*)&cladr, &sl); //von wem wird empfangen? (blockierend)
		sprintf(fname,"%s.txt",cladr.sun_path);
		f=fopen(fname,"r");
		if(ende==1 && f==0 ){
			strcpy(eab,"exit");
			l=sendto(sockd,eab,strlen(eab)+1,flags, (struct sockaddr *)&cladr, sl); //Quittierung an alle neuen Clients nach Ende senden
		}else{
			if(f==0){
				counter++;
				printf("Neuer Client\n");
			}else{
				fclose(f);		//wenn file nicht vorhanden abmelden
			}
			if(strcmp(eab, "exit")!=0){	//wenn client noch nicht geschlossen wurde				
				f=fopen(fname, "a");	//client-file mit append öffnen
				sprintf(dbuf,"%s",eab);
				fwrite(dbuf, sizeof(dbuf), 1, f);
				fclose(f);
				sprintf(eab,"%d",(int) strlen(eab));
				l=sendto(sockd,eab,strlen(eab)+1,flags, (struct sockaddr *)&cladr, sl); //antwort (länge der nachricht) senden
			}else{
				f=fopen(fname,"r");	//wenn client geschlossen wurde
				if(f==0){		//...und ein clientfile existiert
				}else{
					printf("\nVon Sender %s\n", fname);	//clientfile auf stdout ausgeben
					while(fread(&eab, sizeof(eab), 1, f)>0){
						printf("%s",eab);
					}
					fclose(f);
					remove(fname);
					counter--;
				}
			}
		}
	}while(counter>0);		//so lange es clients gibt.
	//Serverende
	close(sockd);
	unlink(srvadr.sun_path);
}

