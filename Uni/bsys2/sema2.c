/* sema2.c
 * 1 Semaphormenge mit 1 Semaphor
 * ohne Initialisierung
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

int main () {
  int stat;
  int semid, nsems, semflag;
  char a[10];
  key_t key;
  struct sembuf lock[2], unlock;
  semflag = IPC_CREAT | 0666;
  nsems = 1;  /* Semaphormenge mit 1 Semaphor */
  key = 17;   /* willkuerlicher Wert */	
  semid = semget (key, nsems, semflag);
  if (semid<0) {
    perror ("semget");
    exit(1);
  }
  lock[0].sem_num = 0;  /* Sem.nr. 0 */
  lock[0].sem_op  = 0;  /* warten bis Sem.wert 0 wird */
  lock[0].sem_flg = 0;
  lock[1].sem_num = 0;  /* Sem.nr. 0 */
  lock[1].sem_op  = 1;  /* wenn Sem. frei Sem.wert +1 */
  lock[1].sem_flg = SEM_UNDO;
  unlock.sem_num = 0;  /* Sem.nr. 0 */
  unlock.sem_op  = -1; /* Sem.wert -1 */
  unlock.sem_flg = IPC_NOWAIT | SEM_UNDO;
/* Besetzungsfunktion */  
  stat = semop (semid, lock, 2);
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
