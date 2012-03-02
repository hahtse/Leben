#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>


int ende=0;                                                                      /* Beendigungsflag */

void sigfkt(int snr){                                                          /* Signalbehandlungsfunktion */
    	switch(snr){                                                                 /* w�hle aus nach Signal */
        	case SIGINT: 
            	printf("\nSIGINT\n"); 
            	ende=1;                                                           /* setze Beendigungsflag fuer Server */
            	return;
        		break;
        	default:
            	exit(-1);
    	}
}


main(void){
    	pid_t pid1,p1;                                                                  /* Prozessvariablen */
    	int sockd;                                                                      /* Serversocket */
    	int sockd2;                                                                     /* Clientsocket */
   	int l,sl;                                                                       /* Rueckgabestatus */
    	struct sockaddr_un uxadr;                                                       /* Datenbereich fuer Adressinformation */
    	char eab[100],dat[20];                                                          /* E/A-Bereich */
    	int status1, abbruch;                                                           /* Statusvariablen */
    	FILE *fl;                                                                       /* Filevariable */

    	printf("Server starten...\n");
    	sigset(SIGINT,sigfkt);                                     /* Signalbehandlung f�r SIGINT einschalten */

    	/* Adressinformation bereitstellen */
    	uxadr.sun_family=AF_UNIX;                                                  /* Internationanl: AF_INTER */
    	strcpy(uxadr.sun_path,"sockunix");                                         /* 1 File fuer Datenaustausch */
    	printf("Adressinformation bereitgestellt!\n");
    
    	/* Socket einrichten */
    	if ((sockd=socket(AF_UNIX,SOCK_STREAM,0))<0){                                   /* Socket einrichten */
   		perror("Fehler beim Socketeinrichten: socket");                             /* Fehlerausgabe */
        	exit(1);
    	}
    	printf("Socket eingerichtet!\n");

    	/* Serveroperationen */
    	unlink(uxadr.sun_path);                                               /* File darf nicht existieren */

    	/* Socketdeskriptor mit Adressstruktur verbinden */
    	sl=sizeof(uxadr);                                                               /* Groesse der Adressinformation */
    	if (bind(sockd,(struct sockaddr*)&uxadr,sl)<0){                    /* Socketdeskriptor mit Adressstruktur verbinden */
        	perror("Fehler bei Verbinden des Socketdeskriptor mit der Adressstruktur: bind");
        	exit(2);
	}
    	printf("Socket-File angelegt!\n");

    	/* Warteschlange fuer Clientanforderung einrichten */
    	if (listen(sockd,5)<0){
        	perror("Fehler beim Einrichten der Warteschlange fuer die Clientanforderung: listen");
        	exit(3);
    	}
    	printf("Warteschlange fuer Clientanforderung eingerichtet!\n");

    	do{
        	sockd2=accept(sockd,0,0);                             /* Warten auf Clientanforderung */        
        	if(sockd2<0 && !ende){                                                      /* Fehler bei Clientanforderung */
            	perror("Fehler bei Clientanforderung: accept");
            	exit(4);
        	}
       	printf("\nWarten auf Clientanforderung!\n");

        	if(!ende){
            	pid1=fork();                                                            /* 1.Child starten */
            	switch(pid1){
                	case -1:                                                           /* kein Client gestartet */
                    	perror("fork");           /* Fehlerausgang falls kein Child gestartet werden konnte */
                    	exit(1);                                                        /* Abbruch */
                		break;
                	case 0:   
                		close(sockd); 
                		sigset(SIGINT, SIG_IGN);                                                       /* Client */
                    	l=read(sockd2,eab,sizeof(eab));                                 /* Filename einlesen */
                    	if(l<0){                                                        /* Fehlerausgabe */
                        		perror("Fehler beim Empfangen der File-Nachricht: read");
                        		exit(0);
                    	}
                    	strcpy(dat,eab);                    /* Einlesepuffer in Filenamenvariable kopieren */
                    	if((fl=fopen(dat, "r"))==NULL){                                 /* Datei oeffnen */
                        		printf("Kann Datei %s nicht oeffnen/!\n", dat);
                        		if((fl=fopen(dat, "w"))==NULL){            /* versuche Datei zu erstellen */
                            		printf("Kann Datei %s nicht erstellen!\n", dat);        /* Fehlerausgabe */
                            		exit(0);
                        		}else{
                            		printf("Datei %s erfolgreich erstellt!\n", dat);
                            		strcpy(eab,"1");                                        /* Erfolgskennzeichen */
                            		l=write(sockd2,eab,strlen(eab)+1);                      /* File-Bestaetigung senden */
                            		if(l<0){                                                /* Fehlerausgabe */
                                		perror("Fehler beim Senden der File-Bestaetigungs-Nachricht: write");
                                		exit(0);
                            		}else{
                                		printf("File-Bestaetigungs-Nachricht versand.\n");
                        			}
                    		}
                    	}else{                                                            /* Datei geoeffnet */
                        		fclose(fl);                                                 /* Datei schliessen */
                        		printf("Datei %s erfolgreich geoeffnet!\n", dat);
                        		strcpy(eab,"-1");                                           /* Ueberschreibenkennzeichen */
                        		l=write(sockd2,eab,strlen(eab)+1);        /* Ueberschreiben-Abfrage senden */
                        		if(l<0){                                                    /* Fehlerausgabe */
                            		perror("Fehler beim Senden der Ueberschreiben-Nachricht: write");
                            		exit(0);
                        		}else{
                            		printf("Ueberschreiben-Nachricht versand.\n");
						}
                       		l=read(sockd2,eab,sizeof(eab));             /* Ueberschreiben-Antwort lesen */
                        		if(l<0){                                                    /* Fehlerausgabe */
                            		perror("Fehler beim Empfangen der Ueberschreiben-Nachricht: read");
                            		exit(0);
	                        	}else{ 
	                        		if (l>0){                              /* Ueberschreiben-Nachricht Ausgabe */
                            			printf("Ueberschreiben-Nachricht: %s\n", eab);
							}
						}
                        		if(strcmp(eab,"j")==0){                                     /* Datei ueberschreiben? */
                            		if((fl=fopen(dat, "w"))==NULL){             /* Datei neu erstellen/ueberschreiben */
                                		printf("Kann Datei %s nicht erstellen!\n", dat);    /* Fehlerausgabe */
                                		exit(0);
                            		}else{
                                		printf("Datei %s neu erstellt\n", dat);
                        			}
                        		}else{                                                       /* Fehlerausgabe */
                            		printf("Kann Datei %s nicht ueberschreiben!\n", dat);
                            		exit(0);
                        		}
                    	}

                    	/* Datenempfang */
                    	do{
                        		l=read(sockd2,eab,sizeof(eab));                             /* Datenstrom lesen */
                        		if(l<0 && !ende){                                                    /* Fehlerausgabe */
                            		perror("Fehler beim Empfangen der Nachricht: read");    
                            		exit(0);
                        		}else 
                        
                        			if (l>0){                                         /* Nachricht in File schreiben */
                            			fputs(eab,fl);
                            			printf("Nachricht: %s\n", eab);
                            			sprintf(eab,"%d",l);            /* Anzahl uebertragene Bytes in Sendepuffer */
                            			l=write(sockd2,eab,strlen(eab)+1);/* Anzahl empfangenen Bytes an Client senden */
                            			if(l<0){                                                /* Fehlerausgabe */
                                			perror("Fehler beim Senden der Byte-Nachricht: write");
                                			exit(0);
                            			}else{
			                              printf("Byte-Nachricht versand.\n");
		                      		}
		                        }
                    	}while(l!=0);

                    	fclose(fl);                                                     /* Datei schliessen */
                    	close(sockd2);                                                  /* Clientsocket schliessen */
                    	exit(0);
                		break;
                	default:                                                           /* Fortf�hrung Parent-Prozess */
                    	printf("\nParent-Prozess\n");
                    	printf("Prozessnummer: %d\n", getpid());
                    	close(sockd2);                                                  /* Kommunikationsende Parent */
                		break;
            	}
        	} 
    	}while(!ende);                                                                  /* Solange bis Server beendet */
	close(sockd);
    	abbruch=0;
    	while ((pid1=wait(&status1))>0)     {            /* warte auf Beendigung des Child-Prozesses und gebe Status aus */
        printf("Ende-Status %x %x von Child %d\n", (status1>>8)&0xFF, status1&0xFF,pid1);
	}
    	/* Serverende */
    	printf("...beende Server.\n");
    	
}