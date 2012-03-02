#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include<sys/wait.h>
#include <thread.h>
#include<sys/sem.h>
#include<sys/shm.h>
//Server

     union semun {
             int             val;
             struct semid_ds *buf;
             ushort_t        *array;
     } arg ;


int ende=0;

void sigfkt(int snr){
    switch(snr){
        case SIGINT:{ 
            printf("\nSIGINT\n"); 
            ende=1;
            return;
        }break;
        default:
            exit(-1);
    }
}

void * thrfkt(void *p){
//sonstige
			char dat[20];
			char String[100];
			int l,rc;
			FILE *fl;        
			int sockd2 = * (int*) p;
			pthread_t thrid;

	sigset_t signal_mask;
	rc = pthread_sigmask(SIG_BLOCK, NULL, &signal_mask);


	//shared Memory
	int shmid,shmflag,shmsize,opflag;
	key_t shmkey;
	char *shmadr;

	//Semaphore
	struct sembuf lock[3],unlock[2];
	int semid, nsems, semflag;
	key_t key;

	//Initialisierung des shared memory
	shmsize = sizeof(int);
	shmkey = 4117;
	shmflag = IPC_CREAT | 0664;
	shmid = shmget(shmkey,shmsize,shmflag);
	shmadr = shmat(shmid,0,0);
	
	// Semaphor initialisieren
	key=shmkey;
	nsems = 2;
	semflag = IPC_CREAT | 0644;
	semid = semget(key,nsems,semflag);
	if(semid<0){
		perror("Fehler beim Zugriff auf die Semaphormenge: semget");
		pthread_exit(1);
	}

	printf("Semaphor initiiert\n");
	
	// Semaphoroperation vorbereiten
	printf("Semaphormenge vorbereiten:\n");
	
	// auf SharedMemory-Zugriff vorbereiten,  Semaphor blockieren
	lock[1].sem_num = 0;
	lock[1].sem_op = 0;
	lock[1].sem_flg=0;
	lock[0].sem_num = 0;
	lock[0].sem_op = 0;
	lock[0].sem_flg = 0;
	lock[2].sem_num = 0;
	lock[2].sem_op = 1;
	lock[2].sem_flg = SEM_UNDO;

	unlock[1].sem_num = 1;
	unlock[1].sem_op = 1;
	unlock[1].sem_flg = 0;
	unlock[0].sem_num= 0;
	unlock[0].sem_op= -1;
	unlock[0].sem_flg=SEM_UNDO;

	thrid = pthread_self();
	                                                     
                    l=read(sockd2,String,sizeof(String));                               
                    if(l<0){                                                        
                        perror("Fehler beim Empfangen der File-Nachricht: read");
                        pthread_exit(1);
                    }
                    strcpy(dat,String);                                               
                    if((fl=fopen(dat, "r"))==NULL){                                 
                        printf("Kann Datei %s nicht oeffnen/!\n", dat);
                        if((fl=fopen(dat, "w"))==NULL){                             
                            printf("Kann Datei %s nicht erstellen!\n", dat);      
                            pthread_exit(1);
                        }
                        else{
                            printf("Datei %s erfolgreich erstellt!\n", dat);
                            strcpy(String,"1");                                        // 1 = erfolg
                            l=write(sockd2,String,strlen(String)+1);                    
                            if(l<0){                                              
                                perror("Fehler beim Senden der File-Bestaetigungs-Nachricht: write");
                                pthread_exit(1);
                            }
                            else
                                printf("File-Bestaetigungs-Nachricht versand.\n");
                        }
                    }
                    else{								
                        fclose(fl);							
                        printf("Datei %s erfolgreich geoeffnet!\n", dat);
                        strcpy(String,"-1");						// -1 = ueberschreiben
                        l=write(sockd2,String,strlen(String)+1);				
                        if(l<0){							
                            perror("Fehler beim Senden der Ueberschreiben-Nachricht: write");
                            pthread_exit(1);
                        }
                        else
                            printf("Ueberschreiben-Nachricht versand.\n");

                        l=read(sockd2,String,sizeof(String));                        
                        if(l<0){        
                            perror("Fehler beim Empfangen der Ueberschreiben-Nachricht: read");
                            pthread_exit(1);
                        }
                        else if (l>0)                                              
                            printf("Ueberschreiben-Nachricht: %s\n", String);

                        if(strcmp(String,"j")==0){                                    
                            if((fl=fopen(dat, "w"))==NULL){                        
                                printf("Kann Datei %s nicht erstellen!\n", dat);   
                                pthread_exit(1);
                            }
                            else
                                printf("Datei %s neu erstellt\n", dat);
                        }
                        else{                                                       
                            printf("Kann Datei %s nicht ueberschreiben!\n", dat);
                            pthread_exit(1);
                        }
                    }

                    /* Datenempfang */
                    do{
                        l=read(sockd2,String,sizeof(String));                             
                        if(l<0 && !ende){                                                    
                            perror("Fehler beim Empfangen der Nachricht: read");    
                            pthread_exit(1);
                        }
                        else if (l>0){                                              
                            fputs(String,fl);
                            printf("Nachricht: %s\n", String);
                            sprintf(String,"%d",l);                               
                            l=write(sockd2,String,strlen(String)+1);                     
                            if(l<0){                                                
                                perror("Fehler beim Senden der Byte-Nachricht: write");
                                pthread_exit(1);
                            }
                            else
                                printf("Byte-Nachricht versand.\n");
                        }
                    }while(l!=0);

			rc = semop(semid,lock,3);		
			if(rc >= 0){
				memcpy(&thrid,shmadr,sizeof(int));
			}
			rc = semop(semid,unlock,nsems);

                    fclose(fl);                                                     
                    close(sockd2);                                            
                    pthread_exit(1);
}

//start

main(void){               
    int sockd;           
    int sockd2;   
    int sl;  
    struct sockaddr_un uxadr;                                                                                                            
    int abbruch;
	int counter=0;
            
	union semun semval;                                               

    printf("Server starten...\n");
    signal(SIGINT,sigfkt);                                                          

    /* Adressinformation bereitstellen */
    uxadr.sun_family=AF_UNIX;                                                      
    strcpy(uxadr.sun_path,"sockunix");                                              
    printf("Adressinformation bereitgestellt!\n");
    
    /* Socket einrichten */
    if ((sockd=socket(AF_UNIX,SOCK_STREAM,0))<0){                                 
        perror("Fehler beim Socketeinrichten: socket");                           
        exit(1);
    }
    printf("Socket eingerichtet!\n");

    /* Serveroperationen */
    unlink(uxadr.sun_path);                                                        

    /* Socketdeskriptor mit Adressstruktur verbinden */
    sl=sizeof(uxadr);                                                               
    if (bind(sockd,(struct sockaddr*)&uxadr,sl)<0){                                
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

	//main-Thread einrichten
	pthread_t thrid;
	pthread_attr_t attr;
	void *args;
	int rc;
	pthread_t mainthread;
	sigset_t signal_mask;


	//shared Memory
	int shmid,shmflag,shmsize,opflag;
	key_t shmkey;
	char *shmadr;

	//Semaphore
	struct sembuf lock[2],unlock[2];
	int semid, nsems, semflag;
	key_t key;

	//Initialisierung des shared memory
	shmsize = sizeof(int);
	shmkey = 4117;
	shmflag = IPC_CREAT | 0664;
	shmid = shmget(shmkey,shmsize,shmflag);
	shmadr = shmat(shmid,0,0);
	
	// Semaphor initialisieren
	key=shmkey;
	nsems = 2;
	semflag = IPC_CREAT | 0644;
	semid = semget(key,nsems,semflag);
	if(semid<0){
		perror("Fehler beim Zugriff auf die Semaphormenge: semget");
		exit(1);
	}

	// Semaphoroperation vorbereiten
	printf("Semaphormenge vorbereiten:\n");
	
	// auf SharedMemory-Zugriff vorbereiten,  Semaphor blockieren
	lock[1].sem_num = 0;
	lock[1].sem_op = 1;
	lock[1].sem_flg=SEM_UNDO;
	lock[0].sem_num = 0;
	lock[0].sem_op = 0;
	lock[0].sem_flg = 0;

	unlock[0].sem_num= 0;
	unlock[0].sem_op= -1;
	unlock[0].sem_flg=SEM_UNDO;
	unlock[1].sem_num= 1;
	unlock[1].sem_op= -1;
	unlock[1].sem_flg=SEM_UNDO;

	printf("Semaphor initiiert\n");

	mainthread = pthread_self();

	
	sigemptyset(&signal_mask);
	sigaddset(&signal_mask, SIGINT);
	rc = pthread_sigmask(SIG_BLOCK, NULL, &signal_mask);
	
	

	do{
        sockd2=accept(sockd,0,0);                                                
        if(sockd2<0 && !ende){                                                      
            perror("Fehler bei Clientanforderung: accept");
            exit(4);
        }
        printf("\nWarten auf Clientanforderung!\n");

        if(!ende){        
//TREAD beginn                
			args = (int*) sockd2;
			rc = pthread_create(&thrid,NULL,thrfkt,&args);
			if(rc<0){
				perror("pthread");
				exit(1);
			}
			counter++; 
        } 
    }while(!ende);  

    close(sockd);
    abbruch=0;
   //Join der Threads
	while(counter>0){
	
		semctl(semid, 1, GETVAL, semval	);
		if(semval.val == -1 ){
			rc = semop(semid,lock,2);		
			if(rc >= 0){
				memcpy(shmadr,&thrid,sizeof(int));
				rc = semop(semid,unlock,2);
				pthread_join(thrid,NULL);
			}
			counter--;
		}	}
	 unlink(uxadr.sun_path);
    printf("...beende Server.\n");
    
}
