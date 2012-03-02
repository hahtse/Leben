/* sema1.c
 * 1 Semaphormenge mit 1 Semaphor
 * mit Initialisierung
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>
#include <errno.h>

int main () {
  int stat;
  int semid, nsems, semflag;
  char a[10];
  key_t key;
  struct sembuf lock, unlock;
  union semun {
    int val;
    struct semid_ds *buf;
    ushort *array;
  } semval;
  semflag = IPC_CREAT | IPC_EXCL | 0666;
  nsems = 1;  /* Semaphormenge mit 1 Semaphor */	
  key = 17;   /* willkuerlicher Wert */
/* Test ob Sem. Menge existiert und Anlegen mit IPC_EXCL */
  semid = semget(key, nsems, semflag); 
  if (semid < 0) {
    if (errno != EEXIST) {
	  perror("semget1");
	  exit (1);
    }
/* Semaphormenge war schon angelegt */
    semflag = 0666;
    semid = semget(key, nsems, semflag);
    if (semid < 0) {
      perror("semget2");
      exit(1);
    }
  } else {
/* Semaphormenge ist jetzt angelegt worden */
    semval.val = 1;  /* Semaphor nur hier initialisieren */
    stat = semctl (semid, 0, SETVAL, semval);
  }
  lock.sem_num = 0;   /* Sem.nr. 0 */
  lock.sem_op  = -1;  /* warten bis Sem.wert >0 und -1 */
  lock.sem_flg = SEM_UNDO;
  unlock.sem_num = 0;
  unlock.sem_op  = 1; /* Sem.wert +1 */
  unlock.sem_flg = SEM_UNDO;
/* Besetzungsfunktion */  
  stat = semop (semid, &lock, 1);
  if (stat < 0) perror ("semop");
/* kritischer Abschnitt */	
  puts ("k. A. weiter mit Enter");
  fgets(a, sizeof(a), stdin);
/* Freigabefunktion */
  stat = semop (semid, &unlock, 1);
  if (stat < 0) perror ("semop");
  puts ("Semaphormenge loeschen (j/n) ?");
  fgets(a, sizeof(a), stdin);
  if (a[0] == 'j') {  
    stat = semctl (semid, 0, IPC_RMID, 0);
    if (stat < 0) perror ("semctl");
  }
  exit (0);
}

