#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <time.h>

int main(void){
	int rc;
	char temp[100];
	
	//variablen fuer shared memory
	int shmid;
	int shmflag = IPC_CREAT | 0644;			//die üblichen zugriffsrechte
	key_t shmkey = 17;				//identifikation
	size_t shmsize = 110;				//und größe des bereichs
	char *shmadr;
	void *adr;
	
	//variablen fuer semaphore
	int semID;
	struct sembuf sema;
	
	//shared memory
	shmid = shmget(shmkey, shmsize, shmflag);	//shared memory anlegen
	if(shmid <0){
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
	while(1){
		sema.sem_op  = -1; //sperren
 		sema.sem_flg = SEM_UNDO;
 		if (-1==semop(semID, &sema, 1)) {
      	    		perror("semop");
      		}
	
		
		if(strlen(shmadr)!=0) {
			sema.sem_op  = 1;
			if (-1==semop(semID, &sema, 1)) { //freigeben
        	            perror("semop");
        		}
			usleep(500);
   	         	continue; //wenn der memory grade belegt ist, dann wieder in die schleife -->warten
  	      	}
		
		
		//anmeldung schicken
		printf("anmeldung\n");
		sprintf(shmadr,"%10d", getpid());
		
		sema.sem_op  = 1; //freigeben
		if (-1==semop(semID, &sema, 1)) {
            		perror("semop");
        	}
        	break;
	}
	
 	printf("Eingabe: ");
	while(fgets(temp, sizeof(temp), stdin)){
		while(1){
		sema.sem_op  = -1;
 		sema.sem_flg = SEM_UNDO;
 		if (-1==semop(semID, &sema, 1)) { //memory sperren
      	    		perror("semop");
      		}
		if(strlen(shmadr)!=0) {
			
			sema.sem_op  = 1;
			if (-1==semop(semID, &sema, 1)) { //wieder freigeben
        	            perror("semop");
        		}
        		usleep(5000);
			
			
   	         	continue; //und nochmal probieren
  	      	}
		//schicken
		sprintf(shmadr,"%10d%s", getpid(),temp);
				
		sema.sem_op  = 1;
		if (-1==semop(semID, &sema, 1)) { //mem freigeben
            	
            		perror("semop");
        	}
        	break;
		}							
	}

	while(1){
		sema.sem_op  = -1;
 		sema.sem_flg = SEM_UNDO;
 		if (-1==semop(semID, &sema, 1)) {
      	    		perror("semop");
      		}
		if(strlen(shmadr)!=0) {
			
			sema.sem_op  = 1;
			if (-1==semop(semID, &sema, 1)) {
        	            perror("semop");
        		}
        		usleep(5000);
        		
			
   	         	continue;
  	      	}
		//abmeldung schicken
		printf("abmeldung\n");
		sprintf(shmadr,"%10d", -getpid());
				
		sema.sem_op  = 1;
		if (-1==semop(semID, &sema, 1)) {
            	
            		perror("semop");
        	}
        	break;
	}
}
