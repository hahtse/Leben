#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>



void *CreateCharArray(int len){                                         /* dynamische Speicherplatzzuweisung f�r ein Array */
	return malloc(len * sizeof(double));
}


main(int argc,char *argv[]){
	int sockd;                                                          /* Socket */
    	int l,sl;                                                           /* Rueckgabestatus */
    	struct sockaddr_un uxadr;                                           /* Datenbereich fuer Adressinformation */
    	char eab[100];                                                      /* E/A-Bereich */
    	char *dat, c;                                                       /* Dateiname, janein-Abfrage */
    	int error, weiter=1, fehler;                                        /* logische Variablen */

    /* Kommandozeileabfrage und eventuell Dateieingabe per stdin */
    	if(argc>1){                                                         /* File ueber Kommandozeile uebergeben? */
    		dat=argv[1];                                                    /* Filename aus 1. Uebergabeparameter lesen */
        	error=0;                                                        /* Argument uebergeben */
        	fehler=0;                                                       /* Kein Fehler aufgetreten */
        	weiter=1;                                                       /* ueberspringe Eingabe des Filenamens */
    	}
    	else{                                                               /* Fehlerausgabe */
        	printf("Keine Kommandozeilen-Parameter angegeben!\n Kommandozeile client file\n");
        	error=1;                                                        /* kein Argument uebergeben */
    	}
    	do{
        	if(error){                                                      /* kein Argument uebergeben */
            	dat=CreateCharArray(20);                                    /* Speicher fuer Filenamenarray zuweisen */
            	printf("Dateiname: ");
            	fehler=scanf("%20s", dat);                                  /* Eingabe Filename */
            	if(fehler>0){                                               /* Eingabe? */
               	fehler=0;                                               /* kein Fehler aufgetreten */
                	weiter=1;                                               /* Filename erfolgreich eingegeben, Beende Eingabeschleife */
            	}else{                                                       /* Filename nicht gueltig */
                	printf("Kein gueltiger Dateiname eingeben!\n Anderen Dateinamen eingeben (j/n)? ");
                	scanf("%1s", &c);
                	if(c=='j'){                                              /* Eingabe Filename? */
                    	error=1;								/* weiterer Versuch Filename einzugeben */
				}else{                                                   /* Programmende */
                    	printf("Beende Programm!\n");
                   	 	weiter=1;                                           /* Beende Eingabeschleife */
                   	 	fehler=1;                                           /* Ein Fehler ist aufgetreten */
                	}
            	}
        	}
    	}
    	while(!weiter);

    	if(!fehler){                                                        /* Filename erfolgreich angegeben? */
        	printf("Client starten...\n");

        	/* Adressinformation bereitstellen */
        	uxadr.sun_family=AF_UNIX;                                       /* International: AF_INTER */
        	strcpy(uxadr.sun_path,"sockunix");                              /* 1 File fuer Datenaustausch */
        	printf("Adressinformation bereitgestellt!\n");

        	/* Socket einrichten */
        	sockd=socket(AF_UNIX,SOCK_STREAM,0);
        	if ((sockd)<0){                                                 /* Fehler beim Socket einrichten? */
            	perror("Fehler beim Socheteinrichten: socket");
            	exit(1);
        	}
        	printf("Socket eingerichtet!\n");

        	/* Verbindung mit Server herstellen */
        	sl=sizeof(uxadr);                                               /* Groesse der Adressinformation */
        	if (connect(sockd,(struct sockaddr*)&uxadr,sl)<0){              /* Anforderung an Server */
            	perror("Fehler bei Serveranforderung: connect");                        /* Fehlerausgabe */
            	exit(1);
        	}
        	printf("Verbindung mit Server hergestellt!\n");

        	/* Datenkommunikation mit Server */
        	strcpy(eab,dat);                                                /* Filename in Sendepuffer kopieren */
        	l=write(sockd,eab,strlen(eab)+1);                               /* Nachricht senden */
        	if(l<0){                                                        /* Fehlerausgabe */
          	perror("Fehler beim Senden der File-Nachricht: write");
            	exit(0);
        	}else
            	printf("File-Nachricht versand.\n");

        		l=read(sockd,eab,sizeof(eab));                                  /* auf Serverantwort warten */
        		if(l<0){                                                        /* Fehlerausgabe */
            		perror("Fehler beim Empfangen der File-Bestaetigungs-Nachricht: read");
            		exit(0);
        		}else{
				if(l>0){                                                  /* Empfang erfolgreich */
            			printf("Nachricht: %s\n", eab);                             /* Ausgabe der Nachricht */
        			}
			}
        		if(strcmp(eab,"-1")==0){                                        /* existiert Datei schon? */
            		printf("Soll Datei %s ueberschrieben werden (j/n)? ", dat);
            		scanf("%1s", &eab);                                         /* Soll Datei ueberschrieben werden? */
            		l=write(sockd,eab,strlen(eab)+1);                           /* Antwort an Server senden */
            		if(l<0){                                                    /* Fehlerausgabe */
                		perror("Fehler beim Senden der Bestaetigungs-Nachricht: write");
                		exit(0);
            		}else{
                		printf("Bestaetigungs-Nachricht versand.\n");
				}            
				if(eab[0]!='j'){                                             /* Eingabe Dateiname? */
                		exit(0);
        			}
			}
        		sprintf(eab,"%s","\0");                                         /* Sendepuffer loeschen */
        		fflush(stdin);                                                  /* Eingabepuffer loeschen */
        		printf("\nText: \n");
        		while((fgets(eab, sizeof(eab), stdin))!=NULL){                  /* Sende stdin bis Ctrl-D, einlesen stdin */
            		l=write(sockd,eab,strlen(eab)+1);                           /* Nachricht senden */
            		if(l<0){                                                     /* Fehlerausgabe */
                		perror("Fehler beim Senden der Nachricht: write");
            		}else{
                		printf("Nachricht versand.\n");
                		l=read(sockd,eab,sizeof(eab));
               		if(l<0){                                                /* Fehlerausgabe */
                   			perror("Fehler beim Empfangen der Byte-Nachricht: read");
                   			exit(0);
               		}else{ 
						if (l>0){                                          /* Empfang erfolgreich */
                   				printf("uebertragene Bytes: %s\n", eab);
              				}
					}
              			printf("\nText: \n");
            		}
        		}


        			/* Kommunikationsnende */
        			printf("...beende Client.\n");
        			close(sockd);                                                   /* Server: l=read() endet mit l=0 */
    			

	}
}
