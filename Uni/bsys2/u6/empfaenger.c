#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include<signal.h>
#include <time.h>
int ende = 0;
void sigfkt(int snr){   //signalbehandlung, CTRL+C setzt ende-flag                                                      
    	switch(snr){                                                              
        	case SIGINT: 
            	printf("\nSIGINT\n"); 
            	ende=1;                                                          
          	break;
    	}
}


int main(void){
	int counter = 0;
	
	char temp [100];
	char key[11];
	int rc;
	
	//variablen fuer shared memory
	int shmid;
	int shmflag = IPC_CREAT | 0644;
	key_t shmkey = 17;
	size_t shmsize = 110;
	char *shmadr; 
	void *adr;

	//variablen fuer semaphore
	int semID;
	struct sembuf sema;	//ohne semaphor-initialisierung
	
 	sigset(SIGINT,sigfkt);  //signalbehandlung anmelden
 	
 	//shared memory
	shmid = shmget (shmkey, shmsize, shmflag);
	if(shmid < 0){
		perror("shmget");
		exit(-1);
	}
	
	adr = NULL;
	shmadr = shmat (shmid, adr, 0); 
	if(shmadr == (char *)(-1)){
		perror("shmat");
		exit(-1);
	}
	
  	//semaphore
	semID = semget(2406, 1, IPC_CREAT | 0666);
	if (semID < 0) {
		perror("semget");
		exit(-1);
	}
	sema.sem_num=0;
	sema.sem_op = 0;

	do{
	
		//semaphore
		//lock
 		sema.sem_flg = SEM_UNDO;
 		if (-1==semop(semID, &sema, 1)) {
   	         	perror("semop1");
  	      	}
		if(strlen(shmadr)==0) {
			sema.sem_op  = 1;
			if (-1==semop(semID, &sema, 1)) {
        	            perror("semop2");
        		}
			usleep(5000);
			sema.sem_op  = -1;
   	         	continue;		//warte solange sharedmemory nichts enthält
  	      	}
	
		strncpy(key, shmadr, 10);
		key[10] = '\0';
		int ikey=atoi(key);
		if(strlen(shmadr) > 10){				//wenn sharedmem-Adr. > 10 --> Nachricht
			strncpy(temp, shmadr+10, strlen(shmadr)-9);
			if(temp[strlen(shmadr)-10]!='\0'){
				perror("EOF fehlt");	
			}
			printf("%d: %s",ikey, temp);
		}else{							//sonst anmeldung/abmeldung
			if(ikey >= 0){
					counter++;
					printf("Neuer Sender: %d\n", ikey);
			}else{
					counter--;
					printf("Sender %d beendet\n", -ikey);
			}
		}
		sprintf(shmadr,'\0');
		
		
		sema.sem_op  = 1;		//unlock
		if (-1==semop(semID, &sema, 1)) {
            		
            		perror("semop3");
       	 	}
		sema.sem_op  = -1;
 		sema.sem_flg = SEM_UNDO;
  	}while( !ende || counter>0); 

	shmdt(shmadr);						//Speichersegment abkoppeln

	rc = shmctl(shmid, IPC_RMID, 0);			//und freigeben
	if(rc < 0){
			perror("Fehler beim freigeben");
	}
	printf("Speicher erfolgreich freigeben\n");
	semctl(semID, 1, IPC_RMID, 0);				//Semaphore löschen
}
