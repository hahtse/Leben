#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

// CLIENT

main(int argc,char *argv[]){
    int sockd;                                                          
    int l,sl;                                                           
    struct sockaddr_un uxadr;                                     
    char String[100];                                                    
    char *dat, c;                                                      
    int error, weiter=0, fehler;                                       

    /* KommandozeilStringfrage und eventuell Dateieingabe per stdin */
    if(argc>1){                                                         /* File ueber schon uebergeben? */
        dat=argv[1];                                                    
        error=0;                                                        
        fehler=0;                                                       
        weiter=1;                                                       
    }
    else{                                                               
        printf("Keine Kommandozeilen-Parameter angegeben!\n Kommandozeile client file\n");
        error=1;                                                        
    }
   while(!weiter){
        if(error){                                                     /* kein Argument uebergeben */
            dat=malloc(20 * sizeof(double));                           
            printf("Dateiname: ");
            fehler=scanf("%20s", dat);                                  
            if(fehler>0){                                               
                fehler=0;                                               
                weiter=1;                                               
            }
            else{                                                       
                printf("Kein gueltiger Dateiname eingeben!\n Anderen Dateinamen eingeben (j/n)? ");
                scanf("%1s", &c);
                if(c=='j')                                              
                    error=1;                                            
                else{                                                   
                    printf("Beende Programm!\n");
			exit(1);                                           
                }
            }
        }
    }
    

    if(!fehler){                                                       
        printf("Client starten...\n");

        /* Adressinformation*/
        uxadr.sun_family=AF_UNIX;                                       
        strcpy(uxadr.sun_path,"sockunix"); 
        printf("Adressinformation bereitgestellt!\n");

        /* Socket einrichten */
        sockd = socket(AF_UNIX,SOCK_STREAM,0);
        if ((sockd)<0){                                                
            perror("Fehler beim Socheteinrichten: socket");
            exit(1);
        }
        printf("Socket eingerichtet!\n");

        /* Verbindung mit Server herstellen */
        sl=sizeof(uxadr);                                               
        if (connect(sockd,(struct sockaddr*)&uxadr,sl)<0){              
            perror("connect");                        
            exit(1);
        }
        printf("Verbindung mit Server hergestellt!\n");

        /* Datenkommunikation mit Server */
        strcpy(String,dat);                                                
        l=write(sockd,String,strlen(String)+1);                            
        if(l<0){                                                     
            perror("Fehler beim Senden der File-Nachricht: write");
            exit(0);
        }
        else
            printf("File-Nachricht versand.\n");

        l=read(sockd,String,sizeof(String));
        if(l<0){							
            perror("Fehler beim Empfangen der File-Bestaetigungs-Nachricht: read");
            exit(0);
        }
        else if (l>0){	
            printf("Nachricht: %s\n", String); 
        }
        if(strcmp(String,"-1")==0){                                        /* existiert Datei schon? */
            printf("Soll Datei %s ueberschrieben werden (j/n)? ", dat);
            scanf("%1s", &String);                                         /* Soll Datei ueberschrieben werden? */
            l=write(sockd,String,strlen(String)+1);                    
            if(l<0){                                                  
                perror("Fehler beim Senden der Bestaetigungs-Nachricht: write");
                exit(0);
            }
            else
                printf("Bestaetigungs-Nachricht versand.\n");
            if(String[0]!='j')                                             
                exit(0);
        }

        printf("\nText: \n");
        while((fgets(String, sizeof(String), stdin))!=NULL){            
            l=write(sockd,String,strlen(String)+1);
            if(l<0)
                perror("Fehler beim Senden der Nachricht: write");
            else{
                printf("Nachricht versand.\n");
                l=read(sockd,String,sizeof(String));
                if(l<0){
                    perror("Fehler beim Empfangen der Byte-Nachricht: read");
                    exit(0);
                }
                else if (l>0){
                    printf("uebertragene Bytes: %s\n", String);
                }
                printf("\nText: \n");
            }
        }

        printf("so das wars arbeit beendet : Client.\n");
        close(sockd);                                                   /* l=read() endet auf l=0 siehe server */
    }

}
